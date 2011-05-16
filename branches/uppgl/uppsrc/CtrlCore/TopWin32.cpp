#include "CtrlCore.h"

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

#if defined(COMPILER_MINGW) && !defined(FLASHW_ALL)
	// MINGW headers don't include this in (some versions of) windows
	extern "C"{
		struct FLASHWINFO {
			UINT  cbSize;
			HWND  hwnd;
			DWORD dwFlags;
			UINT  uCount;
			DWORD dwTimeout;
		};
		WINUSERAPI BOOL WINAPI FlashWindowEx(FLASHWINFO*);
	}
	#define FLASHW_STOP         0
	#define FLASHW_CAPTION      0x00000001
	#define FLASHW_TRAY         0x00000002
	#define FLASHW_ALL          (FLASHW_CAPTION | FLASHW_TRAY)
	#define FLASHW_TIMER        0x00000004
	#define FLASHW_TIMERNOFG    0x0000000C
#endif

#ifdef PLATFORM_WIN32

void    TopWindow::SyncSizeHints() {}

LRESULT TopWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	GuiLock __;
	HWND hwnd = GetHWND();
#ifndef PLATFORM_WINCE
	bool inloop;
#endif
	switch(message) {
#ifndef PLATFORM_WINCE
	case WM_QUERYENDSESSION:
		inloop = InLoop();
		WhenClose();
		return inloop ? !InLoop() : !IsOpen();
	case WM_ENDSESSION:
		EndSession() = true;
		PostQuitMessage(0);
		return 0;
#endif
	case WM_CLOSE:
		if(IsEnabled()) {
			IgnoreMouseUp();
			WhenClose();
		}
		return 0;
	case WM_WINDOWPOSCHANGED:
#ifndef PLATFORM_WINCE
		if(IsIconic(hwnd))
			state = MINIMIZED;
		else
		if(IsZoomed(hwnd))
			state = MAXIMIZED;
		else
#endif
		{
			state = OVERLAPPED;
			overlapped = GetScreenClient(hwnd);
		}
		Layout();
		break;
	}
	return Ctrl::WindowProc(message, wParam, lParam);
}

void TopWindow::SyncTitle0()
{
	GuiLock __;
	HWND hwnd = GetHWND();
#ifndef PLATFORM_WINCE
	if(hwnd)
		if(IsWindowUnicode(hwnd))
			::SetWindowTextW(hwnd, (const WCHAR*)~title);
		else
#endif
			::SetWindowText(hwnd, ToSystemCharset(title.ToString()));
}

void TopWindow::DeleteIco0()
{
	GuiLock __;
	if(ico)
		DestroyIcon(ico);
	if(lico)
		DestroyIcon(lico);
	ico = lico = NULL;
}

void TopWindow::DeleteIco()
{
	ICall(THISBACK(DeleteIco0));
}

void TopWindow::SyncCaption0()
{
	GuiLock __;
	LLOG("SyncCaption");
	if(fullscreen)
		return;
	HWND hwnd = GetHWND();
	if(hwnd) {
		style = ::GetWindowLong(hwnd, GWL_STYLE);
		exstyle = ::GetWindowLong(hwnd, GWL_EXSTYLE);
	}
	style &= ~(WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU|WS_POPUP|WS_DLGFRAME);
	exstyle &= ~(WS_EX_TOOLWINDOW|WS_EX_DLGMODALFRAME);
	style |= WS_CAPTION;

#ifndef flagOPENGL
	if(hasdhctrl)
#endif
		style |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	if(minimizebox)
		style |= WS_MINIMIZEBOX;
	if(maximizebox)
		style |= WS_MAXIMIZEBOX;
	if(sizeable)
		style |= WS_THICKFRAME;
#ifndef PLATFORM_WINCE
	if(frameless)
		style = (style & ~WS_CAPTION) | WS_POPUP;
	else
	if(IsNull(icon) && !maximizebox && !minimizebox || noclosebox) {
		style |= WS_POPUPWINDOW|WS_DLGFRAME;
		exstyle |= WS_EX_DLGMODALFRAME;
		if(noclosebox)
			style &= ~WS_SYSMENU;
	}
	else
#endif
		style |= WS_SYSMENU;
	if(tool)
		exstyle |= WS_EX_TOOLWINDOW;
	if(fullscreen)
		style = WS_POPUP;
	if(hwnd) {
		::SetWindowLong(hwnd, GWL_STYLE, style);
		::SetWindowLong(hwnd, GWL_EXSTYLE, exstyle);
		SyncTitle();
		if(urgent) {
			if(IsForeground()) urgent = false;
			FLASHWINFO fi;
			memset(&fi, 0, sizeof(fi));
			fi.cbSize = sizeof(fi);
			fi.hwnd = hwnd;
			fi.dwFlags = urgent ? FLASHW_TIMER|FLASHW_ALL : FLASHW_STOP;
			FlashWindowEx(&fi);
		}
	}
	DeleteIco();
#ifndef PLATFORM_WINCE //TODO!!!
	if(hwnd) {
		::SendMessage(hwnd, WM_SETICON, false, (LPARAM)(ico = IconWin32(icon)));
		::SendMessage(hwnd, WM_SETICON, true, (LPARAM)(lico = IconWin32(largeicon)));
	}
#endif
}

