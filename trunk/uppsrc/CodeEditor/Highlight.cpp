#include "CodeEditor.h"

NAMESPACE_UPP

#define LTIMING(x)  // RTIMING(x)

bool cmps(const wchar *q, const char *s, int& n) {
	const wchar *t = q;
	while(*q)
		if(*q++ != *s++)
			return false;
	n += int(q - t);
	return *q == *s;
}

bool IsUpperString(const char *q)
{
	while(*q)
	{
		if(*q != '_' && (*q < '0' || *q > '9') && !IsUpper(*q))
			return false;
		q++;
    }
	return true;
}

#define UVSALERT      Color(255, 240, 240)
#define UVSREPINSERT  Color(240, 240, 255)
#define UVSYOUINSERT  Color(240, 255, 240)
#define UVSDELETE     Color(240, 240, 240)

Color GetUvsHighlight(const wchar *text, int& n) {
	n = 0;
	if(text[0] != '$' || text[1] != 'u' || text[2] != 'v' || text[3] != 's' || text[4] != ':' || text[5] != ' ')
		return Null;
	n = 6;
	const wchar *q = text + 6;
	if(cmps(q, "PENDING CONFLICT", n))
		return Null;
	if(cmps(q, "YOUR DELETE (REPOSITORY DELETE)", n))
		return UVSALERT;
	if(cmps(q, "END YOUR DELETE (REPOSITORY DELETE)", n))
		return UVSDELETE;
	if(cmps(q, "YOUR INSERT (REPOSITORY DELETE)", n))
		return UVSALERT;
	if(cmps(q, "END YOUR INSERT (REPOSITORY DELETE)", n))
		return UVSDELETE;
	if(cmps(q, "REPOSITORY DELETE (YOUR DELETE)", n))
		return UVSALERT;
	if(cmps(q, "END REPOSITORY DELETE (YOUR DELETE)", n))
		return UVSDELETE;
	if(cmps(q, "REPOSITORY INSERT (YOUR DELETE)", n))
		return UVSALERT;
	if(cmps(q, "END REPOSITORY INSERT (YOUR DELETE)", n))
		return UVSDELETE;
	if(cmps(q, "YOUR INSERT", n))
		return UVSYOUINSERT;
	if(cmps(q, "END YOUR INSERT", n))
		return Null;
	if(cmps(q, "YOUR DELETE", n))
		return UVSDELETE;
	if(cmps(q, "END YOUR DELETE", n))
		return Null;
	if(cmps(q, "REPOSITORY DELETE", n))
		return UVSDELETE;
	if(cmps(q, "END REPOSITORY DELETE", n))
		return Null;
	if(cmps(q, "REPOSITORY INSERT", n))
		return UVSREPINSERT;
	if(cmps(q, "END REPOSITORY INSERT", n))
		return Null;
	n = 0;
	return Null;
}

struct CodeEditor::HlSt {
	Vector<LineEdit::Highlight>& v;
	LineEdit::Highlight          def;
	int                          pos;

	void Set(int pos, int count, const HlStyle& ink);
	void SetFont(int pos, int count, const HlStyle& f);
	void SetPaper(int pos, int count, Color paper);
	void SetInk(int pos, int count, Color ink);
	void Put(int count, const HlStyle& ink)           { Set(pos, count, ink); pos += count; }
	void Put(const HlStyle& ink)                      { Put(1, ink); }
	void Put(const HlStyle& ink, word flags)          { Put(1, ink); v[pos - 1].flags = flags; }

	HlSt(Vector<LineEdit::Highlight>& v) : v(v) {
		pos = 0;
		def = v[0];
		def.chr = ' ';
		v.Top().chr = ' ';
	}

	~HlSt() {
		for(int i = 0; i < v.GetCount(); i++)
			if(v[i].chr == '_') {
				v[i].font.NoBold();
				v[i].font.NoItalic();
			}
	}
};

void CodeEditor::HlSt::Set(int pos, int count, const HlStyle& ink)
{
	if(pos + count > v.GetCount())
		v.At(pos + count - 1, def);
	while(count-- > 0) {
		LineEdit::Highlight& x = v[pos++];
		x.ink = ink.color;
		if(ink.bold)
			x.font.Bold();
		if(ink.italic)
			x.font.Italic();
		if(ink.underline)
			x.font.Underline();
	}
}

void CodeEditor::HlSt::SetFont(int pos, int count, const HlStyle& f)
{
	if(pos + count > v.GetCount())
		v.At(pos + count - 1, def);
	while(count-- > 0) {
		LineEdit::Highlight& x = v[pos++];
		if(f.bold)
			x.font.Bold();
		if(f.italic)
			x.font.Italic();
		if(f.underline)
			x.font.Underline();
	}
}

void CodeEditor::HlSt::SetPaper(int pos, int count, Color paper)
{
	if(pos + count > v.GetCount())
		v.At(pos + count - 1, def);
	while(count-- > 0)
		v[pos++].paper = paper;
}

