#include "CtrlCore.h"

#ifdef PLATFORM_WIN32
#ifndef PLATFORM_WINCE

void DHCtrl::NcCreate(HWND _hwnd)
{
	hwnd = _hwnd;
}

void DHCtrl::NcDestroy()
{
	hwnd = NULL;
}

LRESULT DHCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void DHCtrl::CloseHWND()
{
	if(hwnd) {
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
}

void DHCtrl::OpenHWND()
{
	CloseHWND();
	HWND phwnd = GetTopCtrl()->GetHWND();
	if(phwnd) {
		CreateWindowEx(0, "UPP-CLASS-A", "",
		               WS_CHILD|WS_DISABLED|WS_VISIBLE,
		               0, 0, 20, 20,
		               phwnd, (HMENU)this, hInstance, this);
	}
}

void DHCtrl::SyncHWND()
{
	HWND phwnd = GetTopCtrl()->GetHWND();
	if(phwnd) {
		Rect r = GetScreenView();
		Rect pr = GetScreenClient(phwnd);
		SetWindowPos(hwnd, NULL, r.left - pr.left, r.top - pr.top, r.Width(), r.Height(),
		             SWP_NOACTIVATE|SWP_NOZORDER);
	}
}

void DHCtrl::State(int reason)
{
	switch(reason) {
	case OPEN:
		OpenHWND();
	default:
		SyncHWND();
		break;
	case CLOSE:
		hwnd = NULL;
	}
}

DHCtrl::DHCtrl()
{
	hwnd = NULL;
}

DHCtrl::~DHCtrl()
{
	CloseHWND();
	BackPaint(EXCLUDEPAINT);
}

#endif
#endif
