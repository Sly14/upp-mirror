#include "Draw.h"

#define LLOG(x)    // RLOG(x)
#define LTIMING(x) // RTIMING(x)

RichValue<Color>::Registrator MK__s;

void Draw::StartPage() {}
void Draw::EndPage() {}

// -------------------------------

bool Draw::IsDrawing() const
{
	return dynamic_cast<const DrawingDraw *>(this);
}

Stream& Draw::DrawingOp(int code)
{
	ASSERT(IsDrawing());
	Stream& drawing = static_cast<DrawingDraw *>(this)->GetStream();
	drawing / code;
	return drawing;
}

/*
Stream& Draw::PutRect(const Rect& r)
{
	ASSERT(IsDrawing());
	Stream& drawing = dynamic_cast<DrawingDraw *>(this)->GetStream();
	return Pack16(drawing, const_cast<Rect&>(r));
}
*/

// -------------------------------

void Draw::DrawImageOp(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	LTIMING("DrawImageOp");
	if(IsNull(src))
		return;
	Size sz = Size(cx, cy);
	if((cx > 2000 || cy > 2000) && IsNull(color)) { //TODO Color!!!
		int yy = 0;
		ImageRaster ir(img);
		RescaleImage rm;
		rm.Create(Size(cx, cy), ir, src);
		while(yy < cy) {
			int ccy = min(cy - yy, 16);
			ImageBuffer ib(cx, ccy);
			for(int q = 0; q < ccy; q++)
				rm.Get(ib[q]);
			DrawImageBandRLE(*this, x, y + yy, ib, 16);
			yy += ccy;
		}
		return;
	}
	if(src.GetSize() == sz)
		img.PaintImage(*this, x, y, src, color);
	else {
		Image h = Rescale(img, Size(cx, cy), src);
		h.PaintImage(*this, x, y, h.GetSize(), color);
	}
}

// -------------------------------

void Draw::DrawRect(const Rect& rect, Color color)
{
	DrawRect(rect.left, rect.top, rect.GetWidth(), rect.GetHeight(), color);
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Image& img, const Rect& src)
{
	DrawImageOp(x, y, cx, cy, img, src, Null);
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Image& img)
{
	DrawImage(x, y, cx, cy, img, img.GetSize());
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Image& img, const Rect& src, Color color)
{
	if(IsNull(color)) return;
	DrawImageOp(x, y, cx, cy, img, src, color);
}

void Draw::DrawImage(int x, int y, int cx, int cy, const Image& img, Color color)
{
	if(IsNull(color)) return;
	DrawImage(x, y, cx, cy, img, img.GetSize(), color);
}

void Draw::DrawImage(const Rect& r, const Image& img, const Rect& src)
{
	DrawImage(r.left, r.top, r.Width(), r.Height(), img, src);
}

void Draw::DrawImage(const Rect& r, const Image& img)
{
	DrawImage(r.left, r.top, r.Width(), r.Height(), img);
}

void Draw::DrawImage(const Rect& r, const Image& img, const Rect& src, Color color)
{
	if(IsNull(color)) return;
	DrawImage(r.left, r.top, r.Width(), r.Height(), img, src, color);
}

void Draw::DrawImage(const Rect& r, const Image& img, Color color)
{
	if(IsNull(color)) return;
	DrawImage(r.left, r.top, r.Width(), r.Height(), img, color);
}

void Draw::DrawImage(int x, int y, const Image& img, const Rect& src)
{
	Size sz = src.GetSize();
	DrawImageOp(x, y, sz.cx, sz.cy, img, src, Null);
}

void Draw::DrawImage(int x, int y, const Image& img)
{
	Size sz = img.GetSize();
	DrawImageOp(x, y, sz.cx, sz.cy, img, img.GetSize(), Null);
}

void Draw::DrawImage(int x, int y, const Image& img, const Rect& src, Color color)
{
	if(IsNull(color)) return;
	Size sz = img.GetSize();
	DrawImageOp(x, y, sz.cx, sz.cy, img, src, color);
}

void Draw::DrawImage(int x, int y, const Image& img, Color color)
{
	if(IsNull(color)) return;
	Size sz = img.GetSize();
	DrawImageOp(x, y, sz.cx, sz.cy, img, img.GetSize(), color);
}

void Draw::DrawData(int x, int y, int cx, int cy, const String& data, const char *type)
{
	DrawDataOp(x, y, cx, cy, data, type);
}

void Draw::DrawData(const Rect& r, const String& data, const char *type)
{
	DrawDataOp(r.left, r.top, r.GetWidth(), r.GetHeight(), data, type);
}

void Draw::DrawLine(Point p1, Point p2, int width, Color color)
{
	DrawLine(p1.x, p1.y, p2.x, p2.y, width, color);
}

#ifndef PLATFORM_WINCE
void Draw::DrawPolyPolyline(const Vector<Point>& vertices, const Vector<int>& counts,
                            int width, Color color, Color doxor)
{
	DrawPolyPolyline(vertices.Begin(), vertices.GetCount(),
		             counts.Begin(), counts.GetCount(),
		             width, color, doxor);
}

