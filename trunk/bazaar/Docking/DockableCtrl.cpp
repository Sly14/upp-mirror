#include "DockableCtrl.h"

#define IMAGECLASS DockingImg
#define IMAGEFILE <Docking/Docking.iml>
#include <Draw/iml_source.h>

#include <Docking/DockCont.h>

DockCont * DockableCtrl::GetContainer() const
{
	return dynamic_cast<DockCont *>(GetParent());
}

DockableCtrl & DockableCtrl::Set(const Image &_icon, const char *_title, String _group)
{
	return SetGroup(_group).Title(_title).Icon(_icon);
}

DockableCtrl & DockableCtrl::SizeHint(const Size &min, const Size &max, const Size &std)
{
	minsize = min; maxsize = max; stdsize = std; 
	return *this;	
}

bool DockableCtrl::IsFloating() const
{
	DockCont *c = GetContainer();
	return c && c->IsFloating();
}

bool DockableCtrl::IsDocked() const
{
	DockCont *c = GetContainer();
	return c && c->IsDocked();
}

bool DockableCtrl::IsAutoHide() const
{
	DockCont *c = GetContainer();
	return c && c->IsAutoHide();		
}

bool DockableCtrl::IsTabbed() const
{
	DockCont *c = GetContainer();
	return c && (c->IsTabbed() || c->GetCount() > 1);	
}

bool DockableCtrl::IsHidden() const
{
	DockCont *c = GetContainer();
	return !c || c->IsHidden();	
}

int DockableCtrl::GetDockAlign() const
{
	DockCont *c = GetContainer();
	return c && c->GetDockAlign();
}

DockableCtrl & DockableCtrl::SetGroup(const String &g)
{
	DockCont *c = GetContainer();
	group = g;
	if (c) c->GroupRefresh();
	return *this;
}

void DockableCtrl::Highlight()
{
	if (!GetParent() || !IsOpen()) return;
	ViewDraw v(this); 
	ChPaint(v, GetSize(), GetStyle().highlight);	
}

DockableCtrl::DockableCtrl()
{
	style = NULL;
	group = Null;
	maxsize = Null;
	stdsize = Null;
	minsize = Size(0, 0);
	AllowDockAll();
	Transparent(true);
	BackPaint();
}

void DockableCtrlImgsLook(Value* look, int i, int n)
{
	while(n--)
		*look++ = DockingImg::Get(i++);
}

Image ChCrop(const Value &element, Size canvas, Rect crop, Color baseline)
{
	ImageDraw draw(canvas);
	ChPaint(draw, canvas, element);	
	if (!IsNull(baseline)) {
		draw.DrawLine(0, canvas.cy-1, canvas.cx+1, canvas.cy, 1, baseline);
	}
	return Crop(draw, crop);
}

Image StandardHighlight(Color inside, Color border)
{
	Size sz(5, 5);
	ImageBuffer ib(sz);
	RGBA *q = ~ib;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			*(q++) = (i == 4 || j == 4 || !i || !j) ? border : inside;
	ib.SetHotSpot(Point(1, 1));
	ib.Set2ndSpot(Point(3, 3));
	return ib;
}

Image AlphaHighlight(const Image &img, int alpha)
{
	ImageDraw draw(img.GetSize());
	draw.Alpha().DrawRect(img.GetSize(), GrayColor(alpha));
	draw.DrawImage(0, 0, img);
	// Is there a better way to set hotspots than this?
	Image temp = (Image)draw; // GCC
	ImageBuffer ib(temp);
	ib.SetHotSpot(Point(1, 1));
	ib.Set2ndSpot(Point(3, 3));
	return ib;
}

CH_STYLE(DockableCtrl, Style, StyleDefault)
{
	const TabCtrl::Style* style = &TabCtrl::StyleDefault();
	
	handle[0] = ChCrop(style->normal[0], Size(20, 20), Rect(2, 2, 11, 20), SColorShadow); // No focus
	handle[1] = Colorize(handle[0], SColorHighlight(), 160); // Focus
	handle_margins = Rect(2, 0, 0, 0);
	handle_vert = false;
	
	title_font = StdFont();
	title_ink[0] = SColorInfoText(); 		// No focus
	title_ink[1] = SColorHighlightText(); 	// Focus
	
	DockableCtrlImgsLook(close, DockingImg::I_DClosed, 4);
	DockableCtrlImgsLook(windowpos, DockingImg::I_DMenud, 4);
	DockableCtrlImgsLook(autohide, DockingImg::I_DHided, 4);
	
	Image img = StandardHighlight(Blend(SColorHighlight, SColorPaper, 90), Blend(SColorHighlight, SBlack, 90));
	highlight[0] = img;	
	highlight[1] = AlphaHighlight(img, 160);	
}

CH_STYLE(DockableCtrl, Style, StyleDefaultVert)
{
	Assign(DockableCtrl::StyleDefault());
	
	handle[0] = RotateAntiClockwise(handle[0]); // No focus
	handle[1] = RotateAntiClockwise(handle[1]); // Focus
	handle_margins = Rect(0, 2, 2, 1);
	handle_vert = true;
}

