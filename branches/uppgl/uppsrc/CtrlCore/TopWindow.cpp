#include "CtrlCore.h"

NAMESPACE_UPP

#define LLOG(x)   // LOG(x)

Rect TopWindow::windowFrameMargin;

String TopWindow::GetDesc() const
{
	return title.ToString();
}

Size TopWindow::GetMinSize() const
{
	return minsize;
}

Size TopWindow::GetStdSize() const
{
	return GetMinSize();
}

void TopWindow::ActiveFocus0(Ctrl& ctrl)
{
	if(IsChild()) return;
	activefocus = &ctrl;
	if(IsForeground()) ctrl.SetWantFocus();
}

void TopWindow::Activate()
{
	LLOG("Activate " << Name() << " activefocus = " << UPP::Name(activefocus));
	UsrLogT(3, "ACTIVATE " + Desc(this));
	if(activefocus && (HasFocus() || !GetFocusChildDeep()) && IsEnabled())
		activefocus->SetWantFocus();
	if(urgent)
		SyncCaption();
	LLOG("Activate End");
}

void TopWindow::ChildGotFocus()
{
	activefocus = GetFocusCtrl();
}

void TopWindow::Deactivate()
{
	LLOG("DeActivate current focus " << UPP::Name(GetFocusCtrl()));
	if(HasFocusDeep())
		activefocus = GetFocusCtrl();
	UsrLogT(3, "DEACTIVATE " + Desc(this));
	LLOG("DeActivate " << Name() << " activefocus = " << UPP::Name(activefocus));
}

void TopWindow::PlaceFocus()
{
	if(activefocus)
		activefocus->SetFocus();
	else
		IterateFocusForward(this, this, true, true);
}

bool TopWindow::IsShowEnabled() const
{
	return true;
}

void TopWindow::SyncCaption()
{
	ICall(THISBACK(SyncCaption0));
}

void TopWindow::SyncTitle()
{
	ICall(THISBACK(SyncTitle0));
}

void TopWindow::Close()
{
	if(InLoop()) {
		if(!InCurrentLoop()) return;
		if(FindAction(IDCANCEL))
			RejectBreak(IDCANCEL);
		else
		if(FindAction(IDEXIT))
			AcceptBreak(IDEXIT);
		else
		if(FindAction(IDYES))
			AcceptBreak(IDYES);
		else
			AcceptBreak(IDOK);
		return;
	}
	KillTimeCallback(1);
	backup.Clear();
	if(IsOpen()) IgnoreMouseUp();
	Ctrl::Close();
}

void TopWindow::Backup()
{
	StringStream s;
	Ctrl::Serialize(s);
	backup = s;
}

void TopWindow::Restore()
{
	StringStream s(backup);
	Ctrl::Serialize(s);
}

bool TopWindow::Accept()
{
	Ctrl *q;
	for(q = GetFirstChild(); q; q = q->GetNext())
		if(!q->Accept())
			return false;
	return true;
}

void TopWindow::Reject()
{
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		q->Reject();
	if(!backup.IsEmpty())
		Restore();
}

void TopWindow::Break(int ID)
{
	EndLoop(ID);
}

bool TopWindow::AcceptBreak(int ID)
{
	if(Accept()) {
		Break(ID);
		return true;
	}
	return false;
}

void TopWindow::RejectBreak(int ID)
{
	Reject();
	Break(ID);
}

void TopWindow::SetupRect()
{
	Rect r = GetRect();
	if(r.IsEmpty())
	   SetRect(GetDefaultWindowRect());
	else
	if(r.left == 0 && r.top == 0 && center == 1) {
		Rect area = Ctrl::GetWorkArea();
		SetRect(area.CenterRect(min(area.Size(), r.Size())));
	}
}

void TopWindow::FixIcons()
{
	TopWindow *q = GetMainWindow();
	if(q) {
		if(IsNull(icon)) {
			icon = q->GetIcon();
			SyncCaption();
		}
		if(IsNull(largeicon)) {
			largeicon = q->GetIcon();
			SyncCaption();
		}
	}
}