void TopWindow::CenterRect(HWND hwnd, int center)
{
	GuiLock __;
	SetupRect();
	if(hwnd && center == 1 || center == 2) {
		Size sz = GetRect().Size();
		Rect frmrc(sz);
	#ifndef PLATFORM_WINCE
		::AdjustWindowRect(frmrc, WS_OVERLAPPEDWINDOW, FALSE);
	#endif
		Rect r, wr;
		wr = Ctrl::GetWorkArea().Deflated(-frmrc.left, -frmrc.top,
			frmrc.right - sz.cx, frmrc.bottom - sz.cy);
		sz.cx = min(sz.cx, wr.Width());
		sz.cy = min(sz.cy, wr.Height());
		if(center == 1) {
			::GetClientRect(hwnd, r);
			if(r.IsEmpty())
				r = wr;
			else {
				Point p = r.TopLeft();
				::ClientToScreen(hwnd, p);
				r.Offset(p);
			}
		}
		else
			r = wr;
		Point p = r.CenterPos(sz);
		if(p.x + sz.cx > wr.right) p.x = wr.right - sz.cx;
		if(p.y + sz.cy > wr.bottom) p.y = wr.bottom - sz.cy;
		if(p.x < wr.left) p.x = wr.left;
		if(p.y < wr.top) p.y = wr.top;
		SetRect(p.x, p.y, sz.cx, sz.cy);
	}
}

static HWND trayHWND__;
HWND   GetTrayHWND__()          { return trayHWND__; }
void   SetTrayHWND__(HWND hwnd) { trayHWND__ = hwnd; }

void TopWindow::Open(HWND hwnd)
{
	GuiLock __;
	if(dokeys && (!GUI_AKD_Conservative() || GetAccessKeysDeep() <= 1))
		DistributeAccessKeys();
	UsrLogT(3, "OPEN " + Desc(this));
	LLOG("TopWindow::Open, owner HWND = " << FormatIntHex((int)hwnd, 8) << ", Active = " << FormatIntHex((int)::GetActiveWindow(), 8));
	IgnoreMouseUp();
	SyncCaption();
#ifdef PLATFORM_WINCE
	if(!GetRect().IsEmpty())
#endif
	if(fullscreen) {
		SetRect(GetScreenSize());
		Create(hwnd, WS_POPUP, 0, false, SW_SHOWMAXIMIZED, false);
	}
	else {
		CenterRect(hwnd, hwnd && hwnd == GetTrayHWND__() ? center ? 2 : 0 : center);
		Create(hwnd, style, exstyle, false, state == OVERLAPPED ? SW_SHOWNORMAL :
		                                    state == MINIMIZED  ? SW_MINIMIZE :
		                                                          SW_MAXIMIZE, false);
	}
	
	PlaceFocus();
	SyncCaption();
	FixIcons();
	
#ifdef flagOPENGL
	isdhctrl = true;
	hDC = ::GetDC(GetHWND());
	if(!hDC)
		return;
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_SUPPORT_COMPOSITION;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 1;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pf = ChoosePixelFormat(hDC, &pfd);
	if(!pf) {
		
		RLOG("OpenGL: ChoosePixelFormat error");
		DestroyGL();
		return;
	}
	if(!SetPixelFormat(hDC, pf, &pfd)) {
		RLOG("OpenGL: SetPixelFormat error");
		DestroyGL();
		return;
	}
	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	hRC = wglCreateContext(hDC);
	ActivateGLContext();
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		RLOG("OpenGL: Glew library initialization error: " + String((const char*) glewGetErrorString(err)));
		DestroyGL();
		return;
	}
	SetTimeCallback(-10, THISBACK(Repaint), 1);