void CodeEditor::HlSt::SetInk(int pos, int count, Color ink)
{
	if(pos + count > v.GetCount())
		v.At(pos + count - 1, def);
	while(count-- > 0)
		v[pos++].ink = ink;
}

const wchar *CodeEditor::HlString(HlSt& hls, const wchar *p)
{
	hls.Put(hl_style[INK_CONST_STRING]);
	const wchar delim = *p;
	p++;
	while(*p && *p != delim)
		if(*p == '\\') {
			const wchar *t = p++;
			if(*p == 'x' || *p == 'X') {
				p++;
				if(IsXDigit(*p))
					p++;
				if(IsXDigit(*p))
					p++;
				hls.Put(int(p - t), hl_style[INK_CONST_STRINGOP]);
			}
			else
			if(*p >= '0' && *p <= '7') {
				p++;
				if(*p >= '0' && *p <= '7') p++;
				if(*p >= '0' && *p <= '7') p++;
				hls.Put(int(p - t), hl_style[INK_CONST_STRINGOP]);
			}
			else {
				hls.Put(hl_style[INK_CONST_STRINGOP]);
				if(*p) {
					hls.Put(hl_style[INK_CONST_STRINGOP]);
					p++;
				}
			}
		}
		else
		if(*p == '%')
			if(p[1] == '%') {
				hls.Put(2, hl_style[INK_CONST_STRING]);
				p += 2;
			}
			else {
				const wchar *t = p++;
				while(!IsAlpha(*p) && *p && *p != '`' && *p != '\"' && *p != '\'' && *p != '\\')
					p++;
				while(IsAlpha(*p) && *p)
					p++;
				hls.Put(int(p - t), hl_style[INK_CONST_STRINGOP]);
			}
		else {
			hls.Put(hl_style[INK_CONST_STRING]);
			p++;
		}
	if(*p == delim)	{
		hls.Put(hl_style[INK_CONST_STRING]);
		p++;
	}
	return p;
}

Color CodeEditor::BlockColor(int level)
{
	if(hilite_scope == 1)
		return  GetHlStyle(level & 1 ? PAPER_BLOCK1 : PAPER_NORMAL).color;
	if(hilite_scope == 2) {
		int q = level % 5;
		return  GetHlStyle(q ? PAPER_BLOCK1 + q - 1 : PAPER_NORMAL).color;
	}
	return GetHlStyle(PAPER_NORMAL).color;
}

void CodeEditor::Bracket(int pos, HlSt& hls)
{
	if(pos == highlight_bracket_pos0 && hilite_bracket
	   || pos == highlight_bracket_pos && (hilite_bracket == 1 || hilite_bracket == 2 && bracket_flash)) {
			HlStyle& h = hl_style[pos == highlight_bracket_pos0 ? PAPER_BRACKET0 : PAPER_BRACKET];
			hls.SetPaper(hls.pos, 1, h.color);
			hls.SetFont(hls.pos, 1, h);
	}
}

Vector <Index<String> > CodeEditor::keyword;
Vector <Index<String> > CodeEditor::name;
Index<String> CodeEditor::kw_upp;
int CodeEditor::kw_macros;
int CodeEditor::kw_logs;
int CodeEditor::kw_sql_base;
int CodeEditor::kw_sql_func;

const Index<String>& CodeEditor::CppKeywords()
{
	return keyword[0];
}

int CodeEditor::InitUpp(const char **q)
{
	while(*q)
		kw_upp.Add(*q++);
	return kw_upp.GetCount();
}

