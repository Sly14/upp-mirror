#ifndef IDEEDITOR_H
#define IDEEDITOR_H

//#include <ide/Common/Common.h>
#include <CtrlLib/CtrlLib.h>


NAMESPACE_UPP

#define  LAYOUTFILE <CodeEditor/CodeEditor.lay>
#include <CtrlCore/lay.h>

#define IMAGEVECTOR Vector
#define IMAGECLASS  CodeEditorImg
#define IMAGEFILE   <CodeEditor/CodeEditor.iml>
#include <Draw/iml_header.h>

void FindWildcardMenu(Callback1<int> cb, Point p, bool tablf, Ctrl *owner = NULL);

struct LineInfoRecord {
	int    lineno;
	String breakpoint;
	int    count;
	int    error;
	int    firstedited;
	int    edited;
	
	LineInfoRecord() { error = 0; edited = 0; }
};

typedef Array<LineInfoRecord> LineInfo;

struct LineInfoRemRecord : Moveable<LineInfoRemRecord> {
	int    firstedited;
	int    edited;
};
typedef Vector<LineInfoRemRecord> LineInfoRem;

void Renumber(LineInfo& lf);
void ClearBreakpoints(LineInfo& lf);
void ValidateBreakpoints(LineInfo& lf);

class CodeEditor;

class EditorBar : public FrameLeft<Ctrl> {
public:
	virtual void Paint(Draw& w);
	virtual void MouseMove(Point p, dword flags);
	virtual void LeftDown(Point p, dword flags);
	virtual void LeftDouble(Point p, dword flags);
	virtual void RightDown(Point p, dword flags);

private:
	struct LnInfo : Moveable<LnInfo> {
		int    lineno;
		String breakpoint;
		int    error;
		int    firstedited;
		int    edited;

		LnInfo() { lineno = -1; error = 0; firstedited = 0; edited = 0; }
	};
	Vector<LnInfo>   li;
	LineInfoRem      li_removed;
	
	int              sy;
	CodeEditor       *editor;
	int              ptrline[2];
	Image            ptrimg[2];
	bool             bingenabled;
	bool             hilite_if_endif;
	bool             line_numbers;
	bool             ignored_next_edit;
	int              next_age;

	String& PointBreak(int& y);
	void    sPaintImage(Draw& w, int y, int fy, const Image& img);

public:
	Callback1<int> WhenBreakpoint;

	void InsertLines(int i, int count);
	void RemoveLines(int i, int count);
	void ClearLines();

	void Scroll()                          { Refresh(); }

	void Renumber(int linecount);
	void ClearBreakpoints();
	void ValidateBreakpoints();

	String  GetBreakpoint(int ln);
	void    SetBreakpoint(int ln, const String& s);
	void    SetEdited(int ln, int count = 1);
	void    ClearEdited();
	void    SetError(int ln, int err);
	void    ClearErrors(int ln);

	void SetEditor(CodeEditor *e)           { editor = e; }

	LineInfo GetLineInfo() const;
	void     SetLineInfo(const LineInfo& li, int total);
	LineInfoRem & GetLineInfoRem()             { return li_removed; }
	void     SetLineInfoRem(LineInfoRem& li)   { li_removed = li; }

	int      GetLineNo(int lineno) const;
	int      GetNoLine(int line) const;

	void     SetPtr(int line, const Image& img, int i);
	void     HidePtr();

	void     EnableBreakpointing(bool b)   { bingenabled = b; }
	void     HiliteIfEndif(bool b)         { hilite_if_endif = b; Refresh(); }
	void     LineNumbers(bool b);

	bool     IsHiliteIfEndif() const       { return hilite_if_endif; }

	EditorBar();
	virtual ~EditorBar();
};

struct IdentPos {
	int    begin;
	int    end;
	String ident;
};

Array<IdentPos> GetLineIdent(const char *line);
Vector<Point>   GetLineString(const wchar *wline, bool& is_begin, bool& is_end);

