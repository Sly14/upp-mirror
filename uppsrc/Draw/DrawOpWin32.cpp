#include "Draw.h"
#ifdef PLATFORM_WIN32

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

void Draw::BeginOp()
{
	LTIMING("Begin");
	Cloff& w = cloff.Add();
	w.org = actual_offset;
	w.hrgn = CreateRectRgn(0, 0, 0, 0);
	ASSERT(w.hrgn);
	int	q = ::GetClipRgn(handle, w.hrgn);
	ASSERT(q >= 0);
	if(q == 0) {
		DeleteObject(w.hrgn);
		w.hrgn = NULL;
	}
}

void Draw::OffsetOp(Point p)
{
	Begin();
	actual_offset += p;
	LTIMING("Offset");
	SetOrg();
}

bool Draw::ClipOp(const Rect& r)
{
	Begin();
	LTIMING("Clip");
	return IntersectClip(r);
}

bool Draw::ClipoffOp(const Rect& r)
{
	Begin();
	LTIMING("Clipoff");
	LLOG("ClipoffOp " << r << ", GetClip() = " << GetClip() << ", actual_offset = " << actual_offset);
	actual_offset += r.TopLeft();
	bool q = IntersectClip(r);
	SetOrg();
	LLOG("//ClipoffOp, GetClip() = " << GetClip() << ", actual_offset = " << actual_offset);
	return q;
}

void Draw::EndOp()
{
	LTIMING("End");
	ASSERT(cloff.GetCount());
	Cloff& w = cloff.Top();
	actual_offset = w.org;
	::SelectClipRgn(handle, w.hrgn);
	SetOrg();
	if(w.hrgn)
		::DeleteObject(w.hrgn);
	cloff.Drop();
}

bool Draw::ExcludeClipOp(const Rect& r)
{
	LTIMING("ExcludeClip");
	Rect rr = LPtoDP(r);
	HRGN hrgn = ::CreateRectRgnIndirect(rr);
	int q = ::ExtSelectClipRgn(handle, hrgn, RGN_DIFF);
	ASSERT(q != ERROR);
	::DeleteObject(hrgn);
	return q == SIMPLEREGION || q == COMPLEXREGION;
}

bool Draw::IntersectClipOp(const Rect& r)
{
	LTIMING("Intersect");
	Rect rr = LPtoDP(r);
	HRGN hrgn = ::CreateRectRgnIndirect(rr);
	int q = ::ExtSelectClipRgn(handle, hrgn, RGN_AND);
	ASSERT(q != ERROR);
	::DeleteObject(hrgn);
	return q == SIMPLEREGION || q == COMPLEXREGION;
}

Rect Draw::GetClipOp() const
{
	Rect r;
	::GetClipBox(handle, r);
	return r;
}

bool Draw::IsPaintingOp(const Rect& r) const
{
	LTIMING("IsPainting");
	LLOG("Draw::IsPaintingOp r: " << r);
	return ::RectVisible(handle, r);
}

void Draw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	LTIMING("DrawRect");
	LLOG("DrawRect " << RectC(x, y, cx, cy) << ": " << color);
	if(IsNull(color)) return;
	if(cx <= 0 || cy <= 0) return;
	if(color == InvertColor)
		::PatBlt(handle, x, y, cx, cy, DSTINVERT);
	else {
		SetColor(color);
		::PatBlt(handle, x, y, cx, cy, PATCOPY);
	}
}

void Draw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	if(IsNull(width) || IsNull(color)) return;
	SetDrawPen(width, color);
	::MoveToEx(handle, x1, y1, NULL);
	::LineTo(handle, x2, y2);
}

void Draw::DrawPolyPolylineOp(const Point *vertices, int vertex_count,
                            const int *counts, int count_count,
	                        int width, Color color, Color doxor)
{
	ASSERT(count_count > 0 && vertex_count > 0);
	if(vertex_count < 2 || IsNull(color))
		return;
	bool is_xor = !IsNull(doxor);
	if(is_xor)
		color = Color(color.GetR() ^ doxor.GetR(), color.GetG() ^ doxor.GetG(), color.GetB() ^ doxor.GetB());
	if(is_xor)
		SetROP2(GetHandle(), R2_XORPEN);
	SetDrawPen(width, color);
	if(count_count == 1)
		::Polyline(GetHandle(), (const POINT *)vertices, vertex_count);
	else
		::PolyPolyline(GetHandle(), (const POINT *)vertices,
		               (const dword *)counts, count_count);
	if(is_xor)
		SetROP2(GetHandle(), R2_COPYPEN);
}