void CodeEditor::InitKeywords()
{
	static const char *cpp[] = {
		"namespace", "asm", "__asm", "else", "struct",
		"enum", "switch", "auto", "__except", "template",
		"explicit", "this",
		"bool", "extern", "mutable", "thread",
		"break", "false", "throw",
		"case", "__fastcall", "true",
		"catch", "__finally", "new", "try",
		"__cdecl", "float", "__try",
		"char", "wchar_t", "for", "operator", "typedef",
		"class", "friend", "private", "typeid",
		"const", "goto", "protected", "typename",
		"const_cast", "if", "public", "union",
		"continue", "inline", "register", "unsigned",
		"__declspec", "__inline", "reinterpret_cast", "using",
		"using", "default", "int", "return",
		"delete", "__int8", "short", "__uuidof",
		"dllexport", "__int16", "signed", "virtual",
		"dllimport", "__int32", "sizeof", "void",
		"do", "__int64", "static", "volatile",
		"double", "__leave", "static_cast",
		"dynamic_cast", "long", "__stdcall", "while",
		"force_inline",
		"and", "bitor", "or", "xor", 
		"compl", "bitand", "and_eq", "or_eq",
		"xor_eq", "not", "not_eq",
		"char16_t", "char32_t", "constexpr", "decltype",
		"noexcept", "nullptr", "static_assert",
		"override", "final",
		NULL
	};
	static const char *cs[] = {
		"abstract", "event", "new", "struct",
		"as", "explicit", "null", "switch",
		"base", "extern", "object", "this",
		"bool", "false", "operator", "throw",
		"break", "finally", "out", "true",
		"byte", "fixed", "override", "try",
		"case", "float", "params", "typeof",
		"catch", "for", "private", "uint",
		"char", "foreach", "protected", "ulong",
		"checked", "goto", "public", "unchecked",
		"class", "if", "readonly", "unsafe",
		"const", "implicit", "ref", "ushort",
		"continue", "in", "return", "using",
		"decimal", "int", "sbyte", "virtual",
		"default", "interface", "sealed", "volatile",
		"delegate", "internal", "short", "void",
		"do", "is", "sizeof", "while",
		"double", "lock", "stackalloc",
		"else", "long", "static",
		"enum", "namespace", "string",
		"await", "async", "throws", "awaits",
		NULL
	};
	static const char *upp[] = {
		"byte", "word", "dword", "__countof", "pick_", "wchar", "NULL", "Null",
		"int8", "uint8", "int16", "uint16", "int32", "uint32", "int64", "uint64", "qword",
		"INTERLOCKED_", "INTERLOCKED", "ONCELOCK", "ONCELOCK_", "INITBLOCK", "EXITBLOCK",
		"rval_", "rval_default",
		NULL
	};
	static const char *usc[] = {
		"void", "self", "if", "else", "while", "do", "case",
		"default", "break", "return", "switch", "operator", "for",
		"fn", "group", "ctrl", "subctrl", "template", "enum_property",
		"raw", "int", "double", "String", "bool",
		"Text", "Qtf", "Doc", "Font", "Color", "macro",
		NULL
	};
	static const char *usclib[] = {
		"Color", "Point", "Size", "Rect", "RectC",
		"StdFont", "Arial", "Roman", "Courier", "GetImageSize",
		"GetTextSize", "print", "Black", "Gray", "LtGray",
		"WhiteGray", "White", "Red", "Green", "Brown", "Blue",
		"Magenta", "Cyan", "Yellow", "LtRed", "LtGreen", "LtYellow",
		"LtBlue", "LtMagenta", "LtCyan", "SBlack", "SGray", "SLtGray",
		"SWhiteGray", "SWhite", "SRed", "SGreen", "SBrown", "SBlue",
		"SMagenta", "SCyan", "SYellow", "SLtRed", "SLtGreen", "SLtYellow",
		"SLtBlue", "SLtMagenta", "SLtCyan", "IntNull", "DblNullLim",
		"DrawRect", "DrawText", "DrawSmartText", "DrawImage", "is_number",
		"GetSmartTextSize", "GetQtfHeight", "DrawQtf",
		"is_array", "is_map", "is_void", "int", "to_string", "count",
		"len", "remove", "insert", "mid", "keys", "values", "exists",
		"erase", "rand", "OpenFileOut", "OpenFileAppend", "OpenFileIn",
		"GetMinSize", "GetStdSize",
		NULL
	};
	static const char *java[] = {
		"abstract", "assert", "boolean", "break", "byte", "case",
		"catch", "char", "class", "const", "continue",
		"default", "do", "double", "else", "enum", "extends",
		"false", "final", "finally", "float", "for",
		"goto", "if", "implements", "import", "instanceof",
		"int", "interface", "long", "native", "new",
		"null", "package", "private", "protected", "public",
		"return", "short", "static", "strictfp", "super", "switch",
		"synchronized", "this", "throw", "throws", "transient",
		"true", "try", "void", "volatile", "while",
		NULL
	};
	static const char *javascript[] = {
		"break", "continue", "do", "for", "import", "new", "this", "void",
		"case", "default", "else", "function", "in", "return", "typeof", "while",
		"comment", "delete", "export", "if", "label", "switch", "var", "with",
		"catch", "enum", "throw", "class", "extends", "try", "const", "finally",
		"debugger", "super", "true", "false", "undefined",
		NULL
	};
	static const char *css[] = {
		"azimuth", "background-attachment", "background-color", "background-image", "background-position",
		"background-repeat", "background", "border-collapse", "border-color", "border-spacing", "border-style",
		"border-top", "border-right", "border-bottom", "border-left", "border-top-color", "border-right-color",
		"border-bottom-color", "border-left-color", "border-top-style", "border-right-style", "border-bottom-style",
		"border-left-style", "border-top-width", "border-right-width", "border-bottom-width", "border-left-width",
		"border-width", "border", "bottom", "caption-side", "clear", "clip", "color", "content", "counter-increment",
		"counter-reset", "cue-after", "cue-before", "cue", "cursor", "direction", "display", "elevation", "empty-cells",
		"float", "font-family", "font-size", "font-style", "font-variant", "font-weight", "font", "height", "left",
		"letter-spacing", "line-height", "list-style-image", "list-style-position", "list-style-type", "list-style",
		"margin-right", "margin-left", "margin-top", "margin-bottom", "margin", "max-height", "max-width", "min-height",
		"min-width", "orphans", "outline-color", "outline-style", "outline-width", "outline", "overflow", "padding-top",
		"padding-right", "padding-bottom", "padding-left", "padding", "page-break-after", "page-break-before",
		"page-break-inside", "pause-after", "pause-before", "pause", "pitch-range", "pitch", "play-during", "position",
		"quotes", "richness", "right", "speak-header", "speak-numeral", "speak-punctuation", "speak", "speech-rate",
		"stress", "table-layout", "text-align", "text-decoration", "text-indent", "text-transform", "top",
		"unicode-bidi", "vertical-align", "visibility", "voice-family", "volume", "white-space", "widows", "width",
		"word-spacing", "z-index",
		NULL
	};
	static const char *cssn[] = {
		"em", "px", "pt",
		"left-side", "far-left", "left", "center-left", "center", "center-right", "right", "far-right", "right-side",
		"behind", "leftwards", "rightwards", "inherit", "scroll", "fixed", "transparent", "none", "top", "bottom",
		"repeat", "repeat-x", "repeat-y", "no-repeat", "background-color", "background-image", "background-repeat",
		"background-attachment", "background-position", "collapse", "separate", "border-top-color", "auto", "both",
		"normal", "attr", "open-quote", "close-quote", "no-open-quote", "no-close-quote", "cue-before", "cue-after",
		"crosshair", "default", "pointer", "move", "e-resize", "ne-resize", "nw-resize", "n-resize", "se-resize",
		"sw-resize", "s-resize", "w-resize", "text", "wait", "help", "progress", "ltr", "rtl", "inline", "block",
		"list-item", "inline-block", "table", "inline-table", "table-row-group", "table-header-group",
		"table-footer-group", "table-row", "table-column-group", "table-column", "table-cell", "table-caption",
		"below", "level", "above", "higher", "lower", "show", "hide", "italic", "oblique", "small-caps", "bold",
		"bolder", "lighter", "font-style",
		"font-variant", "font-weight", "font-size", "line-height", "font-family", "caption", "icon", "menu",
		"message-box", "small-caption", "status-bar", "inside", "outside", "disc", "circle", "square", "decimal",
		"decimal-leading-zero", "lower-roman", "upper-roman", "lower-greek", "lower-latin", "upper-latin", "armenian",
		"georgian", "lower-alpha", "upper-alpha", "list-style-type", "list-style-position", "list-style-image",
		"invert", "outline-color", "outline-style", "outline-width", "visible", "hidden", "always", "avoid",
		"x-low", "low", "medium", "high", "x-high", "mix", "static", "relative", "absolute", "once", "digits",
		"continuous", "code", "spell-out", "x-slow", "slow", "fast", "x-fast", "faster", "slower", "justify",
		"underline", "overline", "line-through", "blink", "capitalize", "uppercase", "lowercase", "embed",
		"bidi-override", "baseline", "sub", "super", "text-top", "middle", "text-bottom", "silent", "x-soft", "soft",
		"loud", "x-loud", "pre", "nowrap", "pre-wrap", "pre-line", 
		NULL
	};
	static const char *upp_macros[] = {
		"CLASSNAME", "THISBACK", "THISBACK1", "THISBACK2", "THISBACK3", "THISBACK4",
		"PTEBACK", "PTEBACK1", "PTEBACK2",  "PTEBACK3",  "PTEBACK4", 
		"QUOTE", "XASSERT", "NEVER", "XNEVER", "CHECK", "XCHECK", "ASSERT", "ASSERT_",
		"NAMESPACE_UPP", "END_UPP_NAMESPACE", "NEVER_", "SKYLARK", "RPC_METHOD", "RPC_GMETHOD",
		NULL
	};
	static const char *upp_logs[] = {
		"LOG", "LOGF", "DUMP", "DUMPC", "DUMPCC", "DUMPCCC", "DUMPM",
		"LLOG", "LLOGF", "LDUMP", "LDUMPC", "LDUMPCC", "LDUMPCCC", "LDUMPM",
		"DLOG", "DLOGF", "DDUMP", "DDUMPC", "DDUMPCC", "DDUMPCCC", "DDUMPM",
		"RLOG", "RLOGF", "RDUMP", "RDUMPC", "RDUMPCC", "RDUMPCCC", "RDUMPM",
		"LOGBEGIN", "LOGEND", "LOGBLOCK", "LOGHEXDUMP", "LOGSRCPOS", 
		"RLOGBEGIN", "RLOGEND", "RLOGBLOCK", "RLOGHEXDUMP", "RLOGSRCPOS", "RQUOTE",
		"RTIMING", "TIMING", "LTIMING", "DTIMING",
		"LOGHEX", "DUMPHEX", "DLOGHEX", "DDUMPHEX", "RLOGHEX", "RDUMPHEX", "LLOGHEX", "LDUMPHEX",
		"DEBUGCODE",			
		NULL
	};
	static const char *sql_base[] = {
		"Select", "Update", "Insert", "Delete", "From",
		"Join", "InnerJoin", "LeftJoin", "RightJoin", "FullJoin", "OuterJoin",
		"Where", "On", "OrderBy", "GroupBy",
		"Of", "As", "StartWith", "ConnectBy", "Having", "ForUpdate", "NoWait", "Limit",
		"Offset", "Hint", "SQL",
		NULL
	};
	static const char *sql_func[] = {
		"Decode", "Distinct", "All", "SqlAll", "Count", "Descending",
		"SqlMax", "SqlMin", "SqlSum", "Avg", "Stddev", "Variance",
		"Greatest", "Least", "ConvertCharset", "ConvertAscii",
		"Upper", "Lower", "Substr", "Instr", "Wild", "SqlDate", "AddMonths", "LastDay",
		"MonthsBetween", "NextDay", "SqlNvl", "Prior", "NextVal", "CurrVal", "SqlArg",
		NULL
	};
	static const char *sql_bool[] = {
		"SqlIsNull", "NotNull", "Like", "LikeUpperAscii", "NotLike", "Between",
		"NotBetween", "In", "NotIn", "Exists", "NotExists",
		NULL
	};
	static const char *tfile[] = {
		"T_",
		NULL,
	};
	static const char *tlng[] = {
		"enUS", "enGB", "enAU", "enCA", "enNZ", "enIE", "enZA", "enJM", "enCB", "enBZ",
		"enTT", "bgBG", "csCZ", "daDK", "deDE", "deCH", "deAT", "deLU", "deLI", "elGR",
		"esES", "esMX", "esES", "esGT", "esCR", "esPA", "esDO", "esVE", "esCO", "esPE",
		"esAR", "esEC", "esCL", "esUY", "esPY", "esBO", "esSV", "esHN", "esNI", "esPR",
		"fiFI", "frFR", "frBE", "frCA", "frCH", "frLU", "huHU", "isIS", "itIT", "itCH",
		"nlNL", "nlBE", "noNO", "noNO", "plPL", "ptBR", "ptPT", "roRO", "ruRU", "hrHR",
		"srSP", "srSP", "skSK", "svSE", "svFI", "trTR", "slSI", "afZA", "sqAL", "euES",
		"beBY", "caES", "etEE", "foFO", "idID", "lvLV", "ltLT", "ukUA", "zhCN", "zhTW",
		"koKR", "jaJP",
		NULL
	};
	static const char *lay[] = {
		"LAYOUT", "ITEM", "UNTYPED", "END_LAYOUT",
		NULL
	};
	static const char *sch[] = {
		"BIT", "BIT_ARRAY", "BIT_", "BIT_ARRAY_",
		"BOOL", "BOOL_ARRAY", "BOOL_", "BOOL_ARRAY_",
		"INT", "INT_ARRAY", "INT_", "INT_ARRAY_",
		"DOUBLE", "DOUBLE_ARRAY", "DOUBLE_", "DOUBLE_ARRAY_",
		"DATE", "DATE_ARRAY", "DATE_", "DATE_ARRAY_",
		"DATETIME", "DATETIME_ARRAY", "DATETIME_", "DATETIME_ARRAY_",
		"TIME", "TIME_ARRAY", "TIME_", "TIME_ARRAY_",
		"STRING", "STRING_ARRAY", "STRING_", "STRING_ARRAY_",
		"LONG", "LONG_", "LONGRAW", "LONGRAW_", "BLOB", "BLOB_", "CLOB", "CLOB_",
		"AUTO_INCREMENT", "KEY", "NOT_NULL", "TIMESTAMP", "COMMENT", "SEQUENCE", "SEQUENCE_",
		"PRIMARY_KEY", "INDEX", "PARTIAL_INDEX", "UNIQUE", "SQLDEFAULT", "REFERENCES", "REFERENCES_",
		"REFERENCES_CASCADE", "REFERENCES_CASCADE_", "DUAL_PRIMARY_KEY", "DUAL_UNIQUE",
		"UNIQUE_LIST", "SQLCHECK",
		"TABLE", "TABLE_", "END_TABLE", "TABLE_I", "TABLE_I_", "TABLE_II",
		"TABLE_II_", "TABLE_III", "TABLE_III_", "VAR", "VAR_",
		"COLUMN", "COLUMN_ARRAY", "ATTRIBUTE", "INLINE_ATTRIBUTE",
		"TYPE", "TYPE_I", "TYPE_II", "TYPE_III", "END_TYPE",
		"TYPE_", "TYPE_I_", "TYPE_II_", "TYPE_III_", "SERIAL", "ISERIAL",
		"INDEX_LIST",
		NULL
	};
	static const char *sql[] = {
		"ABORT", "ADD", "AFTER", "ALL", "ALTER", "ANALYZE", "AND", "AS", "ATTACH",
		"AUTOINCREMENT", "BEFORE", "BEGIN", "BETWEEN", "BY", "CASCADE", "CASE", "CHECK",
		"COLLATE", "COLUMN", "COMMIT", "CONFLICT", "CONSTRAINT", "CREATE", "CROSS",
		"CURRENT_DATE", "CURRENT_TIME", "CURRENT_TIMESTAMP", "DATABASE", "DEFAULT",
		"DELETE", "DESC", "DETACH", "DISTINCT", "DROP", "EACH", "END", "EXCEPT",
		"EXCLUSIVE", "EXISTS", "FOREIGN", "FROM", "FULL", "GROUP", "HAVING",
		"IN", "INDEX", "INITIALLY", "INNER", "INSERT", "INSTEAD", "INTERSECT", "INTO",
		"IS", "ISNULL", "JOIN", "KEY", "LEFT", "LIKE", "LIMIT", "MATCH", "NATURAL", "NOT",
		"NOTNULL", "NULL", "OF", "OFFSET", "ON", "OR", "ORDER", "OUTER", "PRIMARY", "RAISE",
		"REFERENCES", "REPLACE", "RESTRICT", "UPDATE", "SET", "WHERE", "NEW", "OLD", "TRIGGER",
		"SELECT", "TABLE",
		NULL
	};
	static const char *empty[] = {
		NULL
	};
	static const char *javascriptn[] = {
		"alert", "eval", "toString", "valueOf", "length",
		NULL
	};

	LoadSyntax(cpp, upp); // Order here is important, must be the same as enum
	LoadSyntax(usc, usclib);
	LoadSyntax(java, empty);
	LoadSyntax(tfile, tlng);
	LoadSyntax(usc, usclib);
	LoadSyntax(lay, empty);
	LoadSyntax(sch, empty);
	LoadSyntax(sql, empty);
	LoadSyntax(cs, empty);
	LoadSyntax(javascript, javascriptn);
	LoadSyntax(css, cssn);
	LoadSyntax(empty, empty);

	kw_macros = InitUpp(upp_macros);
	kw_logs = InitUpp(upp_logs);
	kw_sql_base = InitUpp(sql_base);
	kw_sql_func = InitUpp(sql_func);
	InitUpp(sql_bool);
}

