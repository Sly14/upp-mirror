#include "MenuImp.h"

#ifdef PLATFORM_WIN32
#include <mmsystem.h>
#endif

NAMESPACE_UPP

#define LLOG(x)    // LOG(x)
#define LTIMING(x) // RTIMING(x)

MenuItemBase::MenuItemBase()
{
	accel = 0;
	state = 0;
	isenabled = true;
	type = 0;
	font = StdFont();
	leftgap = 16;
	textgap = 6;
	accesskey = 0;
	NoWantFocus();
	style = &MenuBar::StyleDefault();
	Transparent();
	maxiconsize = Size(INT_MAX, INT_MAX);
}

Bar::Item& MenuItemBase::Text(const char *s)
{
	accesskey = ExtractAccessKey(s, text);
	Refresh();
	return *this;
}

Bar::Item& MenuItemBase::Key(dword key)
{
	if(key) {
		accel = key;
		Refresh();
	}
	return *this;
}

Bar::Item& MenuItemBase::Image(const UPP::Image& img)
{
	return *this;
}

Bar::Item& MenuItemBase::Check(bool check)
{
	type = CHECK0 + check;
	return *this;
}

Bar::Item& MenuItemBase::Radio(bool check)
{
	type = RADIO0 + check;
	return *this;
}

Bar::Item& MenuItemBase::Tip(const char *s)
{
	return *this;
}

Bar::Item& MenuItemBase::Help(const char *s)
{
	HelpLine(s);
	return *this;
}

Bar::Item& MenuItemBase::Description(const char *s)
{
	Ctrl::Description(s);
	return *this;
}

Bar::Item& MenuItemBase::Topic(const char *s)
{
	HelpTopic(s);
	return *this;
}

Bar::Item& MenuItemBase::Enable(bool en)
{
	isenabled = en;
	Refresh();
	return *this;
}

String MenuItemBase::GetDesc() const
{
	return text;
}

dword  MenuItemBase::GetAccessKeys() const
{
	return AccessKeyBit(accesskey);
}

void   MenuItemBase::AssignAccessKeys(dword used)
{
	if(!accesskey) {
		accesskey = ChooseAccessKey(text, used);
		used |= AccessKeyBit(accesskey);
	}
	Ctrl::AssignAccessKeys(used);
}

void DrawMnemonicText(Draw& w, int x, int y, const String& s, Font font, Color color,
                      int mnemonic)
{
	int apos = HIWORD(mnemonic);
	int q;
	if(apos && apos < s.GetLength())
		q = apos - 1;
	else {
		q = s.Find(ToUpper(mnemonic));
		if(q < 0)
			q = s.Find(ToLower(mnemonic));
	}
	w.DrawText(x, y, s, font, color);
	if(q < 0) return;
	FontInfo f = font.Info();
	w.DrawRect(x + GetTextSize(~s, font, q).cx, y + f.GetAscent() + 1, f[s[q]], 1,
	           SColorMark());
}

void DrawMenuText(Draw& w, int x, int y, const String& s, Font f, bool enabled,
                  bool hl, int mnemonic, Color color, Color hlcolor)
{
	if(enabled)
		DrawMnemonicText(w, x, y, s, f, hl ? hlcolor : color, mnemonic);
	else {
		if(GUI_GlobalStyle() >= GUISTYLE_XP)
			DrawMnemonicText(w, x, y, s, f, SColorDisabled, 0);
		else {
			DrawMnemonicText(w, x + 1, y + 1, s, f, SColorPaper, 0);
			DrawMnemonicText(w, x, y, s, f, SColorDisabled, 0);
		}
	}
}

void MenuItemBase::DrawMenuText(Draw& w, int x, int y, const String& s, Font f, bool enabled,
                                bool hl, Color color, Color hlcolor)
{
	UPP::DrawMenuText(w, x, y, s, f, enabled, hl, VisibleAccessKeys() ? accesskey : 0,
	                  color, hlcolor);
}

