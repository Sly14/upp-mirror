#include "CtrlLib.h"

NAMESPACE_UPP

void PopUpTable::CancelMode() {
	if(open) {
		DoClose();
		WhenCancel();
	}
	ArrayCtrl::CancelMode();
}

void PopUpTable::DoClose() {
	open = false;
	Close();
}

void PopUpTable::Deactivate() {
	if(open) {
		DoClose();
		IgnoreMouseClick();
		WhenCancel();
	}
}

void PopUpTable::LeftUp(Point p, dword keyflags) {
	ArrayCtrl::LeftUp(p, keyflags);
	DoClose();
	WhenSelect();
}

bool PopUpTable::Key(dword key, int n) {
	switch(key) {
	case K_ENTER:
	case K_ALT_DOWN:
		DoClose();
		WhenSelect();
		return true;
	case K_ESCAPE:
		DoClose();
		WhenCancel();
		return true;
	case K_UP:
		if(!IsCursor()) {
			SetCursor(GetCount() - 1);
			return true;
		}
		break;
	case K_DOWN:
		if(!IsCursor()) {
			SetCursor(0);
			return true;
		}
		break;
	}
	return ArrayCtrl::Key(key, n);
}

void PopUpTable::PopUp(Ctrl *owner, int x, int top, int bottom, int width) {
	DoClose();
	int h = AddFrameSize(width, min(droplines, GetCount()) * GetLineCy()).cy;
	Rect rt = RectC(x, bottom, width, h);
	Rect area = Ctrl::GetWorkArea();
	bool up = false;
	if(rt.bottom > area.bottom) {
		up = true;
		rt.top = top - h;
		rt.bottom = rt.top + h;
	}
	open = false;
	Ctrl popup;
	if(GUI_PopUpEffect()) {
		if(up) {
			popup.SetRect(Rect(rt.left, rt.bottom - 1, rt.right, rt.bottom));
			popup.Add(TopPos(0, rt.Height()).LeftPos(0, rt.Width()));
		}
		else {
			popup.SetRect(Rect(rt.left, rt.top, rt.right, rt.top + 1));
			popup.Add(BottomPos(0, rt.Height()).LeftPos(0, rt.Width()));
		}
		CenterCursor();
		popup.PopUp(owner, true, true, GUI_DropShadows());
		SetFocus();
		Ctrl::ProcessEvents();
		Animate(popup, rt, GUIEFFECT_SLIDE);
		Ctrl::Remove();
	}
	CenterCursor();
	SetRect(rt);
	Ctrl::PopUp(owner, true, true, GUI_DropShadows());
	SetFocus();
	open = true;
}

void PopUpTable::PopUp(Ctrl *owner, int width)
{
	Rect r = owner->GetScreenRect();
	r.right = r.left + width;
	PopUp(owner, r.left, r.top, r.bottom, width);
}

void PopUpTable::PopUp(Ctrl *owner)
{
	Rect r = owner->GetScreenRect();
	PopUp(owner, r.left, r.top, r.bottom, r.Width());
}

void PopUpTable::Normal()
{
	AddColumn().Accel();
	NoHeader();
	HeaderTab(0).SetMargin(0);
	MouseMoveCursor();
	NoGrid();
	AutoHideSb();
	SetLineCy(Draw::GetStdFontCy());
	NoPopUpEx();
}

PopUpTable::PopUpTable() {
	SetFrame(BlackFrame());
	droplines = 16;
	open = false;
}

PopUpTable::~PopUpTable() {}

END_UPP_NAMESPACE