static void DrawPolyPolyPolygonRaw(
	Draw& draw, const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count)
{
	for(int i = 0; i < disjunct_polygon_count_count; i++, disjunct_polygon_counts++)
	{
		int poly = *disjunct_polygon_counts;
		int sub = 1;
		if(*subpolygon_counts < poly)
			if(disjunct_polygon_count_count > 1)
			{
				const int *se = subpolygon_counts;
				int total = 0;
				while(total < poly)
					total += *se++;
				sub = se - subpolygon_counts;
			}
			else
				sub = subpolygon_count_count;
		ASSERT(sizeof(POINT) == sizeof(Point)); // modify algorithm when not
		if(sub == 1)
			Polygon(draw, (const POINT *)vertices, poly);
		else
			PolyPolygon(draw, (const POINT *)vertices, subpolygon_counts, sub);
		vertices += poly;
		subpolygon_counts += sub;
	}
}

void Draw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
	Color color, int width, Color outline, Image image, Color doxor)
{
	if(vertex_count == 0)
		return;
	bool is_xor = !IsNull(doxor);
	HDC hdc = GetHandle();
	if(image) {
		int old_rop = GetROP2(hdc);
		HGDIOBJ old_brush = GetCurrentObject(hdc, OBJ_BRUSH);
		COLORREF old_bk = GetBkColor(hdc);
		COLORREF old_fg = GetTextColor(hdc);
		if(!is_xor) {
			SetROP2(hdc, R2_MASKPEN);
//			SelectObject(hdc, image.GetBrush(Null, Null));
			SetTextColor(hdc, Black());
			SetBkColor(hdc, White());
			SetDrawPen(PEN_NULL, Black);
			DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
				subpolygon_counts, subpolygon_count_count,
				disjunct_polygon_counts, disjunct_polygon_count_count);
			SetROP2(hdc, R2_MERGEPEN);
//			if(IsNull(color)) // use color fill brush
//				SelectObject(hdc, image.GetBrush(Null, Black));
//			else
			{ // just change text color
				SetTextColor(hdc, color);
				SetBkColor(hdc, Black());
			}
		}
		else if(!IsNull(color)) { // colored xor fill with image mask
			SetROP2(hdc, R2_XORPEN);
			SetTextColor(hdc, COLORREF(color) ^ COLORREF(doxor));
//			SelectObject(hdc, image.GetBrush(Null, Null));
		}
		else { // xor fill with image data
			SetROP2(hdc, R2_XORPEN);
//			SelectObject(hdc, image.GetBrush(Null, Black));
		}
		DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
			subpolygon_counts, subpolygon_count_count,
			disjunct_polygon_counts, disjunct_polygon_count_count);
		SelectObject(hdc, old_brush);
		SetTextColor(hdc, old_fg);
		SetBkColor(hdc, old_bk);
		SetROP2(hdc, old_rop);
		if(!IsNull(outline)) {
			SetColor(Null);
			SetDrawPen(width, outline);
			ASSERT(sizeof(POINT) == sizeof(Point));
			DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
				subpolygon_counts, subpolygon_count_count,
				disjunct_polygon_counts, disjunct_polygon_count_count);
		}
	}
	else { // simple fill
//		RTIMING("AreaTool::Fill(solid color)");
		SetDrawPen(IsNull(outline) ? PEN_NULL : width, Nvl(outline, Black));
		int old_rop2;
		if(is_xor) {
			color = Color(color.GetR() ^ doxor.GetR(), color.GetG() ^ doxor.GetG(), color.GetB() ^ doxor.GetB());
			old_rop2 = SetROP2(hdc, R2_XORPEN);
		}
		SetColor(color);
		DrawPolyPolyPolygonRaw(*this, vertices, vertex_count,
			subpolygon_counts, subpolygon_count_count,
			disjunct_polygon_counts, disjunct_polygon_count_count);
		if(is_xor)
			SetROP2(hdc, old_rop2);
	}
}

void Draw::DrawEllipseOp(const Rect& r, Color color, int width, Color pencolor)
{
	SetColor(color);
	SetDrawPen(width, pencolor);
	::Ellipse(GetHandle(), r.left, r.top, r.right, r.bottom);
}

void Draw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	SetDrawPen(width, color);
	::Arc(GetHandle(), rc.left, rc.top, rc.right, rc.bottom, start.x, start.y, end.x, end.y);
}

#endif