void MenuItemBase::PaintTopItem(Draw& w, int state) {
	Size sz = GetSize();
	if(GUI_GlobalStyle() >= GUISTYLE_XP) {
		ChPaint(w, 0, 0, sz.cx, sz.cy, style->topitem[state]);
		String text = GetText();
		Size isz = GetTextSize(text, StdFont());
		DrawMenuText(w, 6, (sz.cy - isz.cy) / 2, text, GetFont(), IsItemEnabled(), state,
		             style->topitemtext[0], style->topitemtext[state]);
	}
	else {
		w.DrawRect(sz, SColorFace);
		static const ColorF b0[] = { (ColorF)1, SColorLight, SColorLight, SColorShadow, SColorShadow, };
		static const ColorF b1[] = { (ColorF)1, SColorShadow, SColorShadow, SColorLight, SColorLight, };
		String text = GetText();
		Size isz = GetTextSize(text, StdFont());
		DrawMenuText(w, 6, (sz.cy - isz.cy) / 2, text, GetFont(), IsItemEnabled(), false,
		             SColorMenuText, SColorHighlightText);
		if(state)
			DrawBorder(w, 0, 0, sz.cx, sz.cy, state == 2 ? b1 : b0);
	}
}

// -------------------------------------

Bar::Item& MenuItem::Image(const UPP::Image& img)
{
	licon = img;
	Refresh();
	return *this;
}

MenuItem& MenuItem::RightImage(const UPP::Image& img)
{
	ricon = img;
	Refresh();
	return *this;
}

void MenuItem::SendHelpLine()
{
	BarCtrl::SendHelpLine(this);
}

void MenuItem::ClearHelpLine()
{
	BarCtrl::ClearHelpLine(this);
}

void MenuItem::MouseEnter(Point, dword)
{
	SetFocus();
	Refresh();
	SendHelpLine();
}

void MenuItem::MouseLeave()
{
	ClearHelpLine();
}

void MenuItem::GotFocus()
{
	Refresh();
	SendHelpLine();
}

void MenuItem::LostFocus()
{
	Refresh();
	ClearHelpLine();
}

int MenuItem::GetVisualState()
{
	return HasFocus() ?
	       (HasMouse() && GetMouseLeft() || GetMouseRight()) ? PUSH : HIGHLIGHT : NORMAL;
}

void MenuItem::SyncState()
{
	int s = GetVisualState();
	if(s != state) {
		state = s;
		Refresh();
	}
}

void MenuItem::Paint(Draw& w)
{
	int q = text.Find('\t');
	String txt, keydesc;
	if(accel)
		keydesc = GetKeyDesc(accel);
	if(q >= 0) {
		keydesc = text.Mid(q + 1);
		txt = text.Mid(0, q);
	}
	else
		txt = text;
	state = GetVisualState();
	bool hl = state != NORMAL;
	Size sz = GetSize();

	if(hl)
		if(GUI_GlobalStyle() >= GUISTYLE_XP)
			ChPaint(w, 0, 0, sz.cx, sz.cy, style->item);
		else
			w.DrawRect(sz, SColorHighlight);
	UPP::Image li = licon;
	if(li.IsEmpty()) {
		switch(type) {
		case CHECK0: li = CtrlImg::MenuCheck0(); break;
		case CHECK1: li = CtrlImg::MenuCheck1(); break;
		case RADIO0: li = CtrlImg::MenuRadio0(); break;
		case RADIO1: li = CtrlImg::MenuRadio1(); break;
		}
	}
	Size imsz = li.GetSize();
	Size isz = min(maxiconsize, imsz);
	if(isz != imsz)
		li = CachedRescale(li, isz);
	int iy = (sz.cy - isz.cy) / 2;
	bool chk = false;
	int x = (max(isz.cx, leftgap - 2) - isz.cx) / 2;
	if(!licon.IsEmpty() && type) {
		chk = type == CHECK1 || type == RADIO1;
		x = 2;
		if(GUI_GlobalStyle() >= GUISTYLE_XP) {
			if(chk && !hl) {
				DrawXPButton(w, RectC(0, iy - 2, isz.cx + 4, isz.cy + 4), BUTTON_EDGE|BUTTON_CHECKED);
			}
		}
		else {
			w.DrawRect(1, iy - 1, isz.cx + 2, isz.cy + 2, chk ? Blend(SColorFace, SColorLight)
			                                                  : SColorFace);
			DrawBorder(w, 0, iy - 2, isz.cx + 4, isz.cy + 4, chk ? ThinInsetBorder : ThinOutsetBorder);
		}
	}
	if(isenabled)
		DrawHighlightImage(w, x, iy, li, hl || chk, true);
	else
		w.DrawImage(x, iy, DisabledImage(li));
	x = max(isz.cx, leftgap) + textgap;
	isz = GetTextSize(text, StdFont());
	DrawMenuText(w, x, (sz.cy - isz.cy) / 2, txt, font, isenabled, hl, SColorMenuText,
	             style->itemtext);
	isz = ricon.GetSize();
	if(isenabled)
		DrawHighlightImage(w, sz.cx - isz.cx, (sz.cy - isz.cy) / 2, ricon, hl, true);
	else
		w.DrawImage(sz.cx - isz.cx, (sz.cy - isz.cy) / 2, DisabledImage(ricon));
	x = sz.cx - max(isz.cx, 16) - 1;
	if(!IsEmpty(keydesc)) {
		isz = GetTextSize(keydesc, StdFont());
		UPP::DrawMenuText(w, x - isz.cx - 2, (sz.cy - isz.cy) / 2, keydesc, font, isenabled, hl,
		                  0, SColorMark(), style->itemtext);
	}
}