void Draw::DrawPolyline(const Point *vertices, int count,
                        int width, Color color, Color doxor)
{
	DrawPolyPolyline(vertices, count, &count, 1, width, color, doxor);
}

void Draw::DrawPolyline(const Vector<Point>& vertices,
                        int width, Color color, Color doxor)
{
	DrawPolyline(vertices.Begin(), vertices.GetCount(), width, color, doxor);
}

void Draw::DrawPolyPolyPolygon(const Vector<Point>& vertices,
                           const Vector<int>& subpolygon_counts,
                           const Vector<int>& disjunct_polygon_counts,
                           Color color, int width, Color outline,
                           uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(vertices.Begin(), vertices.GetCount(),
	                    subpolygon_counts.Begin(), subpolygon_counts.GetCount(),
	                    disjunct_polygon_counts.Begin(), disjunct_polygon_counts.GetCount(),
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolyPolygon(const Point *vertices, int vertex_count,
                       const int *subpolygon_counts, int subpolygon_count_count,
                       Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(vertices, vertex_count,
	                    subpolygon_counts, subpolygon_count_count, &vertex_count, 1,
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolyPolygon(const Vector<Point>& vertices, const Vector<int>& subpolygon_counts,
                       Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolygon(vertices.Begin(), vertices.GetCount(),
	                subpolygon_counts.Begin(), subpolygon_counts.GetCount(),
	                color, width, outline, pattern, doxor);
}

void Draw::DrawPolygons(const Point *vertices, int vertex_count,
                    const int *polygon_counts, int polygon_count_count,
                    Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(vertices, vertex_count,
	                    polygon_counts, polygon_count_count,
	                    polygon_counts, polygon_count_count,
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolygons(const Vector<Point>& vertices, const Vector<int>& polygon_counts,
                    Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolygons(vertices.Begin(), vertices.GetCount(),
                 polygon_counts.Begin(), polygon_counts.GetCount(),
                 color, width, outline, pattern, doxor);
}

void Draw::DrawPolygon(const Point *vertices, int vertex_count,
                   Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolyPolyPolygon(vertices, vertex_count, &vertex_count, 1, &vertex_count, 1,
	                    color, width, outline, pattern, doxor);
}

void Draw::DrawPolygon(const Vector<Point>& vertices,
                       Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	DrawPolygon(vertices.Begin(), vertices.GetCount(), color, width, outline, pattern, doxor);
}
#endif
void Draw::DrawEllipse(int x, int y, int cx, int cy, Color color, int pen, Color pencolor)
{
	DrawEllipse(RectC(x, y, cx, cy), color, pen, pencolor);
}

void Draw::Offset(int x, int y)
{
	Offset(Point(x, y));
}

bool Draw::Clip(int x, int y, int cx, int cy)
{
	return Clip(RectC(x, y, cx, cy));
}

bool Draw::Clipoff(int x, int y, int cx, int cy)
{
	return Clipoff(RectC(x, y, cx, cy));
}

bool Draw::ExcludeClip(int x, int y, int cx, int cy)
{
	return ExcludeClip(RectC(x, y, cx, cy));
}

bool Draw::IntersectClip(int x, int y, int cx, int cy)
{
	return IntersectClip(RectC(x, y, cx, cy));
}

bool Draw::IsPainting(int x, int y, int cx, int cy) const
{
	return IsPainting(RectC(x, y, cx, cy));
}

// ---------------------------

void NilDraw::BeginOp() {}
void NilDraw::EndOp() {}
void NilDraw::OffsetOp(Point p) {}
bool NilDraw::ClipOp(const Rect& r) { return false; }
bool NilDraw::ClipoffOp(const Rect& r) { return false; }
bool NilDraw::ExcludeClipOp(const Rect& r) { return false; }
bool NilDraw::IntersectClipOp(const Rect& r) { return false; }
Rect NilDraw::GetClipOp() const { return Rect(0, 0, 0, 0); }
bool NilDraw::IsPaintingOp(const Rect& r) const { return false; }

void NilDraw::DrawRectOp(int x, int y, int cx, int cy, Color color) {}
void NilDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& img,
                          const Rect& src, Color color) {}
void NilDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color) {}
void NilDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color pencolor) {}
void NilDraw::DrawTextOp(int x, int y, int angle, const wchar *text, Font font,
	                    Color ink, int n, const int *dx) {}

void NilDraw::DrawDrawingOp(const Rect& target, const Drawing& w) {}

// ---------------------------

void BackDraw::Create(int cx, int cy)
{
	Create(ScreenInfo(), cx, cy);
}

bool BackDraw::IsPaintingOp(const Rect& r) const
{
	Rect rr = r + GetOffset();
	if(!rr.Intersects(size))
		return false;
	return painting ? painting->IsPainting(rr + painting_offset) : true;
}

BackDraw::BackDraw()
{
	painting = NULL;
	painting_offset = Point(0, 0);
}

BackDraw::~BackDraw()
{
	Destroy();
}
