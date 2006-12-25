#include "CtrlLib.h"

NAMESPACE_UPP

SliderCtrl::SliderCtrl()
: min(0)
, max(100)
, value(Null)
, step(1)
, round_step(true)
{
	Transparent();
	NoWantFocus();
}

bool SliderCtrl::IsVert() const
{
	return GetSize().cx < GetSize().cy;
}

int  SliderCtrl::HoVe(int  x, int  y) const
{
	return IsVert() ? y : x;
}

int& SliderCtrl::HoVeR(int& x, int& y) const
{
	return IsVert() ? y : x;
}

void SliderCtrl::Paint(Draw& w)
{
	Size size = GetSize();
	if(IsVert()) {
		int half = size.cx >> 1;
		DrawBorder(w, half - 2, 2, 4, size.cy - 4, InsetBorder);
		if(!IsNull(value))
			w.DrawImage((size.cx - CtrlImg::vthumb().GetSize().cx) >> 1, SliderToClient(value),
			            HasCapture() || HasFocus() ? CtrlImg::vthumb1() : CtrlImg::vthumb());
	}
	else {
		int half = size.cy >> 1;
		DrawBorder(w, 2, half - 2, size.cx - 4, 4, InsetBorder);
		if(!IsNull(value))
			w.DrawImage(SliderToClient(value), (size.cy - CtrlImg::hthumb().GetSize().cy) >> 1,
			            HasCapture() || HasFocus() ? CtrlImg::hthumb1() : CtrlImg::hthumb());
	}
}

bool SliderCtrl::Key(dword key, int repcnt)
{
	if(IsEditable())
		switch(key) {
		case K_LEFT:
		case K_UP:
			Dec();
			return true;
		case K_RIGHT:
		case K_DOWN:
			Inc();
			return true;
		}
	return Ctrl::Key(key, repcnt);
}

void SliderCtrl::LeftDown(Point pos, dword keyflags)
{
	if(!IsEditable())
		return;
	int thumb = SliderToClient(value);
	int p = HoVe(pos.x, pos.y);
	if(IsNull(thumb)) {
		value = ClientToSlider(p);
		UpdateActionRefresh();
	}
	else
	if(p >= thumb && p < thumb + HoVe(CtrlImg::hthumb().GetSize().cx, CtrlImg::vthumb().GetSize().cy))
		SetCapture();
	else
	if(p < thumb)
		Dec();
	else
		Inc();
	Refresh();
}

void SliderCtrl::LeftRepeat(Point p, dword f)
{
	if(!HasCapture())
		LeftDown(p, f);
}

void SliderCtrl::LeftUp(Point pos, dword keyflags)
{
	Refresh();
}

void SliderCtrl::MouseMove(Point pos, dword keyflags)
{
	if(HasCapture()) {
		int n = ClientToSlider(HoVe(pos.x, pos.y));
		if(n != value) {
			value = n;
			UpdateActionRefresh();
		}
	}
}

void SliderCtrl::SetData(const Value& v)
{
	int i = v;
	if(!IsNull(i))
		i = minmax(i, min, max);
	if(i != value) {
		value = i;
		UpdateRefresh();
	}
}

Value SliderCtrl::GetData() const
{
	return value;
}

SliderCtrl& SliderCtrl::MinMax(int _min, int _max)
{
	if(min != _min || max != _max) {
		min = _min;
		max = _max;
		if(!IsNull(value)) {
			int v = minmax(value, min, max);
			if(value != v) {
				value = v;
				Update();
			}
		}
		Refresh();
	}
	return *this;
}

int SliderCtrl::SliderToClient(int v) const
{
	if(IsNull(v))
		return Null;
	v = minmax(v, min, max);
	if(max > min)
		v = iscale(v - min, HoVe(GetSize().cx - CtrlImg::hthumb().GetSize().cx,
		                         GetSize().cy - CtrlImg::vthumb().GetSize().cy), max - min);
	else
		v = 0;
	return v;
}

int SliderCtrl::ClientToSlider(int p) const
{
	if(max <= min)
		return min;
	return minmax(min + iscale(p, max - min,
	                           HoVe(GetSize().cx - CtrlImg::hthumb().GetSize().cx,
	                                GetSize().cy - CtrlImg::vthumb().GetSize().cy)), min, max);
}

void SliderCtrl::Dec()
{
	int n = value;
	if(IsNull(value))
		n = max;
	else
	if(n > min) {
		if(round_step && step > 1)
			n = idivfloor(n - 1, step) * step;
		else
			n -= step;
		if(n < min)
			n = min;
	}
	if(n != value) {
		value = n;
		UpdateActionRefresh();
	}
}

void SliderCtrl::Inc()
{
	int n = value;
	if(IsNull(value))
		n = min;
	else
	if(n < max) {
		if(round_step && step > 1)
			n = idivceil(n + 1, step) * step;
		else
			n += step;
		if(n > max)
			n = max;
	}
	if(n != value) {
		value = n;
		UpdateActionRefresh();
	}
}

SliderCtrl::~SliderCtrl() {}

END_UPP_NAMESPACE