inline int  CharFilterCIdent(int i)  { return IsAlNum(i) || i == '_' ? i : 0; }
inline bool islbrkt(int c)           { return c == '{' || c == '[' || c == '('; }
inline bool isrbrkt(int c)           { return c == '}' || c == ']' || c == ')'; }
inline bool isbrkt(int c)            { return islbrkt(c) || isrbrkt(c); }

class CodeEditor : public LineEdit {
	friend class EditorBar;

public:
	virtual bool Key(dword code, int count);
	virtual void LeftDown(Point p, dword keyflags);
	virtual void LeftDouble(Point p, dword keyflags);
	virtual void MouseMove(Point p, dword keyflags);
	virtual void Serialize(Stream& s);
	void         CheckEdited(bool e = true) { check_edited = e; }
	bool         GetCheckEdited()           { return check_edited; }

protected:
	virtual void HighlightLine(int line, Vector<Highlight>& h, int pos);
	virtual void PreInsert(int pos, const WString& s);
	virtual void PostInsert(int pos, const WString& s);
	virtual void PreRemove(int pos, int size);
	virtual void PostRemove(int pos, int size);
	virtual void DirtyFrom(int line);
	virtual void SelectionChanged();

	virtual void ClearLines();
	virtual void InsertLines(int line, int count);
	virtual void RemoveLines(int line, int count);

	virtual void NewScrollPos();

	EditorBar bar;

	struct Isx : Moveable<Isx> {
		int    line;
		int    pos;

		friend bool operator==(Isx a, Isx b) { return a.line == b.line && a.pos == b.pos; }
		friend bool operator!=(Isx a, Isx b) { return !(a == b); }
	};

	struct IfState : Moveable<IfState> {
		enum        { IF = '0', ELIF, ELSE, ELSE_ERROR, ENDIF_ERROR };
		WString iftext;
		short   ifline;
		char    state;

		bool operator==(const IfState& b) const {
			return iftext == b.iftext && state == b.state && ifline == b.ifline;
		}

		IfState()                         { ifline = state = 0; }
	};

	struct SyntaxState {
		int         line;

		bool        comment;
		bool        linecomment;
		bool        string;
		bool        linecont;
		char        macro;
		enum        { MACRO_OFF, MACRO_CONT, MACRO_END };

		int         cl, bl, pl;

		WithDeepCopy< Vector<int> > brk;
		WithDeepCopy< Vector<int> > blk;
		WithDeepCopy< Vector<int> > bid;
		WithDeepCopy< Vector<Isx> > par;
		WithDeepCopy< Vector<IfState> > ifstack;

		int         stmtline;
		int         endstmtline;
		int         seline;
		int         spar;
		Color       uvscolor;

		void  DropItem(int type);
		bool  Drop(int type);
		void  Clear();
		bool  MatchHilite(const SyntaxState& st) const;
		void  ScanSyntax(const wchar *ln, const wchar *e);

		static Color IfColor(char ifstate);

		SyntaxState()                         { Clear(); }
	};

	friend struct SyntaxState;

	SyntaxState scache[4];
	SyntaxState rm_ins;
	char        rmb;
	byte        hilite_bracket;
	int         highlight_bracket_pos0;
	int         highlight_bracket_pos;
	bool        bracket_flash;
	int         bracket_start;

	byte    hilite_scope;
	byte    hilite_ifdef;
	bool    indent_spaces : 1;
	bool    no_parenthesis_indent : 1;
	int     indent_amount;
	double  stat_edit_time;
	Time    last_key_time;

	bool    auto_enclose;
	bool    mark_lines;
	bool    check_edited;

	struct FindReplace : WithIDEFindReplaceLayout<TopWindow> {
		WString itext;
		virtual bool Key(dword key, int count);
	} findreplace;
	FrameRight<Button> findwb;
	FrameRight<Button> replacewb;
	FrameRight<Button> findI;
	FrameRight<Button> replaceI;