int CodeEditor::LoadSyntax(const char *keywords[], const char *names[])	// Changed
{
	Index <String> &key = keyword.Add()	;
	while(*keywords)
		key.Add(*keywords++);
	Index <String> &nam = name.Add();
	while(*names)
		nam.Add(*names++);	
	return keyword.GetCount() - 1;
}

void CodeEditor::HighlightLine(int line, Vector<LineEdit::Highlight>& hl, int pos)
{
	LTIMING("HighlightLine");
	if(highlight < 0 || highlight >= keyword.GetCount())
		return;
	WString text = GetWLine(line);
	SyntaxState ss = ScanSyntax(line);
	ss.Grounding(text.Begin(), text.End());
	SyntaxState sm = ScanSyntax(line + 1);
	HlSt hls(hl);
	const wchar *p = text;
	const wchar *e = text.End();
	int uvsn;
	GetUvsHighlight(text, uvsn);
	if(uvsn) {
		hls.SetInk(0, text.GetLength() + 1, Yellow);
		hls.SetPaper(0, text.GetLength() + 1, Gray);
		hls.pos += uvsn;
		p += uvsn;
		return;
	}
	else
	if(highlight == HIGHLIGHT_CALC) {
		if(line == GetLineCount() - 1 || *p == '$')
			hls.SetPaper(0, text.GetLength() + 1, hl_style[PAPER_BLOCK1].color);
	}
	else
		hls.SetPaper(0, text.GetLength() + 1,
		             Nvl(ss.uvscolor, sm.macro ? hl_style[PAPER_MACRO].color : hl_style[PAPER_NORMAL].color));
	int block_level = ss.bid.GetCount() - 1;
	String cppid;
	if(IsNull(ss.uvscolor) && !uvsn && !ss.comment && highlight != HIGHLIGHT_CALC) {
		if(!sm.macro) {
			int i = 0, bid = 0, pos = 0, tabsz = GetTabSize();
			while(bid < ss.bid.GetCount() - 1
			&& (i >= text.GetLength() || text[i] == ' ' || text[i] == '\t')) {
				hls.SetPaper(i, 1, BlockColor(bid));
				if(i < text.GetLength() && text[i] == '\t' || ++pos >= tabsz) {
					pos = 0;
					bid++;
				}
				i++;
			}
			hls.SetPaper(i, 1 + max(0, text.GetLength() - i), BlockColor(ss.bid.GetCount() - 1));
		}
		while(*p == ' ' || *p == '\t') {
			p++;
			hls.Put(hl_style[INK_NORMAL]);
		}
		if(*p == '#' && findarg(highlight, HIGHLIGHT_JAVASCRIPT, HIGHLIGHT_CSS, HIGHLIGHT_JSON) < 0) {
			static const char *pd[] = {
				"define", "error", "if", "elif", "else", "endif",
				"ifdef", "ifndef", "include", "line", "undef", "pragma",
				// CLR
				"using"
			};
			static Index<String> macro;
			if(macro.GetCount() == 0)
				for(int i = 0; i < __countof(pd); i++)
					macro.Add(pd[i]);
			const wchar *q = p + 1;
			while(*q == ' ' || *q == '\t')
				q++;
			StringBuffer id;
			while(IsAlpha(*q))
				id.Cat(*q++);
			cppid = id;
			hls.Put(macro.Find(cppid) < 0 ? 1 : int(q - p), hl_style[INK_MACRO]);
			p = q;
		}
	}
	int lindent = int(p - ~text);
	int lbrace = -1;
	int lbclose = -1;
	Color lbcolor = Null;
	bool is_comment = false;
	while(p < e) {
		int pair = MAKELONG(p[0], p[1]);
		if(ss.linecomment && ss.linecont || pair == MAKELONG('/', '/') && highlight != HIGHLIGHT_CSS && highlight != HIGHLIGHT_JSON) {
			hls.Put(text.GetLength() + 1 - hls.pos, hl_style[INK_COMMENT]);
			is_comment = true;
			break;
		}
		else
		if(ss.comment && highlight != HIGHLIGHT_JSON)
			if(pair == MAKELONG('*', '/')) {
				hls.Put(2, hl_style[INK_COMMENT]);
				p += 2;
				ss.comment = false;
			}
			else {
				hls.Put(hl_style[INK_COMMENT]);
				p++;
			}
		else
		if((*p == '\"' || *p == '\'') || ss.linecont && ss.string)
			p = HlString(hls, p);
		else
		if(pair == MAKELONG('/', '*') && highlight != HIGHLIGHT_JSON) {
			hls.Put(2, hl_style[INK_COMMENT]);
			p += 2;
			ss.comment = true;
			is_comment = true;
		}
		else
		if(*p == '(') {
			ss.brk.Add(')');
			Bracket(int(p - text) + pos, hls);
			hls.Put(hl_style[INK_PAR0 + max(ss.pl++, 0) % 4]);
			p++;
		}
		else
		if(*p == '{') {
			ss.brk.Add(ss.was_namespace ? 0 : '}');
			Bracket(int(p - text) + pos, hls);
			hls.Put(hl_style[INK_PAR0 + max(ss.cl, 0) % 4]);
			if(ss.was_namespace)
				ss.was_namespace = false;
			else {
				++block_level;
				++ss.cl;
			}
			if(hls.pos < text.GetCount())
				hls.SetPaper(hls.pos, text.GetCount() - hls.pos + 1, BlockColor(block_level));
			p++;
		}
		else
		if(*p == '[') {
			ss.brk.Add(']');
			Bracket(int(p - text) + pos, hls);
			hls.Put(hl_style[INK_PAR0 + max(ss.bl++, 0) % 4]);
			p++;
		}
		else
		if(*p == ')' || *p == '}' || *p == ']') {
			int bl = ss.brk.GetCount();
			int bc = bl ? ss.brk.Pop() : 0;
			if(*p == '}' && hilite_scope && block_level > 0 && bc)
				hls.SetPaper(hls.pos, text.GetLength() + 1 - hls.pos, BlockColor(--block_level));
			Bracket(int(p - text) + pos, hls);
			int& l = *p == ')' ? ss.pl : *p == '}' ? ss.cl : ss.bl;
			if(bc && (bc != *p || l <= 0) || bc == 0 && *p != '}') {
				hls.Put(p == ~text ? hl_style[INK_PAR0] : hl_style[INK_ERROR]);
				ss.brk.Clear();
				ss.cl = ss.bl = ss.pl = 0;
			}
			else {
				if(bc) --l;
				hls.Put(1, hl_style[INK_PAR0 + l % 4]);
			}
			p++;
		}
		else
		if(highlight == HIGHLIGHT_CSS ? *p == '#' : pair == MAKELONG('0', 'x') || pair == MAKELONG('0', 'X')) {
			hls.Put(2, hl_style[INK_CONST_HEX]);
			p += 2;
			const wchar *t = p;
			while(IsXDigit(*p))
				p++;
			hls.Put(int(p - t), hl_style[INK_CONST_HEX]);
		}
		else
		if(IsDigit(*p)) {
			const wchar *t = p;
			int c = INK_CONST_INT;
			bool isDot, isFloat = false;
			if(*p == '0') c = INK_CONST_OCT;
			while(IsDigit(*p)) p++;
			int fixdigits = p - t;
			if(*p == '.' || (*p == 'e' || *p == 'E') && highlight != HIGHLIGHT_CSS) {
				if(*p == '.')
					isDot = true;
				c = INK_CONST_FLOAT;
				p++;
				if(*p == '-')
					p++;
			}
			while((IsDigit(*p) || (isDot && *p == 'f')) && (isFloat == false)) {
				if(*p == 'f')
					isFloat = true;
				p++;
			}
			if(c == INK_CONST_OCT && p - t == 1)
				c = INK_CONST_INT;
			int n = int(p - t);
			for(int i = 0; i < n; i++)
				hls.Put(hl_style[c],
				        c == INK_CONST_OCT || (fixdigits < 4 && n - fixdigits < 5) || i == fixdigits || !thousands_separator ? 0 :
				        i < fixdigits ? decode((fixdigits - i) % 3, 1, LineEdit::SHIFT_L, 0, LineEdit::SHIFT_R, 0) :
				                        decode((i - fixdigits) % 3, 1, LineEdit::SHIFT_R, 0, LineEdit::SHIFT_L, 0));

//				hls.Put(q > 0 && ((q / 3) & 1) == 1 && c != INK_CONST_OCT ?
//				        hl_style[c == INK_CONST_INT ? INK_CONST_INT_3 : INK_CONST_FLOAT_3]
//				        : hl_style[c]);
		}
		else
		if(pair == MAKELONG('t', '_') && p[2] == '(' && p[3] == '\"') {
			int pos0 = hls.pos;
			hls.Put(3, hl_style[INK_UPP]);
			p = HlString(hls, p + 3);
			if(*p == ')') {
				hls.Put(hl_style[INK_UPP]);
				p++;
			}
			hls.SetPaper(pos0, hls.pos - pos0, hl_style[PAPER_LNG].color);
		}
		else
		if(pair == MAKELONG('t', 't') && p[3] == '(' && p[4] == '\"') {
			int pos0 = hls.pos;
			hls.Put(4, hl_style[INK_UPP]);
			p = HlString(hls, p + 4);
			if(*p == ')') {
				hls.Put(hl_style[INK_UPP]);
				p++;
			}
			hls.SetPaper(pos0, hls.pos - pos0, hl_style[PAPER_LNG].color);
		}
		else
		if(iscib(*p)) {
			const wchar *q = p;
			StringBuffer id;
			while((iscidl(*q) || highlight == HIGHLIGHT_CSS && *q == '-') && q < e)
				id.Cat(*q++);
			String iid = id;
			if(highlight == HIGHLIGHT_SQL)
				iid = ToUpper(iid);
			int uq = kw_upp.Find(iid);
			int nq;
			hls.Put(int(q - p), (nq = keyword[highlight].Find(iid)) >= 0 ? hl_style[INK_KEYWORD] :
			                    name[highlight].Find(iid) >= 0 ? hl_style[INK_UPP] :
			                    uq >= 0 ? uq < kw_macros ? hl_style[INK_UPPMACROS] :
			                              uq < kw_logs ? hl_style[INK_UPPLOGS] :
			                              uq < kw_sql_base ? hl_style[INK_SQLBASE] : 
			                              uq < kw_sql_func ? hl_style[INK_SQLFUNC] :
			                              hl_style[INK_SQLBOOL] :
			                    IsUpperString(iid) && !sm.macro ? hl_style[INK_UPPER] :
			                    hl_style[INK_NORMAL]);
			p = q;
			if(nq == 0)
				ss.was_namespace = true;
		}
		else {
			if(*p == ';')
				ss.was_namespace = false;
			hls.Put(strchr("!+-*^/%~&|=[]:?<>.", *p) ? hl_style[INK_OPERATOR] : hl_style[INK_NORMAL]);
			p++;
		}
	}
	if(hilite_ifdef && !IsNull(cppid) && !is_comment) {
		if((cppid == "else" || cppid == "elif" || cppid == "endif") && !ss.ifstack.IsEmpty()) {
			WStringBuffer ifln;
			ifln.Cat(" // ");
			ifln.Cat(ss.ifstack.Top().iftext);
			if(ss.ifstack.Top().ifline && hilite_ifdef == 2) {
				ifln.Cat(", line ");
				ifln.Cat(FormatInt(ss.ifstack.Top().ifline));
			}
			ifln.Cat('\t');
			int start = text.GetLength();
			WString ifs(ifln);
			hls.Set(start, ifs.GetLength(), hl_style[INK_IFDEF]);
			for(int i = 0; i < ifs.GetCount(); i++)
				hl[start + i].chr = ifs[i];
		}
	}
	if(hilite_scope) {
		if(lbrace >= 0 && lbclose < 0 && lbrace > lindent)
			hls.SetPaper(lindent, lbrace - lindent, lbcolor);
		if(lbrace < 0 && lbclose >= 0)
			hls.SetPaper(lbclose, text.GetLength() + 1 - lbclose, lbcolor);
	}
	if(!IsNull(cppid) && (cppid == "else" || cppid == "elif" || cppid == "endif" || cppid == "if"
	                      || cppid == "ifdef" || cppid == "ifndef"))
	   hls.SetPaper(0, hls.v.GetCount(), hl_style[PAPER_IFDEF].color);
}

INITBLOCK {
	CodeEditor::InitKeywords();
}

END_UPP_NAMESPACE
