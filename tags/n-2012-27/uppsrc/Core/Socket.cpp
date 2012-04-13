#include "Core.h"

#ifdef PLATFORM_WIN32
#include <winsock2.h>
	#ifdef COMPILER_MSC
	#include <Ws2ipdef.h>
	#endif
#include <Ws2tcpip.h>
#endif

#ifdef PLATFORM_POSIX
#include <arpa/inet.h>
#endif

NAMESPACE_UPP

#ifdef PLATFORM_WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#define LLOG(x)  // DLOG("TCP " << x)

IpAddrInfo::Entry IpAddrInfo::pool[COUNT];

RawMutex IpAddrInfoPoolMutex;

void IpAddrInfo::EnterPool()
{
	IpAddrInfoPoolMutex.Enter();
}

void IpAddrInfo::LeavePool()
{
	IpAddrInfoPoolMutex.Leave();
}

int sGetAddrInfo(const char *host, const char *port, addrinfo **result)
{
	addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	return getaddrinfo(host, port, &hints, result);
}

rawthread_t rawthread__ IpAddrInfo::Thread(void *ptr)
{
	Entry *entry = (Entry *)ptr;
	EnterPool();
	if(entry->status == WORKING) {
		char host[1025];
		char port[257];
		strcpy(host, entry->host);
		strcpy(port, entry->port);
		LeavePool();
		addrinfo *result;
		if(sGetAddrInfo(host, port, &result) == 0 && result) {
			EnterPool();
			if(entry->status == WORKING) {
				entry->addr = result;
				entry->status = RESOLVED;
			}
			else {
				freeaddrinfo(result);
				entry->status = EMPTY;
			}
		}
		else {
			EnterPool();
			if(entry->status == CANCELED)
				entry->status = EMPTY;
			else
				entry->status = FAILED;
		}
	}
	LeavePool();
	return 0;
}

bool IpAddrInfo::Execute(const String& host, int port)
{
	Clear();
	entry = exe;
	addrinfo *result;
	entry->addr = sGetAddrInfo(~host, ~AsString(port), &result) == 0 ? result : NULL;
	return entry->addr;
}

void IpAddrInfo::Start()
{
	if(entry)
		return;
	EnterPool();
	for(int i = 0; i < COUNT; i++) {
		Entry *e = pool + i;
		if(e->status == EMPTY) {
			entry = e;
			e->addr = NULL;
			if(host.GetCount() > 1024 || port.GetCount() > 256)
				e->status = FAILED;
			else {
				e->status = WORKING;
				e->host = host;
				e->port = port;
				StartRawThread(&IpAddrInfo::Thread, e);
			}
			break;
		}
	}
	LeavePool();
}

void IpAddrInfo::Start(const String& host_, int port_)
{
	Clear();
	port = AsString(port_);
	host = host_;
	Start();
}

bool IpAddrInfo::InProgress()
{
	if(!entry) {
		Start();
		return true;
	}
	EnterPool();
	int s = entry->status;
	LeavePool();
	return s == WORKING;
}

addrinfo *IpAddrInfo::GetResult()
{
	EnterPool();
	addrinfo *ai = entry ? entry->addr : NULL;
	LeavePool();
	return ai;
}

void IpAddrInfo::Clear()
{
	EnterPool();
	if(entry) {
		if(entry->status == RESOLVED && entry->addr)
			freeaddrinfo(entry->addr);
		if(entry->status == WORKING)
			entry->status = CANCELED;
		else
			entry->status = EMPTY;
		entry = NULL;
	}
	LeavePool();
}

IpAddrInfo::IpAddrInfo()
{
	TcpSocket::Init();
	entry = NULL;
}

#ifdef PLATFORM_POSIX

#define SOCKERR(x) x

const char *TcpSocketErrorDesc(int code)
{
	return strerror(code);
}

int TcpSocket::GetErrorCode()
{
	return errno;
}

#else

#define SOCKERR(x) WSA##x

