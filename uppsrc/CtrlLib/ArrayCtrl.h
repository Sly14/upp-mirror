template <class T>
void DefaultCtrlFactoryFn(One<Ctrl>& ctrl)
{
	ctrl = new T;
}

template <class T>
Callback1<One<Ctrl>&> DefaultCtrlFactory()
{
	return callback(DefaultCtrlFactoryFn<T>);
}

class ArrayCtrl : public Ctrl {
public:
	virtual bool  Accept();
	virtual void  Reject();
	virtual void  Paint(Draw& w);
	virtual void  Layout();
	virtual bool  Key(dword key, int);
	virtual void  LeftDown(Point p, dword);
	virtual void  LeftDouble(Point p, dword);
	virtual void  RightDown(Point p, dword);
	virtual void  MouseMove(Point p, dword);
	virtual void  MouseWheel(Point p, int zdelta, dword keyflags);
	virtual Image CursorImage(Point p, dword);
	virtual void  GotFocus();
	virtual void  LostFocus();
	virtual void  ChildGotFocus();
	virtual void  ChildLostFocus();

public:
	struct IdInfo {
		Value           insertval;
		ValueGen       *insertgen;
		int            (*accel)(int);

		IdInfo& InsertValue(const Value& v)  { insertval = v; return *this; }
		IdInfo& InsertValue(ValueGen& g)     { insertgen = &g; return *this; }
		IdInfo& Accel(int (*filter)(int))    { accel = filter; return *this; }
		IdInfo& Accel()                      { return Accel(CharFilterDefaultToUpperAscii); }

		Value   GetInsertValue()             { return insertgen ? insertgen->Get() : insertval; }

		IdInfo()                             { insertgen = NULL; accel = NULL; }
	};

	class Column : FormatConvert {
		ArrayCtrl            *arrayctrl;
		Mitor<int>            pos;
		const Convert        *convert;
		Ptr<Ctrl>             edit;
		const Display        *display;
		Callback1<One<Ctrl>&> factory;
		int                 (*accel)(int);
		int                   margin;
		bool                  cached;
		bool                  clickedit;
		Any                   cache;

		void   InvalidateCache(int i);
		void   InsertCache(int i);
		void   RemoveCache(int i);
		void   ClearCache();

		friend class ArrayCtrl;

	public:
		Column& Add(int _pos)                      { pos.Add(_pos); return *this; }
		Column& Add(Id id)                         { pos.Add(-id.AsNdx()); return *this; }
		Column& AddIndex(Id id)                    { arrayctrl->AddIndex(id); return Add(id); }
		Column& AddIndex()                         { Add(arrayctrl->GetIndexCount()); arrayctrl->AddIndex(); return *this; }

		Column& SetConvert(const Convert& c);
		Column& SetFormat(const char *fmt);
		Column& SetDisplay(const Display& d);
		Column& Edit(Ctrl& e);
		Column& Ctrls(Callback1<One<Ctrl>&> factory);
		Column& Ctrls(void (*factory)(One<Ctrl>&)) { return Ctrls(callback(factory)); }
		template <class T>
		Column& Ctrls()                            { return Ctrls(DefaultCtrlFactory<T>()); }
		Column& InsertValue(const Value& v);
		Column& InsertValue(ValueGen& g);
		Column& NoClickEdit()                      { clickedit = false; return *this; }
		Column& Cache();
		Column& Accel(int (*filter)(int))          { accel = filter; return *this; }
		Column& Accel()                            { return Accel(CharFilterDefaultToUpperAscii); }

		Column& Margin(int m)                      { margin = m; return *this; }

		HeaderCtrl::Column& HeaderTab();

		Column();
	};

	struct Order {
		virtual bool operator()(const Vector<Value>& row1, const Vector<Value>& row2) const = 0;
	};

private:
	struct ItemOrder;
	struct RowOrder;
	struct SortPredicate;

	struct Control : Moveable<Control> {
		int   pos;
		Ctrl *ctrl;
	};

	struct CellCtrl : StaticRect {
		virtual void LeftDown(Point, dword);

		Ctrl      *ctrl;
	};

	struct CellInfo : Moveable<CellInfo> {
		BitAndPtr ptr;