Size MenuItem::GetMinSize() const
{
	Size sz1 = GetTextSize(text, font);
	Size sz2(0, 0);
	if(accel) {
		sz2 = GetTextSize(GetKeyDesc(accel), font);
		sz2.cx += 12;
	}
	Size lsz = min(maxiconsize, licon.GetSize());
	Size rsz = ricon.GetSize();
	return AddFrameSize(Size(max(lsz.cx, leftgap) + sz1.cx + max(sz2.cx, (rsz.cx ? 16 : 0))
	                         + max(rsz.cx, 16) + textgap + 10,
	                         max(max(lsz.cy, rsz.cy) + 4, sz1.cy + 6)));
}

void MenuItem::LeftUp(Point, dword)
{
	if(!isenabled) return;
#ifdef PLATFORM_WIN32
#ifdef PLATFORM_WINCE
	PlaySound(L"MenuCommand", NULL, SND_ASYNC|SND_NODEFAULT); //TODO?
#else
	PlaySound("MenuCommand", NULL, SND_ASYNC|SND_NODEFAULT);
#endif
#endif
	LLOG("Menu Item pre Action");
	WhenAction();
	LLOG("Menu Item post Action");
}

void MenuItem::RightUp(Point p, dword w)
{
	LeftUp(p, w);
}

bool MenuItem::HotKey(dword key)
{
	if(isenabled && (key == accel || CompareAccessKey(accesskey, key)
	|| key < 256 && IsAlNum((char)key) && CompareAccessKey(accesskey, ToUpper((char)key) + K_DELTA + K_ALT))) {
		LLOG("MenuItem::HotKey(" << key << ") -> SetFocus");
		SetFocus();
		Sync();
		Sleep(50);
		WhenAction();
		return true;
	}
	return false;
}

bool MenuItem::Key(dword key, int count)
{
	if(key == K_ENTER && isenabled) {
		WhenAction();
		return true;
	}
	return false;
}

// ----------------------------------------------------

void SubMenuBase::Pull(Ctrl *item, Point p, Size sz)
{
	if(!item->IsOpen() || menu.IsOpen()) return;
	menu.Clear();
	if(parentmenu)
		menu.SetStyle(*parentmenu->style);
	proc(menu);
	if(parentmenu) {
		parentmenu->SetActiveSubmenu(&menu, item);
		menu.SetParentMenu(parentmenu);
	}
	menu.PopUp(parentmenu, p, sz);
	if(parentmenu) {
		parentmenu->doeffect = false;
		parentmenu->WhenSubMenuOpen();
	}
}

// ----------------------------------------------------

SubMenuItem::SubMenuItem()
{
	RightImage(CtrlImg::right_arrow());
}

void SubMenuItem::GotFocus()
{
	Refresh();
}

void SubMenuItem::Pull()
{
	Rect r = GetScreenRect();
	Point p = r.TopRight();
	p.x -= 3;
	p.y -= 2;
	SubMenuBase::Pull(this, p, Size(-r.Width(), 0));
	if(parentmenu)
		parentmenu->SyncState();
}

void SubMenuItem::MouseEnter(Point, dword)
{
	SetFocus();
	Refresh();
	if(!menu.IsOpen() && isenabled)
		SetTimeCallback(400, THISBACK(Pull), TIMEID_PULL);
}

void SubMenuItem::MouseLeave()
{
	if(HasFocus() && GetParent())
		GetParent()->SetFocus();
	KillTimeCallback(TIMEID_PULL);
}

