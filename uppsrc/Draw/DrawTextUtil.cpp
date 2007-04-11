#include "Draw.h"

NAMESPACE_UPP

void DrawTextEllipsis(Draw& w, int x, int y, int cx, const wchar *text, const char *ellipsis,
				      Font font, Color ink, int n)
{
	if(n < 0) n = wstrlen(text);
	FontInfo f = font.Info();
	const char *s;
	int dtl = 0;
	int el = 0;
	for(s = ellipsis; *s; s++) {
		dtl += f[(byte)*s];
		el++;
	}
	int l = 0;
	int i;
	for(i = 0; i < n; i++) {
		l += f[(byte) text[i]];
		if(l > cx) {
			while(l + dtl > cx && i > 0) {
				l -= f[(byte) text[i]];
				i--;
			}
			i++;
			break;
		}
	}
	w.DrawText(x, y, text, font, ink, i);
	if(i < n)
		w.DrawText(x + l, y, ellipsis, font, ink, el);
}

void DrawTextEllipsis(Draw& w, int x, int y, int cx, const char *text, const char *ellipsis,
				      Font font, Color ink, int n)
{
	DrawTextEllipsis(w, x, y, cx, WString(text), ellipsis, font, ink, n);
}

Size GetTLTextSize(const wchar *text, Font font)
{
	Size sz(0, 0);
	int cy = font.Info().GetHeight();
	const wchar *s = text;
	const wchar *t = s;
	for(;;) {
		if(*s == '\n' || *s == '\0') {
			int a = 0;
			const wchar *q = t;
			while(q < s) {
				while(q < s && *q < ' ') {
					if(*q == '\t')
						a = (a + 2 * cy) / (2 * cy) * (2 * cy);
					q++;
				}
				t = q;
				while(q < s && *q >= ' ')
					q++;
				a += GetTextSize(t, font, q - t).cx;
			}
			t = s + 1;
			sz.cy += cy;
			sz.cx = max(sz.cx, a);
		}
		if(*s++ == '\0') break;
	}
	return sz;
}

Size GetTLTextSize(Draw&, const wchar *text, Font font)
{
	return GetTLTextSize(text, font);
}

int GetTLTextHeight(const wchar *s, Font font)
{
	int cy = font.Info().GetHeight();
	int h = cy;
	while(*s) {
		if(*s == '\n')
			h += cy;
		s++;
	}
	return h;
}

int GetTLTextHeight(Draw&, const wchar *s, Font font)
{
	return GetTLTextHeight(s, font);
}

void DrawTLText(Draw& draw, int x, int y, int cx, const wchar *text,
                Font font, Color ink, int accesskey) {
	int cy = font.Info().GetHeight();
	const wchar *s = text;
	const wchar *t = s;
	for(;;) {
		if(*s == '\n' || *s == '\0') {
			int a = x;
			const wchar *q = t;
			while(q < s) {
				while(q < s && *q < ' ') {
					if(*q == '\t')
						a = (a - x + 2 * cy) / (2 * cy) * (2 * cy) + x;
					q++;
				}
				t = q;
				bool ak = false;
				while(q < s && *q >= ' ') {
					if(accesskey && ToUpper(ToAscii(*q)) == accesskey) {
						ak = true;
						accesskey = 0;
						break;
					}
					q++;
				}
				draw.DrawText(a, y, t, font, ink, q - t);
				a += GetTextSize(t, font, q - t).cx;
				if(ak) {
					draw.DrawText(a, y, q, font().Underline(), ink, 1);
					a += GetTextSize(q, font().Underline(), 1).cx;
					q++;
				}
			}
			t = s + 1;
			y += cy;
		}
		if(*s++ == '\0') break;
	}
}

END_UPP_NAMESPACE
