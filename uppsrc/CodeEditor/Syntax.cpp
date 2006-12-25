#include "CodeEditor.h"

NAMESPACE_UPP

Color GetUvsHighlight(const wchar *text, int& n);

const wchar *eatstring(const wchar *s) {
	int delim = *s++;
	while(*s)
		if(*s == '\\' && s[1])
			s += 2;
		else
		if(*s == delim)
			return s + 1;
		else
			s++;
	return s;
}

inline const wchar *strnext(const wchar *p, const wchar *end, int ch) {
	while(p < end) {
		if(*p == ch) return p;
		p++;
	}
	return NULL;
}

void CodeEditor::SyntaxState::Clear() {
	line = cl = bl = pl = 0;
	linecont = linecomment = comment = string = false;
	macro = MACRO_OFF;
	brk.Clear();
	blk.Clear();
	bid.Clear();
	bid.Add(0);
	par.Clear();
	stmtline = endstmtline = seline = -1;
	uvscolor = Null;
	ifstack.Clear();
}

const wchar *isstmt(const wchar *p) {
	static const char *stmt[] = {
		"if", "else", "while", "do", "for"
	};
	for(const char **q = stmt; q < stmt + __countof(stmt); q++) {
		const char *k = *q;
		const wchar *s = p;
		for(;;) {
			if(*k== '\0') {
				if(!iscid(*s)) return s;
				break;
			}
			if(*s != *k)
				break;
			s++;
			k++;
		}
	}
	return NULL;
}

Color CodeEditor::SyntaxState::IfColor(char c)
{
	switch(c)
	{
	case CodeEditor::IfState::IF:          return LtBlue();
	case CodeEditor::IfState::ELIF:        return Gray();
	case CodeEditor::IfState::ELSE:        return Green();
	case CodeEditor::IfState::ELSE_ERROR:  return LtRed();
	case CodeEditor::IfState::ENDIF_ERROR: return LtMagenta();
	default:                               return Null;
	}
}

static WString sReadLn(const wchar *p)
{
	WStringBuffer wbuf;
	while(*p && *p <= ' ')
		p++;
	const wchar *b = p;
	while(*p && !(*p == '/' && (p[1] == '/' || p[1] == '*'))) {
		if(*p <= ' ') {
			while(*++p && *p <= ' ')
				;
			if(*p && !(*p == '/' && (p[1] == '/' || p[1] == '*')))
				wbuf.Cat(' ');
		}
		else
			wbuf.Cat(*p++);
	}
	return wbuf;
}

