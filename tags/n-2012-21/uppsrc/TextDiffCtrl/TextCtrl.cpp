#include "TextDiffCtrl.h"

namespace Upp {


inline Color HistoryBg() { return Color(255, 255, 0); }

TextCompareCtrl::TextCompareCtrl()
{
	letter = Size(1, 1);
	number_width = 0;
	number_yshift = 0;
	number_bg = WhiteGray();
	SetFrame(FieldFrame());
	AddFrame(scroll);
	SetFont(Courier(14), Courier(10));
	scroll.NoAutoHide();
	scroll.WhenScroll = THISBACK(SelfScroll);
	maxwidth = 0;
	tabsize = 4;
	gutter_width = 0;
	gutter_bg = Color(151, 190, 239);
	gutter_fg = SGreen;
}

void TextCompareCtrl::LeftDown(Point pt, dword keyflags)
{
	Size sz = GetSize();
	if(pt.x > sz.cx - gutter_width || HasCapture()) {
		if(!HasCapture())
			SetCapture();
		int line = (pt.y * lines.GetCount()) / sz.cy;
		int page_lines = sz.cy / letter.cy;
		scroll.SetY(line - page_lines / 2);
	}
	SetWantFocus();
}

void TextCompareCtrl::LeftUp(Point pt, dword keyflags)
{
	ReleaseCapture();
}

void TextCompareCtrl::MouseMove(Point pt, dword keyflags)
{
	if(HasCapture())
	{
		LeftDown(pt, keyflags);
	}
}


bool TextCompareCtrl::Key(dword key, int repcnt)
{
	Point pos = scroll, newpos = pos, page = scroll.GetPage();
	switch(key) {
		case K_LEFT:       newpos.x--; break;
		case K_RIGHT:      newpos.x++; break;
		case K_CTRL_LEFT:  newpos.x -= page.x >> 1; break;
		case K_CTRL_RIGHT: newpos.x += page.x >> 1; break;
		case K_UP:         newpos.y--; break;
		case K_DOWN:       newpos.y++; break;
		case K_PAGEUP:     newpos.y -= page.y; break;
		case K_PAGEDOWN:   newpos.y += page.y; break;
		case K_HOME:       newpos.x = 0; break;
		case K_END:        newpos.x = maxwidth - page.x; break;
		case K_CTRL_HOME:  newpos.y = 0; break;
		case K_CTRL_END:   newpos.y = lines.GetCount() - page.y; break;
		case K_F3: {
			bool found = false;
			int i = max(pos.y + 2, 0);
			while(i < lines.GetCount() && lines[i].level)
				i++;
			while(i < lines.GetCount())
				if(lines[i++].level) {
					newpos.y = i - 2;
					found = true;
					break;
				}
			if(!found) {
				BeepExclamation();
				return true;
			}
			break;
		}
		case K_SHIFT_F3: {
			bool found = false;
			int i = min(pos.y, lines.GetCount() - 1);
			while(i > 0 && lines[i].level)
				i--;
			while(i >= 0)
				if(lines[i--].level) {
					newpos.y = i;
					found = true;
					break;
				}
			if(!found) {
				BeepExclamation();
				return true;
			}
			break;
		}
		default: return false;
	}
	if(newpos != pos)
		scroll.Set(newpos);
	return true;
}

void TextCompareCtrl::Paint(Draw& draw)
{
	Point sc = scroll.Get();
	Size offset = (Size)sc * letter;
	Size sz = GetSize();

	int lcnt = lines.GetCount();
	int first_line = offset.cy / letter.cy;
	int last_line = min(idivceil(sz.cy + offset.cy, letter.cy), lines.GetCount() - 1);

	if(gutter_width > 0)
	{
		int t = 0, b = 0;
		int gx = sz.cx - gutter_width;
		for(int i = 0; i < lcnt; i++)
			if(lines[i].level > 1) {
				b = idivceil(sz.cy * i, lcnt);
				if(b >= t) {
					draw.DrawRect(gx, t, gutter_width, b - t, gutter_bg);
					draw.DrawRect(gx, b, gutter_width, 1, gutter_fg);
					t = b + 1;
				}
			}

		draw.DrawRect(gx, t, gutter_width, sz.cy - t, gutter_bg);

		int total = letter.cy * lcnt;
		if(total <= 0)
			total = 1;
		int page_height = (sz.cy * sz.cy) / total;
		int ty = max(0, (sz.cy * offset.cy) / total);
		int by = min(sz.cy, ty + page_height);
		draw.DrawRect(gx, ty, gutter_width, 2, Black);
		draw.DrawRect(gx, by - 2, gutter_width, 2, Black);
		draw.DrawRect(gx, ty, 2, by - ty, Black);
		draw.DrawRect(gx + gutter_width - 2, ty, 2, by - ty, Black);
	}

	Font ifont = Font(font).Italic();
	for(int i = first_line; i <= last_line; i++) {
		const Line& l = lines[i];
		int y = i * letter.cy - offset.cy;
		draw.DrawRect(0, y, number_width, letter.cy, number_bg);
		if(!IsNull(l.number))
			draw.DrawText(0, y + number_yshift, FormatInt(l.number), number_font, l.color);
	}
	draw.Clip(number_width, 0, sz.cx - gutter_width - number_width, sz.cy);
	for(int i = first_line; i <= last_line; i++) {
		const Line& l = lines[i];
		int y = i * letter.cy - offset.cy;
		draw.DrawRect(0, y, sz.cx, letter.cy, SWhite());
		draw.DrawText(number_width - offset.cx, y, ExpandTabs(l.text), l.level == 1 ? ifont : font, l.color);
	}
	int lcy = lcnt * letter.cy - offset.cy;
	draw.DrawRect(0, lcy, sz.cx, sz.cy - lcy, SGray());
	draw.End();
	draw.DrawRect(0, lcy, number_width, sz.cy - lcy, number_bg);
}

void TextCompareCtrl::TabSize(int t)
{
	tabsize = t;
	UpdateWidth();
	Layout();
}

void TextCompareCtrl::SetFont(Font f, Font nf)
{
	font = f;
	number_font = nf;
	FontInfo fi = f.Info();
	FontInfo ni = nf.Info();
	letter.cy = fi.GetHeight();
	letter.cx = fi.GetAveWidth();
	number_width = 5 * ni.GetAveWidth();
	number_yshift = (fi.GetHeight() - ni.GetHeight() + 2) >> 1;
	Layout();
}

void TextCompareCtrl::Layout()
{
	scroll.Set(scroll, (scroll.GetReducedViewSize() - Size(number_width + gutter_width, 0)) / letter, Size(maxwidth, lines.GetCount()));
	Refresh();
}

void TextCompareCtrl::MouseWheel(Point pt, int zdelta, dword keyflags)
{
	scroll.WheelY(zdelta);
}

void TextCompareCtrl::SetCount(int c)
{
	bool rl = (c < lines.GetCount());
	lines.SetCount(c);
	if(rl)
		UpdateWidth();
	Layout();
}

void TextCompareCtrl::AddCount(int c)
{
	lines.InsertN(lines.GetCount(), c);
	Layout();
}

void TextCompareCtrl::Set(int line, String text, Color color, int number, int level)
{
	Line& l = lines.At(line);
	int tl = MeasureLength(text);
	int lt = MeasureLength(l.text);
	bool rl = (tl < lt && lt == maxwidth);
	l.number = number;
	l.text = text;
	l.color = color;
	l.level = level;
	if(rl)
		UpdateWidth();
	else if(tl > maxwidth) {
		maxwidth = tl;
		Layout();
	}
}

void TextCompareCtrl::SelfScroll()
{
	Refresh();
	WhenScroll();
}

void TextCompareCtrl::PairScroll(TextCompareCtrl *ctrl)
{
	scroll.Set(ctrl->scroll.Get());
}

void TextCompareCtrl::UpdateWidth()
{
	maxwidth = 0;
	for(int i = 0; i < lines.GetCount(); i++)
		maxwidth = max(maxwidth, MeasureLength(lines[i].text));
}

int TextCompareCtrl::MeasureLength(const char *text) const
{
	int pos = 0;
	while(*text)
		if(*text++ == '\t')
			pos += tabsize - pos % tabsize;
		else
			pos++;
	return pos;
}

String TextCompareCtrl::ExpandTabs(const char *text) const
{
	String out;
	for(char c; c = *text++;)
		if(c == '\t')
			out.Cat(' ', tabsize - out.GetLength() % tabsize);
		else
			out.Cat(c);
	return out;
}

Point TextCompareCtrl::GetPos() const
{
	Point pos(0, 0);
	int ltop = minmax(scroll.Get().y, 0, lines.GetCount() - 1);
	while(ltop >= 0 && IsNull(lines[ltop].number)) {
		ltop--;
		pos.y++;
	}
	if(ltop >= 0)
		pos.x = lines[ltop].number;
	return pos;
}

void TextCompareCtrl::SetPos(Point pos)
{
	int l = FindFieldIndex(lines, &Line::number,pos.x);
	if(l < 0)
		l = 0;
	SetSb(l + pos.y);
}

};