		void           Set(Ctrl *ctrl);
		bool           IsCtrl() const             { return ptr.GetBit(); }
		CellCtrl&      GetCtrl() const            { ASSERT(IsCtrl()); return *(CellCtrl *)ptr.GetPtr(); }

		void           Set(const Display& d)      { ptr.Set0((void *)&d); }
		bool           IsDisplay() const          { return !ptr.GetBit() && ptr.GetPtr(); }
		const Display& GetDisplay() const         { ASSERT(IsDisplay()); return *(const Display *)ptr.GetPtr(); }

		~CellInfo();
	};

	friend class Column;
	friend struct SortPredicate;

	struct Ln : Moveable<Ln> {
		int y;
		int cy;
		Ln()               { cy = Null; }
	};

	struct Line : Moveable<Line> {
		bool          select;
		Vector<Value> line;

		Line() { select = false; }
	};

	Vector<Line>               array;
	HeaderCtrl                 header;
	ScrollBar                  sb;
	Scroller                   scroller;
	Array<Column>              column;
	Vector<Control>            control;
	ArrayMap<Id, IdInfo>       idx;
	Vector<Ln>                 ln;
	Vector< Vector<CellInfo> > cellinfo;
	Vector<bool>               modify;
	FrameBottom<StaticRect>    scrollbox;

	int   keypos;
	int   cursor;
	int   anchor;
	int   linecy;
	int   virtualcount;
	int   clickcolumn;

	mutable int   selectcount;

	String row_name;
	Color  gridcolor;
	Color  evenpaper, evenink, oddpaper, oddink;

	bool  horzgrid:1;
	bool  vertgrid:1;
	bool  nocursor:1;
	bool  mousemove:1;
	bool  editmode:1;
	bool  insertmode:1;

	bool  inserting:1;
	bool  appending:1;
	bool  appendline:1;
	bool  noinsertappend:1;
	bool  autoappend:1;
	bool  removing:1;
	bool  moving:1;
	bool  askremove:1;
	bool  duplicating:1;
	bool  multiselect:1;
	bool  hasctrls:1;
	bool  headerctrls:1;

	mutable bool  selectiondirty:1;

	unsigned  bains:2;

	int    Pos(int np) const;

	void   InsertCache(int i);
	void   RemoveCache(int i);

	void   RefreshRow(int i);

	void   SetSb();
	void   HeaderLayout();
	void   Scroll();
	void   Page(int q);

	void   DoPoint(Point p, bool dosel = true);
	void   ClickColumn(Point p);
	Rect   GetCellRect(int i, int col);
	Rect   GetCellRectM(int i, int col);

	Point           FindCellCtrl(Ctrl *c);
	void            SyncCtrls();
	bool            IsCtrl(int i, int j) const;
	const CellCtrl& GetCtrl(int i, int j) const;
	CellCtrl&       GetCtrl(int i, int j);
	void            SetCtrlValue(int i, int ii, const Value& v);

	void   PlaceEdits();
	void   EndEdit();
	void   ColEditSetData(int col);
	void   CtrlSetData(int col);
	Value  Get0(int i, int ii) const;
	void   Set0(int i, int ii, const Value& v);
	void   AfterSet();

	void   Reline(int i, int y);
	bool   AcceptRow();
	void   Remove0(int i);
	void   DisableCtrls();
	void   ClearModify();
	void   SetCtrls();
	void   DoRemovem();
	bool   KillCursor0();

	bool   TestKey(int i, int key);

	void   Move(int d);

	bool   SetCursor0(int i, bool dosel = true);

	void   SyncSelection() const;
	void   KeyMultiSelect(dword key);

	void   HeaderScrollVisibility();

protected:
	virtual bool UpdateRow();
	virtual void RejectRow();

public:
	Callback          WhenLeftDouble;
	Callback          WhenLeftClick;
	Callback1<Bar&>   WhenBar;
	Callback          WhenEnterRow;
	Gate              WhenAcceptRow;
	Callback          WhenUpdateRow;
	Callback          WhenKillCursor;
	Callback          WhenCursor;
	Callback          WhenArrayAction;
	Callback          WhenStartEdit;
	Callback          WhenAcceptEdit;
	Callback          WhenSelection;
	Callback          WhenCtrlsAction;

