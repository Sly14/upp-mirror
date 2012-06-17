#include "Skylark.h"
#ifdef PLATFORM_POSIX
#include <sys/wait.h>
#endif

#ifdef PLATFORM_WIN32
#include <wincon.h>
#endif

#define CONSOLE(x)   Cout() << x << '\n'

#ifdef PLATFORM_WIN32
BOOL WINAPI SkylarkApp::CtrlCHandlerRoutine(__in  DWORD dwCtrlType)
{
	LOG("Ctrl+C handler");
	TheApp().quit = true;
	Cout() << "Ctrl + C\n";
	TcpSocket h;
	h.Connect("127.0.0.1", TheApp().port);
	h.Put("quit");
	return TRUE;
}
#endif

void SkylarkApp::WorkThread()
{
	RunThread();
}

void SkylarkApp::ThreadRun()
{
	WorkThread();
}

void SkylarkApp::RunThread()
{
	SQL.ClearError();
	SQLR.ClearError();
	SQL.GetSession().ThrowOnError();
	SQLR.GetSession().ThrowOnError();
	
	for(;;) {
		TcpSocket request;
		accept_mutex.Enter();
		if(quit) {
			accept_mutex.Leave();
			break;
		}
		bool b = request.Timeout(2000).Accept(server);
		accept_mutex.Leave();
		if(quit)
			break;
		if(b) {
			CONSOLE("Accepted " << Thread::GetCurrentId());
		#ifdef PLATFORM_POSIX
			if(prefork)
				alarm(timeout);
		#endif
			Http http(*this);
			http.Dispatch(request);
		#ifdef PLATFORM_POSIX
			if(prefork)
				alarm(0);
		#endif
			CONSOLE("Finished " << Thread::GetCurrentId());
		}
		else
			CONSOLE("Waiting " << Thread::GetCurrentId());
	}
}

void SkylarkApp::Main()
{
	Buffer<Thread> uwt(threads);
	for(int i = 0; i < threads; i++)
		Thread::Start(THISBACK(ThreadRun));

	while(Thread::GetCount()) {
		if(getpid() == main_pid && (msecs() % 1000) == 0)
			ThreadRun();
		Sleep(100);
	}
}

void SkylarkApp::Broadcast(int signal)
{
#ifdef PLATFORM_POSIX
	if(getpid() == main_pid)
		for(int i = 0; i < child_pid.GetCount(); i++)
			kill(child_pid[i], signal);	
#endif
}

void SkylarkApp::Signal(int signal)
{
#ifdef PLATFORM_POSIX
	switch(signal) {
	case SIGTERM:
		quit = true;
	case SIGHUP:
		Broadcast(signal);
		break;
	case SIGINT:
		Broadcast(signal);
		exit(0);
		break;
	case SIGALRM:
		if(getpid() != TheApp().main_pid) {
			// "Timeout - session stoped"
			exit(0);
		}
		break;
	}
#endif
}

void SkylarkApp::SignalHandler(int signal) 
{
	TheApp().Signal(signal);
}

void DisableHUP()
{
#ifdef PLATFORM_POSIX
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGHUP);
	sigprocmask(SIG_BLOCK, &mask, NULL);
#endif
}

void EnableHUP()
{
#ifdef PLATFORM_POSIX
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset(&mask, SIGHUP);
	sigprocmask(SIG_UNBLOCK, &mask, NULL);
#endif
}

void SkylarkApp::Run()
{
	DisableHUP();
	SqlSession::PerThread();
	SqlId::UseQuotes();
	FinalizeViews();

#ifdef PLATFORM_WIN32
	SetConsoleCtrlHandler(CtrlCHandlerRoutine, true);
#endif

	// Add prefork here
	if(!server.Listen(port, 5)) {
		Cout() << "Cannot open server socket!\n";
		return;
	}

	main_pid = getpid();
	quit = false;

#ifdef PLATFORM_POSIX
	if(prefork) {
		struct sigaction sa;
		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = SignalHandler;
		sigaction(SIGTERM, &sa, NULL);
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGHUP, &sa, NULL);
		sigaction(SIGALRM, &sa, NULL);
		EnableHUP();	
		while(!quit) {
			while(child_pid.GetCount() < prefork && !quit) {
				pid_t p = fork();
				if(p == 0) {
					Main();	
					return;
				}
				else
				if(p > 0)
					child_pid.Add(p);
				else {
					// "cant create new process"
					Broadcast(SIGINT);
					abort();
				}
			}
			int status = 0;
			pid_t p = wait(&status);
			if(p > 0) {
				int q = FindIndex(child_pid, p);
				if(q >= 0)
					child_pid.Remove(q);
			}
		}
	
		Broadcast(SIGTERM);
		int status = 0;
		for(int i = 0; i < child_pid.GetCount(); i++)
			waitpid(child_pid[i], &status, 0);	
		// "server stopped";
	}
	else
#endif
		Main();

	CONSOLE("ExitSkylark");
}


void SkylarkApp::SqlError(Http& http)
{
}

void SkylarkApp::InternalError(Http& http)
{
}

void SkylarkApp::NotFound(Http& http)
{
}

void SkylarkApp::Unauthorized(Http& http)
{
}

SkylarkApp *SkylarkApp::app;

SkylarkApp& SkylarkApp::TheApp()
{
	ASSERT(app);
	return *app;
}

SkylarkApp::SkylarkApp()
{
	ASSERT(!app);
	app = this;
	threads = 3 * CPU_Cores() + 1;
	post_identities = 60;
	port = 8001;
#ifdef _DEBUG
	prefork = 0;
	timeout = 0;
#else
	prefork = 1;
	timeout = 300;
#endif
}

SkylarkApp::~SkylarkApp()
{
	app = NULL;
}