const char *TcpSocketErrorDesc(int code)
{
	static Tuple2<int, const char *> err[] = {
		{ WSAEINTR,                 "Interrupted function call." },
		{ WSAEACCES,                "Permission denied." },
		{ WSAEFAULT,                "Bad address." },
		{ WSAEINVAL,                "Invalid argument." },
		{ WSAEMFILE,                "Too many open files." },
		{ WSAEWOULDBLOCK,           "Resource temporarily unavailable." },
		{ WSAEINPROGRESS,           "Operation now in progress." },
		{ WSAEALREADY,              "Operation already in progress." },
		{ WSAENOTSOCK,              "TcpSocket operation on nonsocket." },
		{ WSAEDESTADDRREQ,          "Destination address required." },
		{ WSAEMSGSIZE,              "Message too long." },
		{ WSAEPROTOTYPE,            "Protocol wrong type for socket." },
		{ WSAENOPROTOOPT,           "Bad protocol option." },
		{ WSAEPROTONOSUPPORT,       "Protocol not supported." },
		{ WSAESOCKTNOSUPPORT,       "TcpSocket type not supported." },
		{ WSAEOPNOTSUPP,            "Operation not supported." },
		{ WSAEPFNOSUPPORT,          "Protocol family not supported." },
		{ WSAEAFNOSUPPORT,          "Address family not supported by protocol family." },
		{ WSAEADDRINUSE,            "Address already in use." },
		{ WSAEADDRNOTAVAIL,         "Cannot assign requested address." },
		{ WSAENETDOWN,              "Network is down." },
		{ WSAENETUNREACH,           "Network is unreachable." },
		{ WSAENETRESET,             "Network dropped connection on reset." },
		{ WSAECONNABORTED,          "Software caused connection abort." },
		{ WSAECONNRESET,            "Connection reset by peer." },
		{ WSAENOBUFS,               "No buffer space available." },
		{ WSAEISCONN,               "TcpSocket is already connected." },
		{ WSAENOTCONN,              "TcpSocket is not connected." },
		{ WSAESHUTDOWN,             "Cannot send after socket shutdown." },
		{ WSAETIMEDOUT,             "Connection timed out." },
		{ WSAECONNREFUSED,          "Connection refused." },
		{ WSAEHOSTDOWN,             "Host is down." },
		{ WSAEHOSTUNREACH,          "No route to host." },
		{ WSAEPROCLIM,              "Too many processes." },
		{ WSASYSNOTREADY,           "Network subsystem is unavailable." },
		{ WSAVERNOTSUPPORTED,       "Winsock.dll version out of range." },
		{ WSANOTINITIALISED,        "Successful WSAStartup not yet performed." },
		{ WSAEDISCON,               "Graceful shutdown in progress." },
		{ WSATYPE_NOT_FOUND,        "Class type not found." },
		{ WSAHOST_NOT_FOUND,        "Host not found." },
		{ WSATRY_AGAIN,             "Nonauthoritative host not found." },
		{ WSANO_RECOVERY,           "This is a nonrecoverable error." },
		{ WSANO_DATA,               "Valid name, no data record of requested type." },
		{ WSASYSCALLFAILURE,        "System call failure." },
	};
	const Tuple2<int, const char *> *x = FindTuple(err, __countof(err), code);
	return x ? x->b : "Unknown error code.";
}

int TcpSocket::GetErrorCode()
{
	return WSAGetLastError();
}

#endif

void TcpSocketInit()
{
#if defined(PLATFORM_WIN32)
	ONCELOCK {
		WSADATA wsadata;
		WSAStartup(MAKEWORD(2, 2), &wsadata);
	}
#endif
}

void TcpSocket::Init()
{
	TcpSocketInit();
}

void TcpSocket::Reset()
{
	is_eof = false;
	socket = INVALID_SOCKET;
	ipv6 = false;
	ptr = end = buffer;
	is_error = false;
	is_abort = false;
	mode = NONE;
	ssl.Clear();
}

TcpSocket::TcpSocket()
{
	ClearError();
	Reset();
	timeout = Null;
	waitstep = 20;
}

bool TcpSocket::Open(int family, int type, int protocol)
{
	Init();
	Close();
	ClearError();
	if((socket = ::socket(family, type, protocol)) == INVALID_SOCKET)
		return false;
	LLOG("TcpSocket::Data::Open() -> " << (int)socket);
#ifdef PLATFORM_WIN32
	u_long arg = 1;
	if(ioctlsocket(socket, FIONBIO, &arg))
		SetSockError("ioctlsocket(FIO[N]BIO)");
#else
	if(fcntl(socket, F_SETFL, (fcntl(socket, F_GETFL, 0) | O_NONBLOCK)))
		SetSockError("fcntl(O_[NON]BLOCK)");
#endif
	return true;
}