int SubMenuItem::GetVisualState()
{
	if(menu.IsOpen() && !GetParent()->HasFocusDeep())
		return PUSH;
	return MenuItem::GetVisualState();
}

bool SubMenuItem::Key(dword key, int count)
{
	if(key == K_RIGHT || key == K_ENTER) {
		Pull();
		return true;
	}
	return MenuItem::Key(key, count);
}

bool SubMenuItem::HotKey(dword key)
{
	if(isenabled && (key == accel || CompareAccessKey(accesskey, key))) {
		Pull();
		return true;
	}
	return false;
}


// ----------------------------------------------------

Size TopSubMenuItem::GetMinSize() const
{
	return AddFrameSize(GetTextSize(text, font) + Size(12, 7));
}

int  TopSubMenuItem::GetState()
{
	if(parentmenu && parentmenu->GetActiveSubmenu() == &menu) return PUSH;
	if(HasMouse() && GetParent() && !GetParent()->HasFocusDeep() &&
	                 (!parentmenu || !parentmenu->GetActiveSubmenu() || parentmenu->GetActiveSubmenu() == &menu)
	   || HasFocus())
		return HIGHLIGHT;
	return NORMAL;
}

void TopSubMenuItem::Paint(Draw& w)
{
	PaintTopItem(w, state = GetState());
}

void TopSubMenuItem::Pull()
{
	Rect r = GetScreenRect();
	if(parentmenu && parentmenu->IsChild() && !parentmenu->submenu)
		parentmenu->SetupRestoreFocus();
	Point p = r.BottomLeft();
	if(GUI_GlobalStyle() >= GUISTYLE_XP)
		p += style->pullshift;
	SubMenuBase::Pull(this, p, Size(r.Width(), -r.Height()));
	if(parentmenu)
		parentmenu->SyncState();
}

void TopSubMenuItem::MouseEnter(Point p, dword)
{
	Refresh();
	if(isenabled && parentmenu->GetActiveSubmenu())
		Pull();
}

bool TopSubMenuItem::Key(dword key, int) {
	if(isenabled && (key == K_ENTER || key == K_DOWN)) {
		Pull();
		return true;
	}
	return false;
}

void TopSubMenuItem::GotFocus()
{
	Refresh();
}

void TopSubMenuItem::LostFocus()
{
	Refresh();
}

bool TopSubMenuItem::HotKey(dword key)
{
	if(BarCtrl::Scan(proc, key))
		return true;
	if(isenabled && (key == accel || CompareAccessKey(accesskey, key))) {
		Pull();
		return true;
	}
	return false;
}

void TopSubMenuItem::MouseLeave()
{
	Refresh();
}

void TopSubMenuItem::LeftDown(Point, dword)
{
	if(isenabled && !menu.IsOpen()) {
		Pull();
		Refresh();
	}
}

void TopSubMenuItem::SyncState()
{
	int q = GetState();
	if(q != state) {
		state = q;
		Refresh();
	}
}

// ----------------------------------------------------

int  TopMenuItem::GetState()
{
	if(!IsEnabled()) return NORMAL;
	if(HasMouse() && GetMouseLeft() || GetMouseRight()) return PUSH;
	if(HasFocus() || HasMouse()) return HIGHLIGHT;
	return NORMAL;
}

void TopMenuItem::Paint(Draw& w)
{
	PaintTopItem(w, state = GetState());
}

void TopMenuItem::MouseEnter(Point, dword)
{
	Refresh();
}

void TopMenuItem::MouseLeave()
{
	Refresh();
}

void TopMenuItem::LeftUp(Point, dword)
{
	if(!isenabled) return;
	WhenAction();
	Refresh();
}

void TopMenuItem::LeftDown(Point, dword)
{
	Refresh();
}

void TopMenuItem::GotFocus()
{
	Refresh();
}

void TopMenuItem::LostFocus()
{
	Refresh();
}

bool TopMenuItem::Key(dword key, int count)
{
	if(isenabled && key == K_ENTER) {
		WhenAction();
		return true;
	}
	return false;
}

Size TopMenuItem::GetMinSize() const
{
	return AddFrameSize(GetTextSize(text, StdFont()) + Size(12, 7));
}

int TopMenuItem::GetStdHeight(Font font)
{
	return font.Info().GetHeight() + 7;
}

void TopMenuItem::SyncState()
{
	if(state != GetState()) {
		state = GetState();
		Refresh();
	}
}

END_UPP_NAMESPACE
