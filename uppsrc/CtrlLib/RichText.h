class RichTextView : public Ctrl {
public:
	virtual void  Paint(Draw& w);
	virtual bool  Key(dword key, int count);
	virtual void  Layout();
	virtual Value GetData() const;
	virtual void  SetData(const Value& v);
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual Image CursorImage(Point p, dword keyflags);
	virtual void  LeftDown(Point p, dword keyflags);
	virtual void  MouseMove(Point p, dword keyflags);
	virtual void  LeftRepeat(Point p, dword keyflags);
	virtual void  RightDown(Point p, dword keyflags);

private:
	Rect          margin;
	Color         background;
	Zoom          zoom;
	int           cx;
	ScrollBar     sb;
	Scroller      scroller;
	RichText      text;
	bool          sizetracking;
	bool          vcenter;
	bool          hldec;
	int           highlight;
	int           sell, selh;
	int           cursor, anchor;

	void          EndSizeTracking();
	void          SetSb();
	void          Scroll();
	Zoom          GetZoom() const;
	int           GetPageCx(bool reduced = false) const;
	int           GetPointPos(Point p) const;
	String        GetLink(int pos, Point p) const;
	Rect          GetPage() const;
	void          RefreshSel();
	void          RefreshRange(int a, int b);

protected:
	enum {
		TIMEID_ENDSIZETRACKING = Ctrl::TIMEID_COUNT,
		TIMEID_COUNT
	};

public:
	Callback1<const String&> WhenLink;

	void            Clear();
	void            Pick(pick_ RichText& t);
	void            Pick(pick_ RichText& txt, Zoom z);
	void            SetQTF(const char *qtf, Zoom z = Zoom(1, 1), bool scolors = false);
	const RichText& Get() const                               { return text; }
	String          GetQTF(byte cs = CHARSET_UTF8) const      { return AsQTF(text, cs); }

	int             GetWidth() const                          { return text.GetWidth(); }
	int             GetHeight(int cx) const                   { return text.GetHeight(Zoom(1, 1), cx); }

	int             GetSb() const                             { return sb; }
	void            SetSb(int i)                              { sb = i; }
	int             GetSbTotal() const                        { return sb.GetTotal(); }

	void            GotoLabel(const String& lbl, bool highlight = false);
	void            ClearHighlight()                          { highlight = Null; Refresh(); }

	int             GetLength() const                         { return text.GetLength(); }

	void            Copy();

	RichTextView&   PageWidth(int cx);
	RichTextView&   SetZoom(Zoom z);
	RichTextView&   Background(Color _color);
	RichTextView&   VCenter(bool b = true);
	RichTextView&   NoVCenter()                               { return VCenter(false); }
	RichTextView&   Margins(const Rect& m);
	RichTextView&   HMargins(int a);
	RichTextView&   VMargins(int a);
	RichTextView&   Margins(int a);
	RichTextView&   NoSb(bool b = true)                       { sb.Hide(); return *this; }
	RichTextView&   AutoHideSb(bool b = true)                 { sb.AutoHide(b); return *this; }
	RichTextView&   HyperlinkDecoration(bool b = true)        { hldec = b; Refresh(); return *this; }
	RichTextView&   NoHyperlinkDecoration()                   { return HyperlinkDecoration(false); }

	void            operator=(const char *qtf)                { SetQTF(qtf); }

	typedef RichTextView CLASSNAME;

	RichTextView();
	virtual ~RichTextView();
};

class RichTextCtrl : public RichTextView {
public:
	void            SetQTF(const char *qtf, Zoom z = GetRichTextStdScreenZoom()) { RichTextView::SetQTF(qtf, z, true); }
	void            operator=(const char *qtf)                                   { SetQTF(qtf); }
	RichTextCtrl();
};

int Prompt(const char *title, const Image& icon, const char *qtf, bool okcancel,
           const char *button1, const char *button2 = NULL, const char *button3 = NULL,
		   int cx = 0);

int Prompt(const char *title, const Image& icon, const char *qtf,
           const char *button1, const char *button2 = NULL, const char *button3 = NULL,
		   int cx = 0);

void PromptOK(const char *qtf);
int  PromptOKCancel(const char *qtf);
int  PromptYesNo(const char *qtf);
int  PromptYesNoCancel(const char *qtf);
int  PromptRetryCancel(const char *qtf);
int  PromptAbortRetry(const char *qtf);
int  PromptAbortRetryIgnore(const char *qtf);

void Exclamation(const char *qtf);

void ShowExc(const Exc& exc);

class HelpWindow : public TopWindow {
public:
	virtual bool Key(dword key, int);

private:
	RichTextView   view;
	TreeCtrl       tree;
	Splitter       tree_view;
	ToolBar        toolbar;
	Zoom           zoom;

	struct Pos : Moveable<Pos> {
		String link;
		int    scy;
	};

	Vector<Pos>    back;
	Vector<Pos>    forward;
	Index<String>  tree_ndx;
	String         topic;
	String         label;

	bool GoTo0(const String& link);
	void Back();
	void Forward();
	void FontSize();
	void Tools(Bar& bar);
	void TreeCursor();
	void SetZoom();
	void Print();
	Pos  GetPos();

public:
	virtual Topic AcquireTopic(const String& topic);
	virtual void  FinishText(RichText& text);
	virtual void  BarEx(Bar& bar);

	void GoTo(const String& link);

	void SetBar();
	void Serialize(Stream& s);

	void ClearTree();
	int  AddTree(int parent, const Image& img, const String& topic, const String& title);
	void SortTree(int id = 0);
	void FinishTree();
	void OpenDeep(int id = 0);

	String GetCurrent() const                     { return topic; }
	String GetCurrentLabel() const                { return label; }

	typedef HelpWindow CLASSNAME;

	HelpWindow();
};

void Print(Draw& w, const RichText& text, const Rect& page, const Vector<int>& pagelist);
void Print(Draw& w, const RichText& text, const Rect& page);
bool Print(const RichText& text, const Rect& page, int currentpage, const char *name = NULL);