bool TcpSocket::Listen(int port, int listen_count, bool ipv6_, bool reuse)
{
	Close();
	Init();
	Reset();

	ipv6 = ipv6_;
	if(!Open(ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0))
		return false;
	sockaddr_in sin;
#ifdef PLATFORM_WIN32
	SOCKADDR_IN6 sin6;
	if(ipv6 && IsWinVista())
#else
	sockaddr_in6 sin6;
	if(ipv6)
#endif
	{
		Zero(sin6);
		sin.sin_family = AF_INET6;
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else {
		Zero(sin);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	if(reuse) {
		int optval = 1;
		setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&optval, sizeof(optval));
	}
	if(bind(socket, ipv6 ? (const sockaddr *)&sin6 : (const sockaddr *)&sin,
	        ipv6 ? sizeof(sin6) : sizeof(sin))) {
		SetSockError(Format("bind(port=%d)", port));
		return false;
	}
	if(listen(socket, listen_count)) {
		SetSockError(Format("listen(port=%d, count=%d)", port, listen_count));
		return false;
	}
	return true;
}

bool TcpSocket::Accept(TcpSocket& ls)
{
	Close();
	Init();
	Reset();

	if(timeout && !ls.WaitRead())
		return false;
	if(!Open(ls.ipv6 ? AF_INET6 : AF_INET, SOCK_STREAM, 0))
		return false;
	socket = accept(ls.GetSOCKET(), NULL, NULL);
	if(socket == INVALID_SOCKET) {
		SetSockError("accept");
		return false;
	}
	mode = ACCEPT;
	return true;
}

String TcpSocket::GetPeerAddr() const
{
	if(!IsOpen())
		return Null;
	sockaddr_in addr;
	socklen_t l = sizeof(addr);
	if(getpeername(socket, (sockaddr *)&addr, &l) != 0)
		return Null;
	if(l > sizeof(addr))
		return Null;
#ifdef PLATFORM_WIN32
	return inet_ntoa(addr.sin_addr);
#else
	char h[200];
	return inet_ntop(AF_INET, &addr.sin_addr, h, 200);
#endif
}

void TcpSocket::NoDelay()
{
	ASSERT(IsOpen());
	int __true = 1;
	LLOG("NoDelay(" << (int)socket << ")");
	if(setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&__true, sizeof(__true)))
		SetSockError("setsockopt(TCP_NODELAY)");
}

void TcpSocket::Linger(int msecs)
{
	ASSERT(IsOpen());
	linger ls;
	ls.l_onoff = !IsNull(msecs) ? 1 : 0;
	ls.l_linger = !IsNull(msecs) ? (msecs + 999) / 1000 : 0;
	if(setsockopt(socket, SOL_SOCKET, SO_LINGER,
		reinterpret_cast<const char *>(&ls), sizeof(ls)))
		SetSockError("setsockopt(SO_LINGER)");
}

void TcpSocket::Attach(SOCKET s)
{
	Close();
	socket = s;
}