	IdInfo&    IndexInfo(int ii);
	IdInfo&    IndexInfo(Id id);
	IdInfo&    AddIndex(Id id);
	IdInfo&    AddIndex();
	int        GetIndexCount() const        { return idx.GetCount(); }
	Id         GetId(int ii) const          { return idx.GetKey(ii); }
	int        GetPos(Id id) const          { return idx.Find(id); }
	IdInfo&    SetId(int ii, Id id);
	IdInfo&    AddKey(Id id)                { ASSERT(idx.GetCount() == 0); return AddIndex(id); }
	IdInfo&    AddKey()                     { ASSERT(idx.GetCount() == 0); return AddIndex(); }
	Id         GetKeyId() const             { return idx.GetKey(0); }

	Column&    AddColumn(const char *text = NULL, int w = 0);
	Column&    AddColumn(Id id, const char *text, int w = 0);
	Column&    AddColumnAt(int ii, const char *text, int w = 0);
	Column&    AddColumnAt(Id id, const char *text, int w = 0);
	Column&    AddRowNumColumn(const char *text, int w = 0);

	int                       GetColumnCount() const   { return column.GetCount(); }
	int                       FindColumnWithPos(int pos) const;
	int                       FindColumnWithId(Id id) const;
	Column&                   ColumnAt(int i)          { return column[i]; }
	Column&                   ColumnAt(Id id)          { return column[FindColumnWithId(id)]; }
	HeaderCtrl::Column&       HeaderTab(int i)         { return header.Tab(i); }
	HeaderCtrl::Column&       HeaderTab(Id id)         { return header.Tab(FindColumnWithId(id)); }
	const Column&             ColumnAt(int i) const    { return column[i]; }
	const Column&             ColumnAt(Id id) const    { return column[FindColumnWithId(id)]; }
	const HeaderCtrl::Column& HeaderTab(int i) const   { return header.Tab(i); }
	const HeaderCtrl::Column& HeaderTab(Id id) const   { return header.Tab(FindColumnWithId(id)); }

	const HeaderCtrl&         HeaderObject() const     { return header; }
	HeaderCtrl&               HeaderObject()           { return header; }

	void       SerializeHeader(Stream& s)  { header.Serialize(s); }

	IdInfo&    AddCtrl(Ctrl& ctrl);
	IdInfo&    AddCtrl(Id id, Ctrl& ctrl);
	void       AddCtrlAt(int ii, Ctrl& ctrl);
	void       AddCtrlAt(Id id, Ctrl& ctrl);
	void       AddRowNumCtrl(Ctrl& ctrl);

	void       SetCount(int c);
	void       SetVirtualCount(int c);
	int        GetCount() const;
	void       Clear();
	void       Shrink();
	Value      Get(int i, int ii) const;
	Value      Get(int i, Id id) const;
	void       Set(int i, int ii, const Value& v);
	void       Set(int i, Id id, const Value& v);

	Value      Get(int ii) const;
	Value      Get(Id id) const;
	Value      GetOriginal(int ii) const;
	Value      GetOriginal(Id id) const;
	bool       IsModified(int ii) const;
	bool       IsModified(Id id) const;
	Value      GetKey() const;
	Value      GetOriginalKey() const;
	void       Set(int ii, const Value& v);
	void       Set(Id id, const Value& v);

	Value      GetColumn(int row, int col) const;
	Value      GetConvertedColumn(int row, int col);

	int        GetSelectCount() const;
	bool       IsSelection() const                              { return GetSelectCount(); }
	void       Select(int i, bool sel = true);
	void       Select(int i, int count, bool sel = true);
	bool       IsSelected(int i) const                          { return array[i].select; }
	void       ClearSelection();

	Vector<Value> ReadRow(int i) const;

	void       Set(int i, const Vector<Value>& v);

	void       Add();
	void       Add(const Vector<Value>& v);

//$-void Add(const Value& [, const Value& ]...);
#define  E__Add(I)      void Add(__List##I(E__Value));
	__Expand(E__Add)
#undef   E__Add
//$+
	void       Insert(int i);
	void       Insert(int i, const Vector<Value>& v);

	void       Remove(int i);

