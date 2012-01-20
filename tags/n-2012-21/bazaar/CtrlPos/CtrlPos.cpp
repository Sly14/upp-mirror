#include "CtrlPos.h"

void CtrlPos::DrawAlignHandle(Draw& w, const Rect& _r, const Rect& r, const Ctrl::LogPos& pos, const Color& col)
{
	Point p;
	bool q;
	Size rz = r.GetSize();
	Color col2 = Blend(col, Black);

	p = r.CenterLeft();
	p.y -= rz.cy/4;
	q = (pos.x.GetAlign()&Ctrl::LEFT);
	w.DrawLine(p, Point(_r.left, p.y), (q)?(PEN_SOLID):(PEN_DOT), (q)?(col):(col2));

	p = r.CenterRight();
	p.y += rz.cy/4;
	q = (pos.x.GetAlign()&Ctrl::RIGHT);
	w.DrawLine(p, Point(_r.right, p.y), (q)?(PEN_SOLID):(PEN_DOT), (q)?(col):(col2));

	p = r.TopCenter();
	p.x -= rz.cx/4;
	q = (pos.y.GetAlign()&Ctrl::TOP);
	w.DrawLine(p, Point(p.x, _r.top), (q)?(PEN_SOLID):(PEN_DOT), (q)?(col):(col2));

	p = r.BottomCenter();
	p.x += rz.cx/4;
	q = (pos.y.GetAlign()&Ctrl::BOTTOM);
	w.DrawLine(p, Point(p.x, _r.bottom), (q)?(PEN_SOLID):(PEN_DOT), (q)?(col):(col2));
}

bool CtrlPos::GetAlignMode(const Rect& _r, const Rect& r, const Point& pp, Ctrl::LogPos& pos, int handsize)
{
	int size = handsize+2;
	Rect c;
	Point p;
	int q;
	Size rz = r.GetSize();
	
	q = pos.x.GetAlign();
	
	p = r.CenterLeft();
	p.y -= rz.cy/4;
	c = Rect(p, Point(_r.left, p.y)); c.InflateVert(size); c.Normalize();
	if(c.Contains(pp)) { q&= ~Ctrl::LEFT; q|= (pos.x.GetAlign()&Ctrl::LEFT)?(0):(Ctrl::LEFT); pos.x.SetAlign(q); return true; }

	p = r.CenterRight();
	p.y += rz.cy/4;
	c = Rect(p, Point(_r.right, p.y)); c.InflateVert(size); c.Normalize();
	if(c.Contains(pp)) { q&= ~Ctrl::RIGHT; q|= (pos.x.GetAlign()&Ctrl::RIGHT)?(0):(Ctrl::RIGHT); pos.x.SetAlign(q); return true; }

	q = pos.y.GetAlign();
	
	p = r.TopCenter();
	p.x -= rz.cx/4;
	c = Rect(p, Point(p.x, _r.top)); c.InflateHorz(size); c.Normalize();
	if(c.Contains(pp)) { q&= ~Ctrl::TOP; q|= (pos.y.GetAlign()&Ctrl::TOP)?(0):(Ctrl::TOP); pos.y.SetAlign(q); return true; }

	p = r.BottomCenter();
	p.x += rz.cx/4;
	c = Rect(p, Point(p.x, _r.bottom)); c.InflateHorz(size); c.Normalize();
	if(c.Contains(pp)) { q&= ~Ctrl::BOTTOM; q|= (pos.y.GetAlign()&Ctrl::BOTTOM)?(0):(Ctrl::BOTTOM); pos.y.SetAlign(q); return true; }
	
	return false;
}

void CtrlPos::DrawHintFrame(Draw& w, const Ctrl& g, const Ctrl& q, const Color& hintcol)
{
	Ctrl* c = q.GetFirstChild();
	while(c)
	{
		Ctrl* e(c);
		Point p; p.Clear();
		int f(flags);
		filter(e, p, f);
		if(e && c->InView())
		{
			Rect r = c->GetRect();
			if(c->InView())
				r.Offset(c->GetParent()->GetView().TopLeft());
			r.Offset(CtrlMover::GetOffset(*(c->GetParent()), g));
			
			r.Inflate(1);
			RectCtrl::DrawHandleFrame(w, r, hintcol, 1);
		}
		if(f & CtrlFinder::DEEP)
			DrawHintFrame(w, g, *c, hintcol);
		c = c->GetNext();	
	}
}