TopWindow::Abreak *TopWindow::FindAction(int ID)
{
	for(int i = 0; i < action.GetCount(); i++)
		if(action[i].ID == ID) return &action[i];
	return NULL;
}

TopWindow::Abreak *TopWindow::FindAddAction(int ID)
{
	Abreak *x = FindAction(ID);
	if(x) return x;
	Abreak& a = action.Add();
	a.ID = ID;
	a.dlg = this;
	return &a;
}

Callback TopWindow::Breaker(int ID)
{
	return callback(FindAddAction(ID), &Abreak::Break);
}

Callback TopWindow::Acceptor(int ID)
{
	return callback(FindAddAction(ID), &Abreak::Accept);
}

Callback TopWindow::Rejector(int ID)
{
	return callback(FindAddAction(ID), &Abreak::Reject);
}

TopWindow& TopWindow::Breaker(Ctrl& m, int ID)
{
	m.WhenAction = Breaker(ID);
	return *this;
}

TopWindow& TopWindow::Acceptor(Ctrl& m, int ID)
{
	m.WhenAction = Acceptor(ID);
	return *this;
}

TopWindow& TopWindow::Rejector(Ctrl& m, int ID)
{
	m.WhenAction = Rejector(ID);
	return *this;
}

void TopWindow::Paint(Draw& w)
{
	background.Paint(w, Rect(GetSize()), SColorText, SColorShadow);
}

TopWindow& TopWindow::Background(const PaintRect& prect)
{
	background = prect;
	Refresh();
	return *this;
}

bool TopWindow::Key(dword key, int count)
{
	if(Ctrl::Key(key, count))
		return true;
	if(IsChild()) return false;
	if(key == K_DOWN || key == K_RIGHT || key == K_TAB) {
		Ctrl *ctrl = GetFocusChildDeep();
		if(ctrl && IterateFocusForward(ctrl, this))
			return true;
		ctrl = GetFirstChild();
		if(ctrl) {
			if(ctrl->SetWantFocus()) return true;
			return IterateFocusForward(ctrl, this);
		}
	}
	if(key == K_UP || key == K_LEFT || key == K_SHIFT_TAB) {
		Ctrl *ctrl = GetFocusChildDeep();
		if(ctrl && IterateFocusBackward(ctrl, this))
			return true;
		ctrl = GetLastChild();
		if(ctrl) {
			if(ctrl->SetWantFocus()) return true;
			return IterateFocusBackward(ctrl, this);
		}
	}
	return false;
}

void TopWindow::WorkAreaTrim()
{
	Rect a = GetWorkArea();
	Rect h = GetRect();
	h.left = max(h.left, a.left);
	h.right = min(h.right, a.right);
	h.top = max(h.top, a.top);
	h.bottom = min(h.bottom, a.bottom);
	if(h != GetRect() && !IsChild())
		SetRect(h);
}


void GatherWindowTree(Ctrl *w, const Vector<Ctrl *>& ws, Vector<Ctrl *>& es)
{
	if(!w->InLoop())
		es.Add(w);
	for(int i = 0; i < ws.GetCount(); i++)
		if(ws[i]->GetOwner() == w)
			GatherWindowTree(ws[i], ws, es);
}

