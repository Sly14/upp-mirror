#include "RichEdit.h"

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

struct Speller {
	String        data;
	byte          charset;
	const char   *voc[256];
	int           dict;
	struct Line : Moveable<Line> {
		const byte *begin;
		const byte *end;
	};
	VectorMap<dword, Line> line;
	Index<WString> user;

	bool     Set(const String& data);
	void     Clear()                        { data.Clear(); }
	operator bool() const                   { return !data.IsEmpty(); }

	bool Check(const wchar *wrd, int len) const;
};

bool Speller::Set(const String& _data)
{
	data = _data;
	const char *s = data;
	if(s >= data.End()) {
		data.Clear();
		return false;
	}
	charset = *s++;
	s++;// reserved for prefixes
	dict = *s++;
	for(int i = 0; i < 256 - dict; i++) {
		if(s >= data.End()) {
			data.Clear();
			return false;
		}
		voc[i] = s;
		while(*s) s++;
		s++;
	}
	line.Clear();
	while(s < data.End()) {
		if(s + 8 >= data.End()) {
			data.Clear();
			return false;
		}
		int code = Peek32le(s);
		s += 4;
		int len = Peek32le(s);
		s += 4;
		Line& l = line.Add(code);
		l.begin = (const byte *)s;
		s += len;
		l.end = (const byte *)s;
	};
	return true;
}

bool compare3(const char *s, const char *a, const char *b, int len)
{
	const char *e = s + len;
	while(s < e) {
		if(*s != *a && *s != *b)
			return false;
		s++;
		a++;
		b++;
	}
	return true;
}

bool Speller::Check(const wchar *wrd, int wlen) const
{
	int len = wlen;
	if(len < 64) {
		if(len == 1) return true;
		wchar wh[64];
		const wchar *ws = wrd;
		const wchar *we = wrd + len;
		wchar *wt = wh;
		while(ws < we)
			*wt++ = ToLower(*ws++);

		char w[64];
		char wl[64];
		FromUnicode(w, wrd, len, charset);
		FromUnicode(wl, wh, len, charset);
		if(len == 2) {
			w[len] = 127;
			wl[len++] = 127;
		}
		int i = line.Find(ToLower(wl[0], charset) +
		                  (ToLower(wl[1], charset) << 8) +
		                  (ToLower(wl[2], charset) << 16));
		if(i >= 0) {
			const byte *s = line[i].begin;
			const byte *e = line[i].end;
			int capitals = 0;
			char q[64];
			int  pos = 0;
			while(s < e)
				if(*s < dict) {
					if(pos == len && compare3(q, w, wl, len))
						return true;
					pos = *s++;
					ASSERT(pos < 64);
					capitals = 0;
				}
				else {
					ASSERT(*s >= dict);
					const char *x = voc[(int)*s++ - dict];
					while(*x)
						q[pos++] = *x++;
					ASSERT(pos < 64);
				}
			if(pos == len && compare3(q, w, wl, len))
				return true;
		}
	}
	return user.Find(WString(wrd, wlen)) >= 0;
}

String GetDicFile(int lang)
{
	String name = LNGAsText(lang) + ".scd";
	String dir = ConfigFile("dict");
	String f;
	for(;;) {
		f = AppendFileName(dir, name);
		if(FileExists(f)) return f;
		String d = GetFileFolder(dir);
		if(d == dir) break;
		dir = d;
	}
	f = GetFileOnPath(name, getenv("LIB"));
	if(f.IsEmpty())
		f = GetFileOnPath(name, getenv("LIB"));
	return f;
}

static String sUserFile(int lang)
{
	return ConfigFile(LNGAsText(lang) + ".usp");
}

static Speller& sGetSpeller(int lang)
{
	static ArrayMap<int, Speller> speller;
	int q = speller.Find(lang);
	if(q < 0) {
		Speller& p = speller.Add(lang);
		String f = GetDicFile(lang);
		if(!f.IsEmpty()) {
			p.Set(LoadFile(f));
			FileIn user(sUserFile(lang));
			while(!user.IsEof()) {
				String s = user.GetLine();
				if(!s.IsEmpty())
					p.user.Add(FromUtf8(s));
			}
		}
		return p;
	}
	return speller[q];
}

bool RichEdit::SpellWord(const wchar *wrd, int len, int lang)
{
	Speller& p = sGetSpeller(lang);
	return p ? p.Check(wrd, len) : true;
}

void RichEdit::SpellerAdd(const WString& w, int lang)
{
	Speller& p = sGetSpeller(lang);
	if(p && !p.Check(w, w.GetLength())) {
		FileAppend fa(sUserFile(lang));
		fa.PutLine(ToUtf8(w));
		p.user.Add(w);
	}
}

int RichEdit::fixedlang;

Bits RichEdit::SpellParagraph(const RichPara& para)
{
	int len = para.GetLength();
	Buffer<wchar> text(len);
	Buffer<int> lang(len);
	wchar *s = text;
	int *g = lang;
	for(int i = 0; i < para.GetCount(); i++) {
		const RichPara::Part& p = para[i];
		if(p.IsText()) {
			int l = p.text.GetLength();
			memcpy(s, p.text, l * sizeof(wchar));
			Fill(g, g + l, fixedlang ? fixedlang : p.format.language);
			s += l;
			g += l;
		}
		else {
			*s++ = 127;
			*g++ = 0;
		}
	}
	Bits e;
	s = text;
	wchar *end = text + len;
	while(s < end) {
		if(IsLetter(*s)) {
			const wchar *q = s;
			while(s < end && IsLetter(*s) || s + 1 < end && *s == '\'' && IsLetter(s[1]))
				s++;
			if(!SpellWord(q, s - q, lang[q - text]))
				e.Set(q - text, true, s - q);
		}
		else
			s++;
	}
	return e;
}

END_UPP_NAMESPACE