void CtrlPos::Paint(Draw& w)
{
	Size sz = GetSize();
	
	if(!IsTransparent())
		w.DrawRect(0,0,sz.cx,sz.cy, SColorFace());

	if(!IsEnabled()) return;	

	if(pctrl)
		DrawHintFrame(w, *pctrl, *pctrl, LtGray());
	
	if(!ctrl) return;
	Ctrl& c = *ctrl;
	if(&c == pctrl) return;

	Rect r = c.GetRect();
	Rect _r = c.GetParent()->GetView();
	Rect tr(r);

	if(c.InView())
		r.Offset(_r.TopLeft());
	Point op = CtrlMover::GetOffset(*c.GetParent(), *pctrl);
	r.Offset(op);
	_r.Offset(op);

	RectCtrl::DrawHandleFrame(w, r, style->framecol, style->framesize);

	DrawAlignHandle(w, _r, r, c.GetPos(), style->framecol);

	RectCtrl::DrawHandle(w, r, style->handcol, style->handsize);

	if(pressed)// && moving)
		RectCtrl::DrawRectInfo(w, Point(10,10), tr, style->framecol, style->textcol);
}

void CtrlPos::LeftDown(Point p, dword keyflags)
{
	if(IsReadOnly() || !IsEnabled()) return;

	SetCapture();
	moving = false;
	pressed = (keyflags & K_MOUSELEFT);

	if(ctrl)
	{
		Ctrl& c = *ctrl;
		//if already found prepare moving
		ASSERT(!c.InFrame());
		xpos = c.GetPos();
		xp = p;
	
		Rect r = c.GetRect();
		Rect _r = c.GetParent()->GetView();

		if(c.InView())
			r.Offset(_r.TopLeft());
		Point op = CtrlMover::GetOffset(*c.GetParent(), *pctrl);
		r.Offset(op);
		_r.Offset(op);

		Ctrl::LogPos pos = xpos;
		Rect rr(r); rr.Inflate(style->handsize+2);

		if(GetAlignMode(_r, rr, p, pos, style->handsize))
		{
			pos = LogPosPopUp::MakeLogPos(pos, c);		
			c.SetPos(pos);
			Action();
			Refresh();
			return;
		}
		else if((mode = RectCtrl::GetMode(r, p, keyflags, style->handsize)) != RectCtrl::NONE)
		{
			ci = RectCtrl::SetCursor(mode, keyflags, ci);
			Refresh();
			return;
		}
	}

	if(pctrl)
	{
		Point pt(p);
		int f(flags);
		ctrl = CtrlFinder::GetCtrl(*pctrl, pt, f, filter);
		if(ctrl)
		{
			if(ctrl->InFrame()) //may not move base nor frames
				ClearCtrl();
			else
				WhenLeftDown(*ctrl, p, keyflags);
		}
		Action();
	}

	Refresh();
}

void CtrlPos::MouseMove(Point p, dword keyflags)
{
	if(IsReadOnly() || !IsEnabled()) return;

	moving = true;
	pressed = (keyflags & K_MOUSELEFT);
	//int m = RectCtrl::GetMode(r, p, keyflags, style->handsize);
	//ci = RectCtrl::SetCursor(m, keyflags);
	if(!ctrl) return;
	Ctrl& c = *ctrl;

	if(pressed && mode != RectCtrl::NONE) 
	{
		if(keyflags & K_ALT)
		{
			Ctrl* q = c.GetParent();
			Ctrl* prevc = c.GetPrev();

			//calculate some things not accessible when Removed()

			Rect r = LogPosPopUp::CtrlRect(xpos, q->GetSize());
			if(c.InView())
				r.Offset(c.GetParent()->GetView().TopLeft());
			Point ops = CtrlMover::GetOffset(*(c.GetParent()), *pctrl);
			r.Offset(ops);

			c.Remove(); //prevent moving control from finding when searching new parent
			Point pt(p);
			int ft(flags); flags |= (DEEP | NEST);
			Ctrl* pc = GetCtrl(*pctrl, pt, flags, filter);
			flags &= ~(DEEP | NEST); flags |= (ft & DEEP); //restore DEEP flag from save, NEST is ours
			if(!pc) pc = pctrl;
			if(pc != q)
			{
				r.Offset(-pc->GetView().TopLeft());
				Point opd = CtrlMover::GetOffset(*pc, *pctrl);
				r.Offset(-opd);

				xpos = LogPosPopUp::MakeLogPos(xpos, r, pc->GetSize());
				
				pc->Add(c);
			}
			else
				q->AddChild(&c, prevc); //undo Remove();
		}

		Size psz = c.GetParent()->GetSize();
		Rect r = LogPosPopUp::CtrlRect(xpos, psz);
		RectCtrl::CalcRect(r, p-xp, keyflags, mode, g);
		r.Normalize();
		Ctrl::LogPos pos = LogPosPopUp::MakeLogPos(xpos, r, psz);
		GetCtrl()->SetPos(pos);
		Action();
		Refresh();
	}
}