int  TopWindow::Run(bool appmodal)
{
	GuiLock __;
	LLOG("TopWindow::Run() <- " << typeid(*this).name());
	LLOG("Focus = " << UPP::Name(GetFocusCtrl()));
	if(!IsOpen())
		Open();
	if(!IsVisible()) Show();
	bool pinloop = inloop;
	int  pexitcode = exitcode;
	exitcode = Null;
	Vector<Ctrl *> es;
	if(appmodal)
		es = GetTopCtrls();
	else {
		Vector<Ctrl *> ws = GetTopCtrls();
		for(int i = 0; i < ws.GetCount(); i++)
			if(ws[i]->InLoop())
				es.Add(ws[i]);
		Ctrl *mw = GetMainWindow();
		if(mw) GatherWindowTree(mw, ws, es);
	}
	Vector< Ptr<Ctrl> > disabled = DisableCtrls(es, this);
#ifdef _DEBUG
	for(int d = 0; d < disabled.GetCount(); d++)
		LLOG("DisableCtrls[" << d << "] = " << UPP::Name(disabled[d]));
	LLOG("Running EventLoop in " << UPP::Name(this));
#endif
	EventLoop(this);
#ifdef _DEBUG
	LLOG("Finished EventLoop in " << UPP::Name(this));
	for(int e = 0; e < disabled.GetCount(); e++)
		LLOG("EnableCtrls[" << e << "] = " << UPP::Name(disabled[e]));
#endif
	EnableCtrls(disabled);
	int q = exitcode;
	inloop = pinloop;
	exitcode = pexitcode;
	LLOG("TopWindow::Run() = " << q << " -> " << typeid(*this).name());
#ifdef PLATFORM_WIN32
	LLOG("Focus = " << UPP::Name(GetFocusCtrl()) << ", raw " << (void *)::GetFocus());
#endif
	return q;
}

int  TopWindow::Execute()
{
	int m = Run();
	Close();
	return m;
}

TopWindow& TopWindow::Title(const WString& _title)
{
	if(title != _title) {
		title = _title;
		SyncTitle();
	}
	return *this;
}

TopWindow& TopWindow::Title(const char *s)
{
	return Title(WString(s));
}

TopWindow& TopWindow::Sizeable(bool b)
{
	sizeable = b;
	SyncCaption();
	SyncSizeHints();
	return *this;
}

TopWindow& TopWindow::MinimizeBox(bool b)
{
	minimizebox = b;
	SyncCaption();
	SyncSizeHints();
	return *this;
}

TopWindow& TopWindow::MaximizeBox(bool b)
{
	maximizebox = b;
	SyncCaption();
	SyncSizeHints();
	return *this;
}

TopWindow& TopWindow::Icon(const Image& m)
{
	if(!icon.IsSame(m)) {
		icon = m;
		SyncCaption();
	}
	return *this;
}

TopWindow& TopWindow::LargeIcon(const Image& m)
{
	if(!largeicon.IsSame(m)) {
		largeicon = m;
		SyncCaption();
	}
	return *this;
}

TopWindow& TopWindow::Icon(const Image& smallicon, const Image& _largeicon)
{
	if(!icon.IsSame(smallicon) || !largeicon.IsSame(_largeicon)) {
		icon = smallicon;
		largeicon = _largeicon;
		SyncCaption();
	}
	SyncCaption();
	return *this;
}

TopWindow& TopWindow::ToolWindow(bool b)
{
	tool = b;
	SyncCaption();
	return *this;
}

TopWindow& TopWindow::Urgent(bool b)
{
	GuiLock __;
	urgent = b;
	SyncCaption();
	return *this;
}