bool TcpSocket::RawConnect(addrinfo *rp)
{
	if(!rp) {
		SetSockError("connect", -1, "not found");
		return false;
	}
	for(;;) {
		if(rp && Open(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) {
			if(connect(socket, rp->ai_addr, rp->ai_addrlen) == 0 ||
			   GetErrorCode() == SOCKERR(EINPROGRESS) || GetErrorCode() == SOCKERR(EWOULDBLOCK))
				break;
			Close();
		}
		rp = rp->ai_next;
		if(!rp) {
			SetSockError("connect", -1, "failed");
			return false;
		}
    }
	mode = CONNECT;
	return true;
}


bool TcpSocket::Connect(IpAddrInfo& info)
{
	LLOG("TCP Connect addrinfo");
	Init();
	Reset();
	addrinfo *result = info.GetResult();
	return result && RawConnect(result);
}

bool TcpSocket::Connect(const char *host, int port)
{
	LLOG("TCP Connect(" << host << ':' << port << ')');

	Init();
	Reset();
	IpAddrInfo info;
	if(!info.Execute(host, port)) {
		SetSockError(Format("getaddrinfo(%s) failed", host));
		return false;
	}
	return Connect(info);
}

void TcpSocket::RawClose()
{
	LLOG("TCP close " << (int)socket);
	if(socket != INVALID_SOCKET) {
		int res;
#if defined(PLATFORM_WIN32)
		res = closesocket(socket);
#elif defined(PLATFORM_POSIX)
		res = close(socket);
#else
	#error Unsupported platform
#endif
		if(res && !IsError())
			SetSockError("close");
		socket = INVALID_SOCKET;
	}
}

void TcpSocket::Close()
{
	if(ssl)
		ssl->Close();
	else
		RawClose();
	ssl.Clear();
}

bool TcpSocket::WouldBlock()
{
	int c = GetErrorCode();
#ifdef PLATFORM_POSIX
	return c == SOCKERR(EWOULDBLOCK) || c == SOCKERR(EAGAIN);
#endif
#ifdef PLATFORM_WIN32
	return c == SOCKERR(EWOULDBLOCK) || c == SOCKERR(ENOTCONN);
#endif
}

int TcpSocket::RawSend(const void *buf, int amount)
{
	int res = send(socket, (const char *)buf, amount, 0);
	if(res < 0 && WouldBlock())
		res = 0;
	else
	if(res == 0 || res < 0)
		SetSockError("send");
	return res;
}

int TcpSocket::Send(const void *buf, int amount)
{
	return ssl ? ssl->Send(buf, amount) : RawSend(buf, amount);
}

void TcpSocket::Shutdown()
{
	ASSERT(IsOpen());
	if(shutdown(socket, SD_SEND))
		SetSockError("shutdown(SD_SEND)");
}

String TcpSocket::GetHostName()
{
	Init();
	char buffer[256];
	gethostname(buffer, __countof(buffer));
	return buffer;
}

bool TcpSocket::RawWait(dword flags)
{
	LLOG("Wait(" << timeout << ", " << flags << ")");
	if((flags & WAIT_READ) && ptr != end)
		return true;
	int end_time = msecs() + timeout;
	if(socket == INVALID_SOCKET)
		return false;
	for(;;) {
		if(IsError() || IsAbort())
			return false;
		int to = end_time - msecs();
		if(WhenWait)
			to = waitstep;
		timeval *tvalp = NULL;
		timeval tval;
		if(!IsNull(timeout) || WhenWait) {
			to = max(to, 0);
			tval.tv_sec = to / 1000;
			tval.tv_usec = 1000 * (to % 1000);
			tvalp = &tval;
		}
		fd_set fdset[1];
		FD_ZERO(fdset);
		FD_SET(socket, fdset);
		int avail = select((int)socket + 1,
		                   flags & WAIT_READ ? fdset : NULL,
		                   flags & WAIT_WRITE ? fdset : NULL,
		                   flags & WAIT_EXCEPTION ? fdset : NULL, tvalp);
		LLOG("Wait select avail: " << avail);
		if(avail < 0) {
			SetSockError("wait");
			return false;
		}
		if(avail > 0)
			return true;
		if(to <= 0 && timeout) {
			return false;
		}
		WhenWait();
		if(timeout == 0)
			return false;
	}
}

bool TcpSocket::Wait(dword flags)
{
	return ssl ? ssl->Wait(flags) : RawWait(flags);
}

int TcpSocket::Put(const char *s, int length)
{
	LLOG("Put " << socket << ": " << length);
	ASSERT(IsOpen());
	if(length < 0 && s)
		length = (int)strlen(s);
	if(!s || length <= 0 || IsError() || IsAbort())
		return 0;
	done = 0;
	bool peek = false;
	while(done < length) {
		if(peek && !WaitWrite())
			return done;
		peek = false;
		int count = Send(s + done, length - done);
		if(IsError() || timeout == 0 && count == 0 && peek)
			return done;
		if(count > 0)
			done += count;
		else
			peek = true;
	}
	LLOG("//Put() -> " << done);
	return done;
}

int TcpSocket::RawRecv(void *buf, int amount)
{
	int res = recv(socket, (char *)buf, amount, 0);
	if(res == 0)
		is_eof = true;
	else
	if(res < 0 && WouldBlock())
		res = 0;
	else
	if(res < 0)
		SetSockError("recv");
	LLOG("recv(" << socket << "): " << res << " bytes: "
	     << AsCString((char *)buf, (char *)buf + min(res, 16))
	     << (res ? "" : IsEof() ? ", EOF" : ", WOULDBLOCK"));
	return res;
}

int TcpSocket::Recv(void *buffer, int maxlen)
{
	return ssl ? ssl->Recv(buffer, maxlen) : RawRecv(buffer, maxlen);
}

void TcpSocket::ReadBuffer()
{
	ptr = end = buffer;
	if(WaitRead())
		end = buffer + Recv(buffer, BUFFERSIZE);
}

int TcpSocket::Get_()
{
	if(!IsOpen() || IsError() || IsEof() || IsAbort())
		return -1;
	ReadBuffer();
	return ptr < end ? *ptr++ : -1;
}

int TcpSocket::Peek_()
{
	if(!IsOpen() || IsError() || IsEof() || IsAbort())
		return -1;
	ReadBuffer();
	return ptr < end ? *ptr : -1;
}

int TcpSocket::Get(void *buffer, int count)
{
	LLOG("Get " << count);

	if(!IsOpen() || IsError() || IsEof() || IsAbort())
		return 0;
	
	String out;
	int l = end - ptr;
	done = 0;
	if(l > 0)
		if(l < count) {
			memcpy(buffer, ptr, l);
			done += l;
			ptr = end;
		}
		else {
			memcpy(buffer, ptr, count);
			ptr += count;
			return count;
		}
	while(done < count && !IsError() && !IsEof()) {
		if(!WaitRead())
			break;
		int part = Recv((char *)buffer + done, count - done);
		if(part > 0)
			done += part;
		if(timeout == 0)
			break;
	}
	return done;
}

String TcpSocket::Get(int count)
{
	if(count == 0)
		return Null;
	StringBuffer out(count);
	int done = Get(out, count);
	if(!done && IsEof())
		return String::GetVoid();
	out.SetLength(done);
	return out;
}

String TcpSocket::GetLine(int maxlen)
{
	String ln;
	for(;;) {
		int c = Peek();
		if(c < 0)
			return String::GetVoid();
		Get();
		if(c == '\n')
			return ln;
		if(c != '\r')
			ln.Cat(c);
	}
}

void TcpSocket::SetSockError(const char *context, int code, const char *errdesc)
{
	errorcode = code;
	errordesc.Clear();
	if(socket != INVALID_SOCKET)
		errordesc << "socket(" << (int)socket << ") / ";
	errordesc << context << ": " << errdesc;
	is_error = true;
	LLOG("TCP ERROR " << errordesc);
}

void TcpSocket::SetSockError(const char *context, const char *errdesc)
{
	SetSockError(context, GetErrorCode(), errdesc);
}

void TcpSocket::SetSockError(const char *context)
{
	SetSockError(context, TcpSocketErrorDesc(GetErrorCode()));
}

TcpSocket::SSL *(*TcpSocket::CreateSSL)(TcpSocket& socket);

bool TcpSocket::StartSSL()
{
	ASSERT(IsOpen());
	if(!CreateSSL) {
		SetSockError("StartSSL", -1, "Missing SSL support (Core/SSL)");
		return false;
	}
	if(!IsOpen()) {
		SetSockError("StartSSL", -1, "Socket is not open");
		return false;
	}
	if(!IsOpen()) {
		SetSockError("StartSSL", -1, "Socket is not connected");
		return false;
	}
	ssl = (*CreateSSL)(*this);
	if(!ssl->Start()) {
		ssl.Clear();
		return false;
	}
	return true;
}

int SocketWaitEvent::Wait(int timeout)
{
	FD_ZERO(read);
	FD_ZERO(write);
	FD_ZERO(exception);
	int maxindex = -1;
	for(int i = 0; i < socket.GetCount(); i++) {
		const Tuple2<int, dword>& s = socket[i];
		if(s.a >= 0) {
			const Tuple2<int, dword>& s = socket[i];
			if(s.b & WAIT_READ)
				FD_SET(s.a, read);
			if(s.b & WAIT_WRITE)
				FD_SET(s.a, write);
			if(s.b & WAIT_EXCEPTION)
				FD_SET(s.a, exception);
			maxindex = max(s.a, maxindex);
		}
	}
	timeval *tvalp = NULL;
	timeval tval;
	if(!IsNull(timeout)) {
		tval.tv_sec = timeout / 1000;
		tval.tv_usec = 1000 * (timeout % 1000);
		tvalp = &tval;
	}
	return select(maxindex + 1, read, write, exception, tvalp);
}

dword SocketWaitEvent::Get(int i) const
{
	int s = socket[i].a;
	if(s < 0)
		return 0;
	dword events = 0;
	if(FD_ISSET(s, read))
		events |= WAIT_READ;
	if(FD_ISSET(s, write))
		events |= WAIT_WRITE;
	if(FD_ISSET(s, exception))
		events |= WAIT_EXCEPTION;
	return events;
}

SocketWaitEvent::SocketWaitEvent()
{
	FD_ZERO(read);
	FD_ZERO(write);
	FD_ZERO(exception);
}

END_UPP_NAMESPACE