	enum {
		WILDANY = 16,
		WILDONE,
		WILDSPACE,
		WILDNUMBER,
		WILDID,
	};

	struct Found {
		int     type;
		WString text;
	};

	Array<Found> foundwild;
	bool   foundsel;
	bool   found, notfoundfw, notfoundbk;

	int    iwc;

	int    highlight;

	struct HlSt;

	const wchar *HlString(HlSt& hls, const wchar *p);

	SyntaxState ScanSyntax(int line);

	void   CancelBracketHighlight(int& pos);
	void   CheckBracket(int li, int pos, int ppos, int pos0, WString ln, int d, int limit);
	void   CheckLeftBracket(int pos);
	void   CheckRightBracket(int pos);
	void   CheckBrackets();
	void   OpenNormalFindReplace();
	void   FindReplaceAddHistory();
	void   FindWildcard();
	void   ReplaceWildcard();
	void   InsertWildcard(int c);

	void   SetFound(int fi, int type, const WString& text);

	int     Match(const wchar *f, const wchar *s, int line, bool we, bool icase, int fi = 0);
	WString GetWild(int type, int& i);
	WString GetReplaceText();
	WString GetReplaceText(WString replace, bool wildcards);

	bool   InsertRS(int chr, int count = 1);
	void   IndentEnter(int count = 1);
	void   SyntaxIndent(const SyntaxState& st, bool ndnt);
	void   IndentInsert(int chr);

	void   ForwardWhenBreakpoint(int i);

	Color  BlockColor(int level);
	void   Bracket(int pos, HlSt& hls);

	bool   ToggleSimpleComment(int &start_line, int &end_line, bool usestars = true);
	void   ToggleLineComments(bool usestars = false);
	void   ToggleStarComments();
	void   Enclose(const char *c1, const char *c2, int l = -1, int h = -1);

	enum {
		TIMEID_PERIODIC = Ctrl::TIMEID_COUNT,
		TIMEID_COUNT,
	};

	void   Periodic();

public:
	enum {
		HIGHLIGHT_NONE = -1, HIGHLIGHT_CPP = 0, HIGHLIGHT_USC, HIGHLIGHT_JAVA, HIGHLIGHT_T, HIGHLIGHT_CALC,
		HIGHLIGHT_COUNT
	};

#define HL_COLOR(x, a, b)      x,
	enum {
#include "hl_color.i"
		HL_COUNT
	};
#undef HL_COLOR

	struct HlStyle {
		Color color;
		bool  bold;
		bool  italic;
		bool  underline;
	};

private:
	HlStyle  hl_style[HL_COUNT];

public:
	Callback WhenSelection;
	Callback1<const String&> WhenDbgView;
	Callback WhenLeftDown;

	FrameTop<Button>    topsbbutton;
	FrameTop<Button>    topsbbutton1;

	void   Clear()                    { LineEdit::Clear(); found = notfoundfw = notfoundbk = false; }

	void   Highlight(int h)           { highlight = h; Refresh(); }
	int    GetHighlight() const       { return highlight; }

	const HlStyle& GetHlStyle(int i);
	void           SetHlStyle(int i, Color c, bool bold = false, bool italic = false, bool underline = false);

	void   CloseFindReplace();
	void   FindReplace(bool pick_text);
	bool   Find(bool back, const wchar *text, bool wholeword, bool ignorecase, bool wildcards,
	            bool block);
	bool   Find(bool back = false, bool blockreplace = false);
	bool   GetStringRange(int cursor, int& b, int &e) const;
	bool   GetStringRange(int& b, int &e) const { return GetStringRange(GetCursor(), b, e); }
	bool   FindString(bool back);
	bool   FindLangString(bool back);
	void   Replace();
	void   BlockReplace();
	int    BlockReplace(WString find, WString replace, bool wholeword, bool ignorecase, bool wildcards);
	void   MakeTabs();
	void   MakeTabsOrSpaces(bool tabs);