void TopWindow::SerializePlacement(Stream& s, bool reminimize)
{
	GuiLock __;
#ifndef PLATFORM_WINCE
	int version = 0;
	s / version;
	Rect rect = GetRect();
	s % overlapped % rect;
	bool mn = state == MINIMIZED;
	bool mx = state == MAXIMIZED;
	s.Pack(mn, mx);
	LLOG("TopWindow::SerializePlacement / " << (s.IsStoring() ? "write" : "read"));
	LLOG("minimized = " << mn << ", maximized = " << mx);
	LLOG("rect = " << rect << ", overlapped = " << overlapped);
	if(s.IsLoading()) {
		if(mn) rect = overlapped;
		Rect limit = GetWorkArea();
#ifdef PLATFORM_WIN32
		Rect outer = rect;
		::AdjustWindowRect(outer, WS_OVERLAPPEDWINDOW, FALSE);
		limit.left   += rect.left   - outer.left;
		limit.top    += rect.top    - outer.top;
		limit.right  += rect.right  - outer.right;
		limit.bottom += rect.bottom - outer.bottom;
#endif
#ifdef PLATFORM_X11
		Rect fm = windowFrameMargin;
		if((fm.left|fm.right|fm.top|fm.bottom) == 0)
			fm = Rect(8, 32, 8, 8);
		limit.left += fm.left;
		limit.right -= fm.right;
		limit.top += fm.top;
		limit.bottom -= fm.bottom;
#endif
		Size sz = min(rect.Size(), limit.Size());
		rect = RectC(
			minmax(rect.left, limit.left, limit.right - sz.cx),
			minmax(rect.top,  limit.top,  limit.bottom - sz.cy),
			sz.cx, sz.cy);
		state = OVERLAPPED;
		if(mn && reminimize)
			state = MINIMIZED;
		if(mx)
			state = MAXIMIZED;
#ifdef PLATFORM_WIN32
		if(state == OVERLAPPED)
#endif
			SetRect(rect);
		if(IsOpen()) {
	#ifdef PLATFORM_WIN32
			HWND hwnd = GetHWND();
			switch(state) {
			case MINIMIZED:
				if(!IsIconic(hwnd))
					::ShowWindow(hwnd, SW_MINIMIZE);
				break;
			case MAXIMIZED:
				if(!IsZoomed(hwnd))
					::ShowWindow(hwnd, SW_MAXIMIZE);
				break;
			default:
				if(IsIconic(hwnd) || IsZoomed(hwnd))
					::ShowWindow(hwnd, SW_RESTORE);
				break;
			}
	#endif
	#ifdef PLATFORM_X11
			if(state == MINIMIZED)
				Minimize(false);
			if(state == MAXIMIZED)
				Maximize(false);
	#endif
		}
	}
#endif
}

#ifdef flagOPENGL

ValueSlider::ValueSlider()
{
	pos = 0;
	shaded = true;
	immediate = false;
	src = Color(255, 255, 0);
	dst = Color(255, 153, 51);
}

void ValueSlider::Paint(Draw &w)
{
	Size sz = GetSize();

	w.DrawRect(0, 0, sz.cx, 1, Black);
	w.DrawRect(0, sz.cy - 1, sz.cx, 1, Black);
	w.DrawRect(0, 0, 1, sz.cy, Black);
	w.DrawRect(sz.cx - 1, 0, 1, sz.cy, Black);
		
	int t = (int) ((pos * sz.cx) / (maxValue - minValue));
	if(t < 1) t = 1;
	if(t > sz.cx - 1) t = sz.cx - 1;
	
	if(shaded)
	{
		for(int i = 1; i < t; i++)
			w.DrawRect(i, 1, 1, sz.cy - 2, Blend(src, dst, 256 * i / (sz.cx - 1)));
	}
	else
	{
		w.DrawRect(Rect(1, 1, t, sz.cy - 1), dst);
	}

	if(t < sz.cx - 1)
		w.DrawRect(Rect(t, 1, sz.cx - 1, sz.cy - 1), Color(245, 245, 255));

	String s = Format("%s : %.2f", text, pos);
	Size tsz = GetTextSize(s, StdFont());
	w.DrawText((sz.cx - tsz.cx) / 2, (sz.cy - tsz.cy) / 2, s);
}

void ValueSlider::LeftDown(Point p, dword keyflags)
{
	pos = minValue + (p.x * maxValue) / (float) GetSize().cx;
	Refresh();
	SetCapture();
	if(immediate)
		WhenAction();
}

void ValueSlider::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	if(!immediate)
		WhenAction();
}

void ValueSlider::MouseMove(Point p, dword keyflags)
{
	if(HasCapture())
	{
		pos = minValue + (p.x * maxValue) / (float) GetSize().cx;
		if(pos > maxValue) pos = maxValue;
		if(pos < minValue) pos = minValue;
		Refresh();
		if(immediate)
			WhenAction();
	}	
}

void ValueSlider::SetPos(float p, float minv, float maxv)
{
	pos = p;
	minValue = minv;
	maxValue = maxv;
	Refresh();
}