	bool       IsCursor() const                        { return cursor >= 0; }
	bool       SetCursor(int i);
	bool       KillCursor();
	void       CancelCursor();
	int        GetCursor() const                       { return cursor; }
	void       GoBegin();
	void       GoEnd();
	int        GetCursorSc() const;
	void       ScCursor(int a);
	void       CenterCursor();
	void       ScrollInto(int line);
	void       ScrollIntoCursor();
	int        GetScroll() const;
	void       ScrollTo(int sc);
	void       ShowAppendLine();

	void       SwapUp();
	void       SwapDown();

	int        Find(const Value& v, int ii = 0, int from = 0) const;
	int        Find(const Value& v, Id id, int from = 0) const;

	bool       FindSetCursor(const Value& val, int ii = 0, int from = 0);
	bool       FindSetCursor(const Value& val, Id id, int from = 0);

	void       Sort(const ArrayCtrl::Order& order);
	void       Sort(int from, int count, const ArrayCtrl::Order& order);
	void       Sort(int (*compare)(const Vector<Value>& v1, const Vector<Value>& v2));
	void       Sort(int ii, int (*compare)(const Value& v1, const Value& v2)
	                = StdValueCompare);
	void       Sort(Id id, int (*compare)(const Value& v1, const Value& v2)
	                = StdValueCompare);
	void       Sort()                                  { Sort(0); }

	bool       IsInsert() const                        { return insertmode; }

	void            SetDisplay(int i, int j, const Display& d);
	const Display&  GetDisplay(int row, int col);
	const Display&  GetDisplay(int col);

	Ctrl&      SetCtrl(int i, int j, Ctrl *newctrl);

	ArrayCtrl& SetLineCy(int cy);
	void       SetLineCy(int i, int cy);
	int        GetLineCy() const                       { return linecy; }
	int        GetLineY(int i) const;
	int        GetLineCy(int i) const;
	int        GetTotalCy() const;
	int        GetLineAt(int y) const;

	int        GetClickColumn() const                  { return clickcolumn; }

	bool       StartEdit(int d = 0);
	int        GetEditColumn() const;
	bool       IsEdit() const                          { return editmode; }

	void       DoEdit();
	void       DoInsert(int cursor);
	void       DoInsertBefore();
	void       DoInsertAfter();
	void       DoAppend();
	bool       DoRemove();
	void       DoDuplicate();
	void       DoSelectAll();
	void       StdBar(Bar& menu);

	bool       IsEditing() const;
	bool       AcceptEnter();

	void       ClearCache();
	void       InvalidateCache(int i);

	String     RowFormat(const char *s);

	ArrayCtrl& RowName(const char *s)                  { row_name = s; return *this; }
	ArrayCtrl& AppendLine(bool b = true)               { appendline = b; return *this; }
	ArrayCtrl& NoAppendLine()                          { return AppendLine(false); }
	bool       IsAppendLine() const                    { return appendline; }
	ArrayCtrl& Inserting(bool b = true)                { inserting = b; return AppendLine(b); }
	ArrayCtrl& NoInserting()                           { return Inserting(false); }
	bool       IsInserting() const                     { return inserting; }
	ArrayCtrl& Removing(bool b = true)                 { removing = b; return *this; }
	ArrayCtrl& NoRemoving()                            { return Removing(false); }
	bool       IsRemoving() const                      { return removing; }
	ArrayCtrl& Appending(bool b = true)                { appending = b; return *this; }
	bool       IsAppending() const                     { return appending || autoappend; }
	ArrayCtrl& AutoAppending(bool b = true)            { autoappend = b; return *this; }
	bool       IsAutoAppending() const                 { return autoappend; }
	ArrayCtrl& BeforeAfterInserting(int q = 1)         { bains = 1; return *this; }
	ArrayCtrl& AfterBeforeInserting(int q = 2)         { bains = 2; return *this; }
	ArrayCtrl& Duplicating(bool b = true)              { duplicating = b; return *this; }
	ArrayCtrl& NoInsertAppend(bool b = true)           { noinsertappend = b; return *this; }
	bool       IsDuplicating() const                   { return duplicating; }
	ArrayCtrl& Moving(bool b = true)                   { moving = b; return *this; }
	bool       IsMoving() const                        { return moving; }
	ArrayCtrl& NoDuplicating()                         { return Duplicating(false); }
	ArrayCtrl& AskRemove(bool b = true)                { askremove = b; return *this; }
	ArrayCtrl& NoAskRemove()                           { return AskRemove(false); }
	bool       IsAskRemove() const                     { return askremove; }