	void   CopyWord();
	void   SwapChars();

	void   SerializeFind(Stream& s);
	bool   IsFindOpen() const                       { return findreplace.IsOpen(); }
	void   FindClose()                              { findreplace.Close(); }

	void   Goto();

	void   DoFind();
	void   DoFindBack();

	void    FindWord(bool back);
	WString GetI();
	void    SetI(Ctrl *edit);
	void    PutI(Ctrl& edit, FrameRight<Button>& I);

	void   MoveNextWord(bool sel);
	void   MovePrevWord(bool sel);
	void   MoveNextBrk(bool sel);
	void   MovePrevBrk(bool sel);

	String GetWord();

	bool   GetWordPos(int pos, int& l, int& h);

	void   DeleteWord();
	void   DeleteWordBack();
	void   SetLineSelection(int l, int h);
	bool   GetLineSelection(int& l, int& h);
	void   TabRight();
	void   TabLeft();

	Callback1<int> WhenBreakpoint;

	LineInfo GetLineInfo() const                      { return bar.GetLineInfo(); }
	void     SetLineInfo(const LineInfo& lf);
	LineInfoRem GetLineInfoRem()                      { return LineInfoRem(bar.GetLineInfoRem(), 0); }
	void     SetLineInfoRem(LineInfoRem& lf)          { bar.SetLineInfoRem(LineInfoRem(lf, 0)); }
	double   GetStatEditTime() const                  { return stat_edit_time; }
	void     Renumber()                               { bar.Renumber(GetLineCount()); }
	void     ClearBreakpoints()                       { bar.ClearBreakpoints(); }
	void     ValidateBreakpoints()                    { bar.ValidateBreakpoints(); }
	int      GetLineNo(int line) const                { return bar.GetLineNo(line); }
	int      GetNoLine(int line) const                { return bar.GetNoLine(line); }
	void     SetPtr(int line, const Image& img, int i){ bar.SetPtr(line, img, i); }
	void     HidePtr()                                { bar.HidePtr(); }
	String   GetBreakpoint(int line)                  { return bar.GetBreakpoint(line); }
	void     SetBreakpoint(int line, const String& b) { bar.SetBreakpoint(line, b); }
	void     SetError(int line, int err)              { bar.SetError(line, err); }
	void     ClearErrors(int line = -1)               { bar.ClearErrors(line); }
	void     ClearEdited()                            { bar.ClearEdited(); }
	int		 GetUndoCount()                           { return undo.GetCount(); }
	void     GotoLine(int line);
	void     EnableBreakpointing()                    { bar.EnableBreakpointing(true); }
	void     DisableBreakpointing()                   { bar.EnableBreakpointing(false); }

	void     HiliteScope(byte b)                      { hilite_scope = b; Refresh(); }
	void     HiliteBracket(byte b)                    { hilite_bracket = b; Refresh(); }
	void     HiliteIfDef(byte b)                      { hilite_ifdef = b; Refresh(); }
	void     HiliteIfEndif(bool b)                    { bar.HiliteIfEndif(b); }
	void     IndentSpaces(bool is)                    { indent_spaces = is; }
	void     IndentAmount(int ia)                     { indent_amount = ia; }
	void     NoParenthesisIndent(bool b)              { no_parenthesis_indent = b; }
	void     LineNumbers(bool b)                      { bar.LineNumbers(b); }
	void     MarkLines(bool b)                        { mark_lines = b; }
	bool     GetMarkLines()                           { return mark_lines; }
	void     AutoEnclose(bool b)                      { auto_enclose = b; }

	void     HideBar()                                { bar.Hide(); }

	void     DefaultHlStyles();
	void     LoadHlStyles(const char *s);
	String   StoreHlStyles();

	const char *GetHlName(int i);
	bool        HasHlFont(int i);

	typedef CodeEditor CLASSNAME;

	CodeEditor();
	virtual ~CodeEditor();
};

END_UPP_NAMESPACE

#endif
