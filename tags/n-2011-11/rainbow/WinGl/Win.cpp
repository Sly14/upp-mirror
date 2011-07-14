#include <CtrlCore/CtrlCore.h>

#ifdef GUI_WINGL
extern void GuiMainFn_();


NAMESPACE_UPP

HWND   hWnd = NULL;
HWND   glHwnd = NULL;
HDC    hDC = NULL;
HGLRC  hRC = NULL;

bool glEndSession = false;

bool GlEndSession()
{
	return glEndSession;
}

bool GlIsWaitingEvent()
{
	MSG msg;
	return PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
}

bool GlProcessEvent(bool *quit)
{
	MSG msg;
	if(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		if(msg.message == WM_QUIT && quit)
			*quit = true;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		return true;
	}
	return false;
}

void GlSleep(int ms)
{
	MsgWaitForMultipleObjects(0, NULL, FALSE, ms, QS_ALLINPUT);
}

void ActivateGlContext()
{
	if(hRC != NULL && wglGetCurrentContext() != hRC)
		wglMakeCurrent(hDC, hRC);
}

void DestroyGl(bool destroyWindow)
{
	if(hDC != NULL && hRC != NULL)
		wglMakeCurrent(NULL, NULL);
	
	if(hRC) 
	{
		wglDeleteContext(hRC);
		hRC = NULL;
	}
	if(hDC)
	{
		::ReleaseDC(glHwnd, hDC);
		hDC = NULL;
	}
}

int CreateGlWindow(HINSTANCE hInstance)
{
	WNDCLASSEXW  wc;
	Zero(wc);
	wc.cbSize        = sizeof(wc);
	wc.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = (WNDPROC)glWindowProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)NULL;
	wc.lpszClassName = L"UPP-GL-CLASS";
	RegisterClassExW(&wc);
	
	glHwnd = CreateWindowW(
		L"UPP-GL-CLASS", L"",
		WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
	   	CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
	    NULL, NULL, hInstance, NULL);
	    
	if(!glHwnd)
		return -1;
	
	//ShowWindow(glHwnd, 1);
	return 1;
}

int CreateGlContext()
{
	hDC = ::GetDC(glHwnd);
	if(!hDC)
		return -1;
	
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = 
		PFD_DRAW_TO_WINDOW | 
		PFD_DOUBLEBUFFER | 
		PFD_SUPPORT_OPENGL |
		//PFD_SUPPORT_COMPOSITION |
		0x00008000 | 
		PFD_GENERIC_ACCELERATED;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pf = ChoosePixelFormat(hDC, &pfd);
	if(!pf) {
		RLOG("OpenGL: ChoosePixelFormat error");
		return -2;
	}
	RLOG("OpenGL: ChoosePixelFormat ok..");
	if(!SetPixelFormat(hDC, pf, &pfd)) {
		RLOG("OpenGL: SetPixelFormat error");
		return -3;
	}
	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	hRC = wglCreateContext(hDC);
	if(!hRC)
	{
		RLOG("OpenGL: wglCreateContext error");
		return -4;
	}
	RLOG("OpenGL: wglCreateContext ok..");
	if(!wglMakeCurrent(hDC, hRC))
	{
		RLOG("OpenGL: wglMakeCurrent error");
		return -5;
	}
	RLOG("OpenGL: wglMakeCurrent ok..");
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		RLOG("OpenGL: Glew library initialization error: " + String((const char*) glewGetErrorString(err)));
		return -6;
	}
	RLOG("OpenGL: glewInit ok..");
	//InitializeShaders();
	wglSwapIntervalEXT(0);
	//SetTimeCallback(-10, THISBACK(Repaint), 1);
	                       
	SetTimer(glHwnd, 1, 10, NULL);
	RLOG("OpenGL: SetTimer ok..");
	return 1;
}

Vector<WString>& coreCmdLine__();
Vector<WString> SplitCmdLine__(const char *cmd);

int AppMain(HINSTANCE hInstance, LPSTR lpCmdLine)
{
	UPP::coreCmdLine__() = UPP::SplitCmdLine__(UPP::FromSystemCharset(lpCmdLine));
	UPP::AppInitEnvironment__();
//	Ctrl::InitTimer();
	Ctrl::InitGl();
	//int r = UPP::GlInit(hInstance);
	int r = UPP::CreateGlWindow(hInstance);
	if(r < 0)
		::MessageBox(NULL, Format("OpenGL window could not be created: %r (%s)", r, GetLastErrorMessage()), NULL, MB_ICONEXCLAMATION | MB_OK);
	else
		r = UPP::CreateGlContext();
	if(r < 0)
		::MessageBox(NULL, Format("OpenGL context could not be created: %r (%s)", r, GetLastErrorMessage()), NULL, MB_ICONEXCLAMATION | MB_OK);

	if(r > 0) 
	{
		GuiMainFn_();
		UPP::Ctrl::CloseTopCtrls();
		UPP::UsrLog("---------- About to delete this log of WinGL...");
		UPP::DeleteUsrLog();
		UPP::DestroyGl(false);
		return UPP::GetExitCode();
	} else {
		return r;
	}
}

END_UPP_NAMESPACE

#endif