#endif	
}

void TopWindow::Repaint()
{
	::InvalidateRect(GetHWND(), NULL, false);
}

void TopWindow::Open(Ctrl *owner)
{
	GuiLock __;
	LLOG("TopWindow::Open(Ctrl) -> " << UPP::Name(owner));
	Open(owner ? owner->GetTopCtrl()->GetHWND() : NULL);
	if(IsOpen() && top)
		top->owner = owner;
}

void TopWindow::Open()
{
	Open(::GetActiveWindow()); // :: needed because of ActiveX controls (to create modal dlgs owned by a HWND)
}

void TopWindow::OpenMain()
{
	Open((HWND) NULL);
}

void TopWindow::Minimize(bool effect)
{
	state = MINIMIZED;
	if(IsOpen())
#ifdef PLATFORM_WINCE
		::ShowWindow(GetHWND(), SW_MINIMIZE);
#else
		::ShowWindow(GetHWND(), effect ? SW_MINIMIZE : SW_SHOWMINIMIZED);
#endif
}

TopWindow& TopWindow::FullScreen(bool b)
{
	fullscreen = b;
	HWND hwnd = GetOwnerHWND();
	bool pinloop = inloop;
	WndDestroy();
	Overlap();
	SetRect(GetDefaultWindowRect());
	Open(hwnd);
	inloop = pinloop;
	return *this;
}

void TopWindow::Maximize(bool effect)
{
	state = MAXIMIZED;
	if(IsOpen()) {
		::ShowWindow(GetHWND(), effect ? SW_MAXIMIZE : SW_SHOWMAXIMIZED);
		SyncCaption();
	}
}

void TopWindow::Overlap(bool effect)
{
	GuiLock __;
	state = OVERLAPPED;
	if(IsOpen()) {
		::ShowWindow(GetHWND(), effect ? SW_SHOWNORMAL : SW_RESTORE);
		SyncCaption();
	}
}

TopWindow& TopWindow::Style(dword _style)
{
	GuiLock __;
	style = _style;
	if(GetHWND())
		::SetWindowLong(GetHWND(), GWL_STYLE, style);
	SyncCaption();
	return *this;
}

TopWindow& TopWindow::ExStyle(dword _exstyle)
{
	GuiLock __;
	exstyle = _exstyle;
	if(GetHWND())
		::SetWindowLong(GetHWND(), GWL_EXSTYLE, exstyle);
	SyncCaption();
	return *this;
}

TopWindow& TopWindow::TopMost(bool b, bool stay_top)
{
	GuiLock __;
	HWND hwnd;
	if(hwnd = GetHWND())
		SetWindowPos(hwnd, b ? HWND_TOPMOST : (stay_top ? HWND_NOTOPMOST : HWND_BOTTOM),
		             0,0,0,0,SWP_NOMOVE|SWP_NOSIZE );
	return ExStyle(b ? GetExStyle() | WS_EX_TOPMOST : GetExStyle() & ~WS_EX_TOPMOST);
}

bool TopWindow::IsTopMost() const
{
	return GetExStyle() & WS_EX_TOPMOST;
}

#ifdef flagOPENGL
void TopWindow::DestroyGL()
{
	if (hDC != NULL && hRC != NULL)
	{
		ActivateGLContext();
		wglMakeCurrent(NULL, NULL);
	}
	
	if(hRC)
	    wglDeleteContext(hRC);
	if(hDC)
	    ReleaseDC(GetHWND(), hDC);
}

void TopWindow::ActivateGLContext()
{
	if (hRC != NULL && wglGetCurrentContext() != hRC)
		wglMakeCurrent(hDC, hRC);
}

#endif

#endif

END_UPP_NAMESPACE