void CodeEditor::SyntaxState::ScanSyntax(const wchar *ln, const wchar *e)
{
	string = false;
	if(!linecont)
		linecomment = false;
	linecont = e > ln && e[-1] == '\\';
	const wchar *p = ln;
	int lindent = 0;
	while(p < e && *p == '\t') {
		p++;
		lindent++;
	}
	while(p < e && (*p == ' ' || *p == '\t'))
		p++;
	int n;
	Color c = GetUvsHighlight(p, n);
	if(n) {
		uvscolor = c;
		p += n;
	}
	if(!comment && *p == '#') {
		while(++p < e && (*p == ' ' || *p == '\t'))
			p++;
		const wchar *id = p;
		while(p < e && iscid(*p))
			p++;
		int idlen = p - id;
		if(id[0] == 'i' && id[1] == 'f'
		&& (idlen == 2 || idlen == 5 && id[2] == 'd' && id[3] == 'e' && id[4] == 'f'
		|| idlen == 6 && id[2] == 'n' && id[3] == 'd' && id[4] == 'e' && id[5] == 'f')) {
			IfState& ifstate = ifstack.Add();
			ifstate.state = IfState::IF;
			ifstate.iftext = sReadLn(ln);
			ifstate.ifline = line + 1;
		}
		switch(idlen)
		{
		case 6:
			if(id[0] == 'd' && id[1] == 'e' && id[2] == 'f' && id[3] == 'i' && id[4] == 'n' && id[5] == 'e')
				macro = SyntaxState::MACRO_CONT;
			break;

		case 4:
			if(id[0] == 'e' && id[1] == 'l')
				if(id[2] == 'i' && id[3] == 'f')
					if(ifstack.GetCount() == 0) {
						IfState& ifstate = ifstack.Add();
						ifstate.ifline = 0;
						ifstate.state = IfState::ELSE_ERROR;
					}
					else {
						IfState& ifstate = ifstack.Top();
						if(ifstate.state == IfState::IF || ifstate.state == IfState::ELIF) {
							ifstate.state = IfState::ELIF;
							ifstate.iftext = WString().Cat() << sReadLn(ln) << ", " << ifstate.iftext;
						}
						else
							ifstate.state = IfState::ELSE_ERROR;
					}
				else
				if(id[2] == 's' && id[3] == 'e')
					if(ifstack.GetCount() == 0) {
						IfState& ifstate = ifstack.Add();
						ifstate.ifline = 0;
						ifstate.state = IfState::ELSE_ERROR;
					}
					else {
						IfState& ifstate = ifstack.Top();
						if(ifstate.state == IfState::IF || ifstate.state == IfState::ELIF) {
							ifstate.state = IfState::ELSE;
							ifstate.iftext = "#else, " + ifstate.iftext;
						}
						else
							ifstate.state = IfState::ELSE_ERROR;
					}
			break;

		case 5:
			if(id[0] == 'e' && id[1] == 'n' && id[2] == 'd' && id[3] == 'i' && id[4] == 'f')
			{
				int itop = ifstack.GetCount() - 1;
				if(itop < 0) {
					IfState& ifstate = ifstack.Add();
					ifstate.ifline = 0;
					ifstate.state = IfState::ENDIF_ERROR;
				}
				else if(ifstack[itop].state != IfState::ENDIF_ERROR)
					ifstack.Trim(itop);
			}
			break;
		}
	}
	if(macro == SyntaxState::MACRO_CONT && !(p < e && e[-1] == '\\'))
		macro = SyntaxState::MACRO_END;
	for(;;) {
		if(comment) {
			p = strnext(p, e, '*');
			if(!p) break;
			if(*++p == '/')
				comment = false;
		}
		else {
			int pc = 0;
			for(;;) {
				if(p >= e) return;
				const wchar *pp;
				if(!iscid(pc) && (pp = isstmt(p)) != NULL) {
					stmtline = line;
					spar = 0;
					pc = 0;
					p = pp;
				}
				else {
					int c = *p++;
					if(c == '/') break;
					if(c == '\'' || c == '\"') {
						p = eatstring(p - 1);
						if(p >= e) {
							string = true;
							return;
						}
					}
					else
					if(c == ';' && spar == 0) {
						seline = stmtline;
						endstmtline = line;
						stmtline = -1;
					}
					else
					if(c == '{') {
						cl++;
						brk.Add('}');
						blk.Add() = line;
						bid.Add(lindent + 1);
						stmtline = -1;
						par.Clear();
					}
					else
					if(c == '}') {
						if(brk.GetCount()) {
							cl--;
							brk.Drop();
						}
						if(blk.GetCount())
							blk.Drop();
						if(bid.GetCount())
							bid.Drop();
						stmtline = -1;
						par.Clear();
					}
					else
					if(c == '(') {
						pl++;
						brk.Add(')');
						Isx& m = par.Add();
						m.line = line;
						m.pos = p - ln;
						spar++;
					}
					else
					if(c == '[') {
						bl++;
						brk.Add(']');
						Isx& m = par.Add();
						m.line = line;
						m.pos = p - ln;
						spar++;
					}
					else
					if(c == ')') {
						if(brk.GetCount()) {
							pl--;
							brk.Drop();
						}
						if(par.GetCount())
							par.Drop();
						spar--;
					}
					else
					if(c == ']') {
						if(brk.GetCount()) {
							bl--;
							brk.Drop();
						}
						if(par.GetCount())
							par.Drop();
						spar--;
					}
					pc = c;
				}
			}
			if(*p == '/') {
				linecomment = true;
				return;
			}
			if(*p == '*') {
				comment = true;
				p++;
			}
		}
	}
}

CodeEditor::SyntaxState CodeEditor::ScanSyntax(int line) {
	SyntaxState st;
	for(int i = 0; i < 4; i++)
		if(line >= scache[i].line) {
			st = scache[i];
			break;
		}
	if(st.macro != SyntaxState::MACRO_CONT)
		st.macro = SyntaxState::MACRO_OFF;
	while(st.line < line) {
		if(st.macro != SyntaxState::MACRO_CONT)
			st.macro = SyntaxState::MACRO_OFF;
		WString l = GetWLine(st.line);
		st.ScanSyntax(l, l.End());
		st.line++;
		static int d[] = { 0, 100, 2000 };
		for(int i = 0; i < 3; i++)
			if(st.line == cline - d[i])
				scache[i] = st;
	}
	scache[3] = st;
	return st;
}

bool CodeEditor::SyntaxState::MatchHilite(const SyntaxState& st) const
{
	return comment == st.comment
	    && linecont == st.linecont
	    && linecomment == st.linecomment
		&& macro == st.macro
		&& cl == st.cl && bl == st.bl && pl == st.pl
		&& IsEqual(brk, st.brk)
		&& IsEqual(blk, st.blk)
		&& IsEqual(bid, st.bid)
		&& IsEqual(par, st.par)
		&& IsEqual(ifstack, st.ifstack)
		&& uvscolor == st.uvscolor
		&& seline == st.seline
		&& spar == st.spar;
}

END_UPP_NAMESPACE