float ValueSlider::GetPos()
{
	return pos;
}

InfoPanel::InfoPanel()
{
	Add(alphaSlider.RightPos(110, 100).BottomPos(2, 24));
	Add(angleSlider.RightPos(5, 100).BottomPos(2, 24));
	
	alphaSlider.shaded = true;
	angleSlider.shaded = true;
	alphaSlider.immediate = true;
	angleSlider.immediate = true;
}

void InfoPanel::Paint(Draw& w)
{
	Size sz = GetSize();
	Size wsz = GetTopWindow()->GetSize();
	w.DrawRect(sz, Color(74, 125, 153));
	w.DrawRect(0, 0, sz.cx, 1, Black);
	w.DrawRect(sz.cx - 1, 0, 1, sz.cy, Black);
	String info = Format("FPS: %.2f, Textures: %d, Size: %d, %d", GetFps(), Resources::textures.GetCount(), wsz.cx, wsz.cy);
	w.DrawText(5, sz.cy - 22, info, StdFont(), White);
}

void TopWindow::InitInfoPanel()
{
	static bool init = true;
	
	if(!init)
		return;
	init = false;
	
	Size sz = rect.GetSize();
	Add(infoPanel.LeftPos(0, 470).BottomPos(0, 30));
	
	infoPanel.alphaSlider.SetPos(alpha, 0.f, 255.f);
	infoPanel.alphaSlider.text = "Alpha";
	infoPanel.alphaSlider.WhenAction = THISBACK(SetAlpha);

	infoPanel.angleSlider.SetPos(angle, 0.f, 360.f);
	infoPanel.angleSlider.text = "Angle";
	infoPanel.angleSlider.WhenAction = THISBACK(SetAngle);
}

void TopWindow::SetAlpha()
{
	alpha = infoPanel.alphaSlider.GetPos();
}

void TopWindow::SetAngle()
{
	angle = infoPanel.angleSlider.GetPos();
}
#endif

struct DialogBackground : public Display {
	void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
	{
		w.DrawRect(r, SColorFace());
	}
};

TopWindow::TopWindow()
{
	GuiLock __;
	TransparentBackPaint();
	background = PaintRect(Single<DialogBackground>(), Null);
	center = 1;
	minsize = Size(80, 20);

#ifdef PLATFORM_WIN32
	style = 0;
	exstyle = 0;
	ico = lico = NULL;
#endif
#ifdef PLATFORM_X11
	size_hints = XAllocSizeHints();
	wm_hints = XAllocWMHints();
	class_hint = XAllocClassHint();
	topmost = false;
#endif
	maximizebox = minimizebox = sizeable = tool = noclosebox = false;
	state = OVERLAPPED;
	WhenClose = THISBACK(Close);
	overlapped.Clear();
	dokeys = true;
	fullscreen = frameless = urgent = false;
#ifdef flagOPENGL
	alpha = 255;
	angle = 0.f;
#endif
}

TopWindow::~TopWindow()
{
	GuiLock __;
	if(InLoop())
		EndLoop(IDOK);
	if(!IsChild())
		Close();
#ifdef PLATFORM_WIN32
	DeleteIco();
#endif
#ifdef PLATFORM_X11
	XFree(size_hints);
	XFree(wm_hints);
	XFree(class_hint);
#endif
}

void Maxisize(TopWindow& win, int screencxmax)
{
	if(win.GetWorkArea().Width() <= screencxmax)
		win.Maximize();
}

CH_INT(SwapOKCancel, 0);

void ArrangeOKCancel(Ctrl& ok, Ctrl& cancel)
{
	if(SwapOKCancel() &&
	   ok.GetPos().x.GetB() == cancel.GetPos().x.GetB() &&
	   ok.GetPos().y.GetB() == cancel.GetPos().y.GetB()) {
	       Ctrl::LogPos h = ok.GetPos();
	       ok.SetPos(cancel.GetPos());
	       cancel.SetPos(h);
	}   
}

END_UPP_NAMESPACE