void CtrlPos::LeftUp(Point p, dword keyflags)
{
	ReleaseCapture();
	pressed = false;
	moving = false;
	//xpos.SetNull();
	xp.SetNull();
	mode = RectCtrl::NONE;
	if(IsReadOnly() || !IsEnabled()) return;

	ci = RectCtrl::SetCursor(mode, keyflags, ci);
	Refresh();
}

void CtrlPos::RightDown(Point p, dword keyflags)
{
	//cancel
	if(!(keyflags & K_SHIFT))
		LeftDown(p, keyflags);
	ReleaseCapture();
	int m = mode;
	mode = RectCtrl::NONE;
	if(IsReadOnly() || !IsEnabled()) return;

	ci = RectCtrl::SetCursor(mode, keyflags, ci);
	if(!GetCtrl()) return;

	if(pressed)
	{
		if(m != RectCtrl::NONE)
		{
			GetCtrl()->SetPos(xpos);
			Action();
			Refresh();
		}
	}
	else if(keyflags & K_SHIFT)
	{
		pressed = false;
		moving = false;
		//xpos.SetNull();
		xp.SetNull();
		//mode = RectCtrl::NONE;
		RectTracker tr(*this);
		Size psz = GetCtrl()->GetParent()->GetSize();
		Rect r = tr.Track(Rect(p,p));

		Ctrl* pc = GetCtrl()->GetParent();
		r.Offset(-pc->GetView().TopLeft());
		Point opd = CtrlMover::GetOffset(*pc, *pctrl);
		r.Offset(-opd);

		Ctrl::LogPos pos = LogPosPopUp::MakeLogPos(xpos, r, psz);
		GetCtrl()->SetPos(pos);
		Action();
		Refresh();
	}
	else if(WhenBar)
		MenuBar::Execute(WhenBar);
}

void CtrlPos::MouseWheel(Point p, int zdelta, dword keyflags)
{
	//if(!IsEditable()) return;
	//if(!HasFocus()) SetFocus();
	if(!GetCtrl()) return;
	Ctrl& c = *GetCtrl();
	
	int i = zdelta/120;

	for(; i<0; ++i)
	{
		Ctrl* p = c.GetNext();
		c.GetParent()->AddChild(&c, p);
		c.Refresh();
	}
	for(; i>0; --i)
	{
		Ctrl* p = c.GetPrev();
		c.GetParent()->AddChildBefore(&c, p);
		c.Refresh();
	}
	
}

void CtrlPos::LeftDouble(Point p, dword flags)
{
	//LeftDown already called
	WhenLeftDouble();
}

bool CtrlPos::Key(dword key, int count)
{
	if(key == K_SPACE)
	{
		ClearCtrl();
		Action();
		Refresh();
		return true;	
	}
	return false;
}

void CtrlPos::Updated()
{
	//refresh the view of the currently selected ctrl
	CtrlFinder::Updated();
}

CtrlPos::CtrlPos()
	: pressed(false), moving(false), mode(RectCtrl::NONE), g(4,4)
{
	BackPaint();
	NoIgnoreMouse();
	style = &RectCtrl::StyleDefault();
	//xpos.SetNull();
	xp.SetNull();
}