	ArrayCtrl& Header(bool b = true)                   { header.Invisible(!b); return *this; }
	ArrayCtrl& NoHeader()                              { return Header(false); }
	ArrayCtrl& Track(bool b = true)                    { header.Track(b); return *this; }
	ArrayCtrl& NoTrack()                               { return Track(false); }
	ArrayCtrl& VertGrid(bool b = true)                 { vertgrid = b; Refresh(); return *this; }
	ArrayCtrl& NoVertGrid()                            { return VertGrid(false); }
	ArrayCtrl& HorzGrid(bool b = true)                 { horzgrid = b; Refresh(); return *this; }
	ArrayCtrl& NoHorzGrid()                            { return HorzGrid(false); }
	ArrayCtrl& Grid(bool b = true)                     { return VertGrid(b).HorzGrid(b); }
	ArrayCtrl& NoGrid()                                { return Grid(false); }
	ArrayCtrl& GridColor(Color c)                      { gridcolor = c; return *this; }
	ArrayCtrl& EvenRowColor(Color paper = Blend(SColorHighlight, SColorPaper, 220), Color ink = SColorText);
	ArrayCtrl& OddRowColor(Color paper = SColorInfo, Color ink = SColorText);
	ArrayCtrl& NoCursor(bool b = true)                 { nocursor = b; return *this; }
	ArrayCtrl& MouseMoveCursor(bool b = true)          { mousemove = b; return *this; }
	ArrayCtrl& NoMouseMoveCursor()                     { return MouseMoveCursor(false); }
	ArrayCtrl& AutoHideSb(bool b = true)               { sb.AutoHide(b); return *this; }
	ArrayCtrl& NoAutoHideSb()                          { return AutoHideSb(false); }
	ArrayCtrl& MultiSelect(bool b = true)              { multiselect = b; return *this; }

	ArrayCtrl& ColumnWidths(const char *s);

	void Reset();

	typedef ArrayCtrl CLASSNAME;

	ArrayCtrl();
	virtual ~ArrayCtrl();
};

class ArrayOption : public Display, public Pte<ArrayOption> {
public:
	typedef ArrayOption CLASSNAME;
	ArrayOption();
	virtual ~ArrayOption();

	virtual void       Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;

	void               Connect(ArrayCtrl& ac, int ii = 0);
	void               Connect(ArrayCtrl& ac, Id id)               { Connect(ac, ac.GetPos(id)); }

	void               Disconnect();

	ArrayCtrl::Column& AddColumn(ArrayCtrl& ac, const char *text = NULL, int w = 0);
	ArrayCtrl::Column& AddColumn(ArrayCtrl& ac, Id id, const char *text, int w = 0);

	ArrayOption&       TrueFalse(Value _t, Value _f)               { t = _t; f = _f; return *this; }
	Value              GetFalse() const                            { return f; }
	Value              GetTrue() const                             { return t; }
	ArrayOption&       ThreeState(bool b = true)                   { threestate = b; return *this; }
	ArrayOption&       NoThreeState()                              { return ThreeState(false); }
	bool               IsThreeState() const                        { return threestate; }

	ArrayOption&       HSwitch(bool hs = true)                     { hswitch = hs; return *this; }
	ArrayOption&       NoHSwitch()                                 { return HSwitch(false); }
	bool               IsHSwitch() const                           { return hswitch; }

	ArrayOption&       VSwitch(bool vs = true)                     { vswitch = vs; return *this; }
	ArrayOption&       NoVSwitch()                                 { return VSwitch(false); }
	bool               IsVSwitch() const                           { return vswitch; }

	ArrayOption&       Frame(bool frm = true)                      { frame = frm; return *this; }
	ArrayOption&       NoFrame()                                   { return Frame(false); }
	bool               IsFrame() const                             { return frame; }

	void               Click();

public:
	Callback           WhenAction;

	Callback           operator <<= (Callback cb)                  { return WhenAction = cb; }

private:
	Vector<int>        index;
	ArrayCtrl         *array;
	Value              t, f;
	bool               hswitch, vswitch;
	bool               threestate;
	bool               frame;
};
