#include "CtrlLib.h"

NAMESPACE_UPP

TreeCtrl::Node::Node()
{
	Init();
}

TreeCtrl::Node::Node(const Image& img, const Value& v)
{
	Init();
	image = img;
	key = value = v;
}

TreeCtrl::Node::Node(const Image& img, const Value& v, const Value& t)
{
	Init();
	image = img;
	key = v;
	value = t;
}

TreeCtrl::Node::Node(const Value& v)
{
	Init();
	key = value = v;
}

TreeCtrl::Node::Node(const Value& v, const Value& t)
{
	Init();
	key = v;
	value = t;
}

TreeCtrl::Node::Node(Ctrl& ctrl)
{
	Init();
	SetCtrl(ctrl);
}

TreeCtrl::Node::Node(const Image& img, Ctrl& ctrl, int cx, int cy)
{
	Init();
	SetCtrl(ctrl);
	image = img;
	size = ctrl.GetMinSize();
	if(cx > 0)
		size.cx = cx;
	if(cy > 0)
		size.cy = cy;
}

TreeCtrl::TreeCtrl()
{
	levelcx = 16;
	nocursor = false;
	noroot = false;
	dirty = true;
	multiselect = false;
	nobg = false;
	popupex = true;
	Clear();
	SetFrame(ViewFrame());
	AddFrame(sb);
	sb.WhenScroll = THISBACK(Scroll);
	WhenLeftDouble = THISBACK(StdLeftDouble);
}

void TreeCtrl::StdLeftDouble()
{
	if(IsCursor())
		Open(GetCursor(), !IsOpen(GetCursor()));
}

void TreeCtrl::CancelMode()
{
	selclick = false;
	dropinsert = 0;
	dropitem = -1;
	repoint = Null;
}

TreeCtrl::~TreeCtrl() {}

void   TreeCtrl::Layout()
{
	sb.SetPage(sb.GetReducedViewSize());
	sb.SetLine(item[0].GetSize().cy);
}

Size   TreeCtrl::Item::GetValueSize() const
{
	if(IsNull(size))
		if(ctrl)
			return ctrl->GetMinSize();
		else
			return display ? display->GetStdSize(value) : StdDisplay().GetStdSize(value);
	else
		return size;
}

Size   TreeCtrl::Item::GetSize() const
{
	Size sz = GetValueSize();
	sz += Size(2 * margin, 2 * margin);
	Size isz = image.GetSize();
	sz.cx += isz.cx;
	sz.cy = max(sz.cy, isz.cy);
	return sz;
}

void   TreeCtrl::SetRoot(const TreeCtrl::Node& n)
{
	(TreeCtrl::Node &)item[0] = n;
	RefreshItem(0);
}

void   TreeCtrl::SetRoot(const Image& img, Value v)
{
	SetRoot(TreeCtrl::Node(img, v).CanOpen());
}

void   TreeCtrl::SetRoot(const Image& img, Value v, Value t)
{
	SetRoot(TreeCtrl::Node(img, v, t).CanOpen());
}

void   TreeCtrl::SetRoot(const Image& img, Ctrl& ctrl, int cx, int cy)
{
	SetRoot(TreeCtrl::Node(img, ctrl, cx, cy).CanOpen());
}

int    TreeCtrl::Insert(int parentid, int i, const TreeCtrl::Node& n)
{
	int id;
	if(freelist >= 0) {
		id = freelist;
		freelist = item[id].freelink;
	}
	else {
		id = item.GetCount();
		item.Add();
	}
	Item& m = item[id];
	m.free = false;
	m.parent = parentid;
	(TreeCtrl::Node&)m = n;
	if(parentid >= 0) {
		Item& parent = item[parentid];
		parent.child.Insert(i, id);
	}
	dirty = true;
	Dirty(parentid);
	return id;
}

int    TreeCtrl::Add(int parentid, const TreeCtrl::Node& n)
{
	return Insert(parentid, item[parentid].child.GetCount(), n);
}

int    TreeCtrl::Insert(int parentid, int i)
{
	return Insert(parentid, i, TreeCtrl::Node());
}

int    TreeCtrl::Add(int parentid)
{
	return Add(parentid, TreeCtrl::Node());
}

int    TreeCtrl::Insert(int parentid, int i, const Image& img, Value v, bool withopen)
{
	return Insert(parentid, i, TreeCtrl::Node(img, v).CanOpen(withopen));
}

int    TreeCtrl::Insert(int parentid, int i, const Image& img, Value v, Value t, bool withopen)
{
	return Insert(parentid, i, TreeCtrl::Node(img, v, t).CanOpen(withopen));
}

int    TreeCtrl::Add(int parentid, const Image& img, Value v, bool withopen)
{
	return Add(parentid, TreeCtrl::Node(img, v).CanOpen(withopen));
}

int    TreeCtrl::Add(int parentid, const Image& img, Value v, Value t, bool withopen)
{
	return Add(parentid, TreeCtrl::Node(img, v, t).CanOpen(withopen));
}

int    TreeCtrl::Insert(int parentid, int i, const Image& img, Ctrl& ctrl, int cx, int cy, bool withopen)
{
	return Insert(parentid, i, TreeCtrl::Node(img, ctrl, cx, cy).CanOpen(withopen));
}

int    TreeCtrl::Add(int parentid, const Image& img, Ctrl& ctrl, int cx, int cy, bool withopen)
{
	return Add(parentid, TreeCtrl::Node(img, ctrl, cx, cy).CanOpen(withopen));
}

Value  TreeCtrl::Get(int id) const
{
	const Item& m = item[id];
	return m.ctrl ? m.ctrl->GetData() : m.key;
}

Value  TreeCtrl::GetValue(int id) const
{
	const Item& m = item[id];
	return m.ctrl ? m.ctrl->GetData() : m.value;
}

void  TreeCtrl::Set(int id, Value v)
{
	Item& m = item[id];
	if(m.ctrl)
		m.ctrl->SetData(v);
	else {
		m.value = m.key = v;
		RefreshItem(id);
	}
	SetOption(id);
}

void  TreeCtrl::Set(int id, Value k, Value v)
{
	Item& m = item[id];
	if(m.ctrl)
		m.ctrl->SetData(v);
	else {
		m.key = k;
		m.value = v;
		RefreshItem(id);
	}
	SetOption(id);
}

void   TreeCtrl::SetNode(int id, const TreeCtrl::Node& n)
{
	(TreeCtrl::Node&)item[id] = n;
	Dirty(id);
	SetOption(id);
}

void   TreeCtrl::RemoveChildren(int id)
{
	Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		RemoveSubtree(m.child[i]);
	m.child.Clear();
	Dirty(id);
}

int TreeCtrl::GetChildIndex(int id, int childid) const
{
	for(int i = 0; i < GetChildCount(id); i++)
		if(GetChild(id, i) == childid)
			return i;
	return -1;
}

void   TreeCtrl::RemoveSubtree(int id)
{
	Item& m = item[id];
	if(m.sel)
		selectcount--;
	if(m.linei == cursor)
		cursor = item[m.parent].linei;
	m.value = Null;
	m.image = Null;
	RemoveChildren(id);
	m.freelink = freelist;
	freelist = id;
	m.free = true;
}

void   TreeCtrl::Remove(int id)
{
	ASSERT(id > 0);
	int pi = item[id].parent;
	Item& parent = item[item[id].parent];
	parent.child.Remove(FindIndex(parent.child, id));
	RemoveSubtree(id);
	Dirty(pi);
}

void   TreeCtrl::Remove(const Vector<int>& id)
{
	for(int i = 0; i < id.GetCount(); i++)
		if(!item[id[i]].free)
			Remove(id[i]);
}

void   TreeCtrl::Clear()
{
	item.Clear();
	item.Add();
	item[0].linei = -1;
	item[0].parent = -1;
	freelist = -1;
	Dirty();
	cursor = anchor = -1;
	selectcount = 0;
}

void TreeCtrl::RemoveCtrls(int itemi)
{
	Item& m = item[itemi];
	if(m.ctrl)
		m.ctrl->Remove();
	for(int i = 0; i < m.child.GetCount(); i++)
		RemoveCtrls(m.child[i]);
}

void TreeCtrl::ReLine(int itemi, int level, Size& sz)
{
	int ii = line.GetCount();
	Line& l = line.Add();
	l.level = level;
	l.itemi = itemi;
	l.y = sz.cy;
	l.ll = -1;
	Item& m = item[itemi];
	if(m.ctrl) {
		hasctrls = true;
		m.ctrl->Remove();
	}
	m.linei = ii;
	Size msz = m.GetSize();
	sz.cy += msz.cy;
	sz.cx = max(sz.cx, level * levelcx + msz.cx);
	level++;
	for(int i = 0; i < m.child.GetCount(); i++)
		if(m.isopen) {
			line[ii].ll = line.GetCount();
			ReLine(m.child[i], level, sz);
		}
		else
			RemoveCtrls(m.child[i]);
}

void TreeCtrl::SyncTree()
{
	if(!dirty)
		return;
	if(noroot)
		Open(0);
	Ptr<Ctrl> restorefocus = GetFocusChildDeep();
	hasctrls = false;
	int cursorid = GetCursor();
	for(int i = 0; i < item.GetCount(); i++)
		item[i].linei = -1;
	line.Clear();
	Size treesize = Size(0, 0);
	if(noroot) {
		if(GetChildCount(0))
			treesize.cy = -item[0].GetSize().cy;
		ReLine(0, -1, treesize);
	}
	else
		ReLine(0, 0, treesize);
	treesize.cy = max(0, treesize.cy);
	treesize.cx += levelcx;
	sb.SetTotal(treesize);
	cursor = -1;
	dirty = false;
	if(cursorid >= 0)
		SetCursor(cursorid, false, false);
	SyncCtrls(true, restorefocus);
	SyncInfo();
}

void TreeCtrl::SyncCtrls(bool add, Ctrl *restorefocus)
{
	if(!hasctrls)
		return;
	Point org = sb;
	for(int i = noroot; i < line.GetCount(); i++) {
		const Line& l = line[i];
		Item& m = item[l.itemi];
		if(m.ctrl) {
			if(add)
				AddChildBefore(m.ctrl, GetLastChild());
			if(m.ctrl == restorefocus || m.ctrl->HasChildDeep(restorefocus))
				restorefocus->SetFocus();
			Size msz = m.GetSize();
			Size isz = m.image.GetSize();
			Size vsz = m.GetValueSize();
			m.ctrl->SetRect(levelcx + l.level * levelcx + isz.cx + m.margin - org.x,
			                l.y + (msz.cy - vsz.cy) / 2 - org.y, vsz.cx, vsz.cy);
		}
	}
}

bool TreeCtrl::IsOpen(int id) const
{
	return item[id].isopen;
}

void TreeCtrl::Dirty(int id)
{
	ClearSelection();
	Size sz = GetSize();
	dirty = true;
	while(id >= 0) {
		int q = item[id].linei;
		if(q >= 0) {
			int y = line[q].y - sb.GetY();
			Refresh(0, y, sz.cx, sz.cy - y);
			return;
		}
		id = item[id].parent;
	}
	Refresh();
}

void TreeCtrl::Open(int id, bool open)
{
	Item& m = item[id];
	if(m.isopen != open) {
		m.isopen = open;
		int q = GetCursor();
		while(q >= 0) {
			if(q == id) {
				SetCursor(id);
				break;
			}
			q = GetParent(q);
		}
		Dirty(id);
		if(open)
			WhenOpen(id);
		else
			WhenClose(id);
	}
}

void TreeCtrl::OpenDeep(int id, bool open)
{
	Open(id, open);
	Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		OpenDeep(m.child[i], open);
}

void TreeCtrl::MakeVisible(int id)
{
	id = GetParent(id);
	while(id >= 0) {
		Open(id);
		id = GetParent(id);
	}
}

int TreeCtrl::FindLine(int y) const
{
	int l = FindUpperBound(line, y, LineLess());
	return l > 0 ? l - 1 : 0;
}

void TreeCtrl::RefreshLine(int i, int ex)
{
	SyncTree();
	if(i >= 0) {
		Size sz = GetSize();
		int y = line[i].y - sb.GetY();
		Refresh(0, y - ex, sz.cx, item[line[i].itemi].GetSize().cy + 2 * ex);
		SyncInfo();
	}
}

void TreeCtrl::RefreshItem(int id, int ex)
{
	SyncTree();
	if(id >= 0 && id < item.GetCount())
		RefreshLine(item[id].linei, ex);
}

int  TreeCtrl::GetItemAtLine(int i)
{
	SyncTree();
	return line[i].itemi;
}

int  TreeCtrl::GetLineAtItem(int id)
{
	SyncTree();
	return item[id].linei;
}

int  TreeCtrl::GetLineCount()
{
	SyncTree();
	return line.GetCount();
}

void TreeCtrl::SetCursorLine(int i, bool sc, bool sel)
{
	if(nocursor)
		return;
	if(sel && multiselect) {
		ClearSelection();
		SelectOne(line[i].itemi, true);
	}
	if(i != cursor) {
		i = minmax(i, 0, line.GetCount() - 1);
		if(i < 0) return;
		Item& m = item[line[i].itemi];
		if(sc)
			sb.ScrollIntoY(line[i].y, m.GetSize().cy);
		RefreshLine(cursor);
		cursor = i;
		RefreshLine(cursor);
		if(m.ctrl && m.ctrl->SetWantFocus())
			return;
		WhenCursor();
		WhenSel();
	}
}

void TreeCtrl::SetCursorLine(int i)
{
	SetCursorLine(i, true, true);
}

void TreeCtrl::SetCursorLineSync(int i)
{
	if(nocursor)
		return;
	if(i != cursor) {
		RefreshLine(cursor);
		if(i < 0)
			cursor = -1;
		else
			cursor = minmax(i, (int)noroot, line.GetCount() - 1);
		RefreshLine(cursor);
		Item& m = item[line[cursor].itemi];
		if(cursor >= 0) {
			Sync();
			sb.ScrollIntoY(line[cursor].y, m.GetSize().cy);
		}
		if(!(m.ctrl && m.ctrl->SetWantFocus()))
			SetFocus();
		WhenCursor();
		WhenSel();
	}
}

void TreeCtrl::KillCursor()
{
	RefreshLine(cursor);
	cursor = -1;
	Refresh();
	WhenCursor();
	WhenSel();
	SyncInfo();
}

void TreeCtrl::SetCursor(int id, bool sc, bool sel)
{
	while(id > 0) {
		ASSERT(id >= 0 && id < item.GetCount());
		MakeVisible(id);
		SyncTree();
		const Item& m = item[id];
		if(m.linei >= 0) {
			SetCursorLine(m.linei, sc, true);
			return;
		}
		id = m.parent;
	}
	SetCursorLine(0, sc, true);
}

void TreeCtrl::SetCursor(int id)
{
	SetCursor(id, true, true);
}

int  TreeCtrl::GetCursor() const
{
	return cursor >= 0 ? line[cursor].itemi : -1;
}

Value  TreeCtrl::Get() const
{
	return IsCursor() ? Get(GetCursor()) : Value();
}

Value  TreeCtrl::GetValue() const
{
	return IsCursor() ? GetValue(GetCursor()) : Value();
}

int    TreeCtrl::Find(Value key)
{
	for(int i = 0; i < item.GetCount(); i++)
		if(Get(i) == key)
			return i;
	return -1;
}

bool   TreeCtrl::FindSetCursor(Value key)
{
	int q = Find(key);
	if(q < 0)
		return false;
	SetCursor(q);
	return true;
}

void TreeCtrl::UpdateSelect()
{
	WhenSelection();
	WhenSel();
	WhenAction();
}

bool TreeCtrl::IsSel(int id) const
{
	return multiselect ? IsSelected(id) : GetCursor() == id;
}

bool TreeCtrl::IsSelDeep(int id) const
{
	return IsSel(id) || id && IsSelDeep(GetParent(id));
}

void TreeCtrl::SelectOne0(int id, bool sel)
{
	if(!multiselect)
		return;
	if(item[id].canselect && item[id].sel != sel) {
		item[id].sel = sel;
		selectcount += 2 * sel - 1;
		RefreshItem(id);
	}
}

void TreeCtrl::SelectOne(int id, bool sel)
{
	SelectOne0(id, sel);
	UpdateSelect();
}

void TreeCtrl::ShiftSelect(int l1, int l2)
{
	if(!multiselect)
		return;
	bool b = false;
	if(l1 > l2)
		Swap(l1, l2);
	for(int i = 0; i < line.GetCount(); i++)
		SelectOne0(line[i].itemi, i >= l1 && i <= l2);
	UpdateSelect();
}

void TreeCtrl::LeftDrag(Point p, dword keyflags)
{
	if(p.y + sb.y > sb.GetTotal().cy)
		return;
	WhenDrag();
}

void TreeCtrl::DoClick(Point p, dword flags, bool down)
{
	Point org = sb;
	if(p.y + org.y > sb.GetTotal().cy)
		return;
	int i = FindLine(p.y + org.y);
	const Line& l = line[i];
	int x = levelcx + l.level * levelcx - org.x - (levelcx >> 1) - org.x;
	if(p.x > x - 6 && p.x < x + 6) {
		if(down)
			Open(l.itemi, !IsOpen(l.itemi));
	}
	else {
		if(down && IsSel(l.itemi)) {
			selclick = true;
			return;
		}
		SetFocus();
		int q = cursor;
		SetCursorLine(i, true, false);
		if(multiselect) {
			int id = GetCursor();
			if(flags & K_CTRL) {
				SelectOne(id, !IsSelected(id));
				anchor = cursor;
			}
			else
				if(flags & K_SHIFT)
					ShiftSelect(anchor < 0 ? cursor : anchor, cursor);
				else {
					ClearSelection();
					SelectOne(id);
					anchor = cursor;
				}
		}
		if(cursor != q)
			WhenAction();
	}
}

void TreeCtrl::SyncInfo()
{
	if((HasMouse() || info.HasMouse()) && popupex) {
		Size sz = GetSize();
		Point p = GetMouseViewPos();
		Point org = sb;
		if(p.y + org.y > sb.GetTotal().cy)
			return;
		int i = FindLine(p.y + org.y);
		const Line& l = line[i];
		const Item& m = item[l.itemi];
		int x = levelcx + l.level * levelcx - org.x + m.image.GetSize().cx;
		Rect r = RectC(x, l.y - org.y, sz.cx - x, m.GetSize().cy);
		if(r.Contains(p)) {
			Color fg, bg;
			dword st;
			const Display *d = GetStyle(i, fg, bg, st);
			info.Set(this, r, m.value, d, fg, bg, st, m.margin);
			return;
		}
	}
	info.Cancel();
}

void TreeCtrl::MouseMove(Point, dword)
{
	SyncInfo();
}

void TreeCtrl::LeftDown(Point p, dword flags)
{
	DoClick(p, flags, true);
}

void TreeCtrl::LeftUp(Point p, dword keyflags)
{
	if(selclick) {
		DoClick(p, keyflags, false);
		selclick = false;
	}
}

void TreeCtrl::LeftDouble(Point p, dword flags)
{
	LeftDown(p, flags);
	Point org = sb;
	if(p.y + org.y > sb.GetTotal().cy)
		return;
	int i = FindLine(p.y + org.y);
	const Line& l = line[i];
	int x = levelcx + l.level * levelcx - org.x - (levelcx >> 1) - org.x;
	if(i == GetCursorLine() && (flags & (K_SHIFT|K_CTRL)) == 0 && p.x > x + 6)
		WhenLeftDouble();
}

void TreeCtrl::RightDown(Point p, dword flags)
{
	Point org = sb;
	if(p.y + org.y < sb.GetTotal().cy) {
		int i = FindLine(p.y + org.y);
		if(i >= 0) {
			SetFocus();
			SetCursorLine(i);
		}
	}
	if(WhenBar)
		MenuBar::Execute(WhenBar);
}

const Display *TreeCtrl::GetStyle(int i, Color& fg, Color& bg, dword& st)
{
	const Line& l = line[i];
	const Item& m = item[l.itemi];
	st = 0;
	fg = SColorText;
	bg = SColorPaper;
	if(nobg)
		bg = Null;
	bool hasfocus = HasFocus() && !IsDragAndDropTarget();
	bool drop = l.itemi == dropitem && dropinsert == 0;
	if(IsReadOnly())
		st |= Display::READONLY;
	if(m.sel && multiselect)
		st |= Display::SELECT;
	if(i == cursor && !nocursor)
		st |= Display::CURSOR;
	if(drop) {
		hasfocus = true;
		st |= Display::CURSOR;
	}
	if(hasfocus)
		st |= Display::FOCUS;
	if((st & Display::SELECT) ||
	    !multiselect && (st & Display::CURSOR) && !nocursor ||
	    drop) {
		fg = hasfocus ? SColorHighlightText : SColorText;
		bg = hasfocus ? SColorHighlight : Blend(SColorDisabled, SColorPaper);
	}
	if(m.display)
		return m.display;
	return &StdDisplay();
}

void TreeCtrl::Paint(Draw& w)
{
	SyncTree();
	Size sz = GetSize();
	Point org = sb;
	scroller.Set(org);
	if(!nobg)
		w.DrawRect(sz, SColorPaper);
	int levelcx2 = levelcx >> 1;
	for(int i = 0; i < line.GetCount(); i++) {
		Line& l = line[i];
		if(l.ll >= 0) {
			int yl = line[i].y + item[l.itemi].GetSize().cy - org.y;
			int yh = line[l.ll].y + item[line[l.ll].itemi].GetSize().cy / 2 - org.y;
			if(yh >= 0 && yl < sz.cy) {
				int x = levelcx + levelcx * l.level + levelcx2 - org.x;
				w.DrawRect(x, yl, 1, yh - yl, SColorShadow);
			}
		}
	}
	Rect dri;
	for(int i = FindLine(org.y); i < line.GetCount(); i++) {
		Line& l = line[i];
		const Item& m = item[l.itemi];
		Size msz = m.GetSize();
		Size isz = m.image.GetSize();
		Size vsz = m.GetValueSize();
		int y = l.y - org.y;
		if(y > sz.cy)
			break;
		int x = 0;
		x = levelcx + l.level * levelcx - org.x;
		Point op = Point(x - levelcx2, y + msz.cy / 2);
		Rect r = RectC(x, y, vsz.cx + 2 * m.margin, msz.cy);
		if(l.itemi == dropitem) {
			dri = r;
			if(i == 0)
				dri.top++;
		}
		if(w.IsPainting(0, y, sz.cx, msz.cy)) {
			w.DrawRect(op.x, op.y, levelcx2, 1, SColorShadow);
			if(m.canopen || m.child.GetCount()) {
				Image im = m.isopen ? CtrlImg::treeminus() : CtrlImg::treeplus();
				op -= im.GetSize() / 2;
				w.DrawImage(op.x, op.y, im);
			}
			w.DrawImage(x, y + (msz.cy - isz.cy) / 2, m.image);
			x += isz.cx;
			Color fg, bg;
			dword st;
			const Display *d = GetStyle(i, fg, bg, st);
			if(!(m.ctrl && m.ctrl->IsWantFocus())) {
				w.DrawRect(x, y, vsz.cx + 2 * m.margin, msz.cy, bg);
				d->Paint(w, RectC(x + m.margin, y + (msz.cy - vsz.cy) / 2, vsz.cx, vsz.cy), m.value,
				         fg, bg, st);
				if(i == cursor && !nocursor && multiselect && GetSelectCount() != 1 && HasFocus()
				   && !IsDragAndDropTarget())
					DrawFocus(w, r, st & Display::SELECT ? SColorPaper() : SColorText());
			}
		}
	}
	if(dropitem >= 0 && dropinsert)
		DrawHorzDrop(w, dri.left - 2, dropinsert < 0 ? dri.top : dri.bottom - 1,
		             sz.cx - dri.left + 2);
}

Image TreeCtrl::GetDragSample()
{
	SyncTree();
	Size sz = StdSampleSize();
	ImageDraw iw(StdSampleSize());
	iw.DrawRect(sz, SColorPaper);
	int y = 0;
	for(int i = 0; i < line.GetCount(); i++) {
		Line& l = line[i];
		const Item& m = item[l.itemi];
		const Display *d = m.display;
		if(!d) d = &StdDisplay();
		Size msz = m.GetSize();
		Size isz = m.image.GetSize();
		Size vsz = m.GetValueSize();
		Rect r = RectC(0, y, vsz.cx + 2 * m.margin, msz.cy);
		int x = 0;
		if(IsSel(l.itemi)) {
			iw.DrawImage(x, y + (msz.cy - isz.cy) / 2, m.image);
			x += isz.cx;
			if(!(m.ctrl && m.ctrl->IsWantFocus())) {
				d->Paint(iw, RectC(x + m.margin, y + (msz.cy - vsz.cy) / 2, vsz.cx, vsz.cy), m.value,
				         SColorHighlightText, SColorHighlight, Display::SELECT|Display::FOCUS);
			}
			y += msz.cy;
		}
	}
	return ColorMask(iw, SColorPaper);
}

void TreeCtrl::Scroll()
{
	SyncTree();
	if(hasctrls) {
		Refresh();
		SyncCtrls(false, NULL);
	}
	else
		scroller.Scroll(*this, sb);
}

void TreeCtrl::MouseWheel(Point, int zdelta, dword)
{
	sb.WheelY(zdelta);
}

void TreeCtrl::ChildGotFocus()
{
	for(int i = 0; i < line.GetCount(); i++) {
		Item& m = item[line[i].itemi];
		if(m.ctrl && m.ctrl->HasFocusDeep()) {
			SetCursorLine(i);
			return;
		}
	}
}

bool TreeCtrl::Tab(int d)
{
	if(cursor < 0)
		return false;
	Item& m = item[line[cursor].itemi];
	if(m.ctrl && m.ctrl->HasFocusDeep())
		return false;
	int i = cursor;
	for(int i = cursor + d; i >= 0 && i < line.GetCount(); i += d) {
		Item& m = item[line[i].itemi];
		if(m.ctrl && m.ctrl->SetWantFocus())
			return true;
	}
	return false;
}

bool TreeCtrl::Key(dword key, int)
{
	SyncTree();
	Size sz = GetSize();
	int cid = GetCursor();
	bool shift = key & K_SHIFT;
	key &= ~K_SHIFT;
	switch(key) {
	case K_TAB:
		return Tab(1);
	case K_SHIFT_TAB:
		return Tab(-1);
	case K_UP:
		SetCursorLineSync(cursor > 0 ? cursor - 1 : line.GetCount() - 1);
		break;
	case K_DOWN:
		SetCursorLineSync(cursor >= 0 ? cursor + 1 : 0);
		break;
	case K_PAGEDOWN:
		SetCursorLineSync(cursor >= 0 ? FindLine(line[cursor].y + sz.cy) : 0);
		break;
	case K_PAGEUP:
		SetCursorLineSync(cursor >= 0 ? FindLine(line[cursor].y - sz.cy) : line.GetCount() - 1);
		break;
	case K_LEFT:
		if(cid >= 0)
			Close(cid);
		break;
	case K_RIGHT:
		if(cid >= 0)
			Open(cid);
		break;
	default:
		return false;
	}
	if(GetCursor() != cid) {
		if(multiselect && cursor >= 0) {
			if(shift) {
				if(anchor < 0)
					anchor = cursor;
				ShiftSelect(anchor, cursor);
			}
			else {
				ClearSelection();
				SelectOne(GetCursor());
				anchor = cursor;
			}
		}
		WhenAction();
	}
	return true;
}

void TreeCtrl::GotFocus()
{
	if(dirty)
		return;
	if(hasctrls && cursor >= 0 && item[line[cursor].itemi].ctrl)
		for(int i = 0; i < line.GetCount(); i++) {
			Item& m = item[line[i].itemi];
			if(m.ctrl && m.ctrl->SetWantFocus())
				break;
		}
	else
		RefreshLine(cursor);
	if(GetSelectCount() > 1)
		Refresh();
	SyncInfo();
}

void TreeCtrl::LostFocus()
{
	if(dirty)
		return;
	RefreshLine(cursor);
	if(GetSelectCount() > 1)
		Refresh();
	SyncInfo();
}

void TreeCtrl::ChildRemoved(Ctrl *)
{
	Dirty();
}

struct TreeCtrl::SortOrder {
	TreeCtrl         *tree;
	const ValueOrder *order;
	bool              byvalue;

	bool operator()(int a, int b) const {
		return byvalue ? (*order)(tree->GetValue(a), tree->GetValue(b))
		               : (*order)(tree->Get(a), tree->Get(b));
	}

};

void TreeCtrl::Sort0(int id, const ValueOrder& order, bool byvalue)
{
	SortOrder so;
	so.tree = this;
	so.order = &order;
	so.byvalue = byvalue;
	UPP::Sort(item[id].child, so);
}

void TreeCtrl::Sort(int id, const ValueOrder& order, bool byvalue)
{
	SyncTree();
	Sort0(id, order, byvalue);
	Dirty(id);
}

void TreeCtrl::SortDeep0(int id, const ValueOrder& order, bool byvalue)
{
	Sort0(id, order, byvalue);
	Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		SortDeep0(m.child[i], order, byvalue);
}

void TreeCtrl::SortDeep(int id, const ValueOrder& order, bool byvalue)
{
	SyncTree();
	SortDeep0(id, order, byvalue);
	Dirty(id);
}

void TreeCtrl::Sort(int id, int (*compare)(const Value& v1, const Value& v2), bool byvalue)
{
	Sort(id, FnValueOrder(compare), byvalue);
}

void TreeCtrl::SortDeep(int id, int (*compare)(const Value& v1, const Value& v2), bool byvalue)
{
	SortDeep(id, FnValueOrder(compare), byvalue);
}

void TreeCtrl::SortByValue(int id, const ValueOrder& order)
{
	Sort(id, order, true);
}

void TreeCtrl::SortDeepByValue(int id, const ValueOrder& order)
{
	SortDeep(id, order, true);
}

void TreeCtrl::SortByValue(int id, int (*compare)(const Value& v1, const Value& v2))
{
	Sort(id, compare, true);
}

void TreeCtrl::SortDeepByValue(int id, int (*compare)(const Value& v1, const Value& v2))
{
	SortDeep(id, compare, true);
}

void  TreeCtrl::SetData(const Value& data)
{
	FindSetCursor(data);
}

Value TreeCtrl::GetData() const
{
	return Get();
}

Point  TreeCtrl::GetScroll() const
{
	return sb;
}

void TreeCtrl::ScrollTo(Point sc)
{
	sb = sc;
}

void TreeCtrl::SelClear(int id)
{
	Item& m = item[id];
	if(m.sel) {
		RefreshItem(id);
		m.sel = false;
	}
	for(int i = 0; i < m.child.GetCount(); i++)
		SelClear(m.child[i]);
}

void TreeCtrl::GatherSel(int id, Vector<int>& sel) const
{
	if(IsSel(id))
		sel.Add(id);
	const Item& m = item[id];
	for(int i = 0; i < m.child.GetCount(); i++)
		GatherSel(m.child[i], sel);
}

Vector<int> TreeCtrl::GetSel() const
{
	Vector<int> v;
	GatherSel(0, v);
	return v;
}

void TreeCtrl::RefreshSel()
{
	Size sz = GetSize();
	for(int i = FindLine(sb.GetY()); i < line.GetCount(); i++) {
		Line& l = line[i];
		int y = l.y - sb.GetY();
		if(y > sz.cy)
			break;
		if(IsSel(l.itemi))
			RefreshItem(l.itemi);
	}
}

void TreeCtrl::ClearSelection()
{
	if(selectcount) {
		SelClear(0);
		WhenSelection();
		WhenSel();
		WhenAction();
		selectcount = 0;
	}
}

void TreeCtrl::SetOption(int id)
{
}

void TreeCtrl::DnD(int itemid, int insert)
{
	if(itemid != dropitem || insert != dropinsert)
	{
		RefreshItem(dropitem, 4);
		dropitem = itemid;
		dropinsert = insert;
		RefreshItem(dropitem, 4);
	}
}

bool TreeCtrl::DnDInserti(int ii, PasteClip& d, bool bottom)
{
	if(ii >= 0 && ii < line.GetCount()) {
		int itemi = line[ii].itemi;
		int parent = GetParent(itemi);
		int childi = GetChildIndex(parent, itemi);
		int ins = -1;
		if(bottom)
			if(childi != GetChildCount(parent) - 1 && ii + 1 < line.GetCount())
				return DnDInserti(ii + 1, d, false);
			else {
				childi++;
				ins = 1;
			}
		WhenDropInsert(parent, childi, d);
		if(d.IsAccepted()) {
			DnD(itemi, ins);
			return true;
		}
	}
	return false;
}

void TreeCtrl::DragEnter(Point p, PasteClip& d)
{
	RefreshSel();
}

void TreeCtrl::DragAndDrop(Point p, PasteClip& d)
{
	int py = p.y + sb.GetY();
	if(py < sb.GetTotal().cy) {
		int ii = FindLine(py);
		const Line& l = line[ii];
		if(l.itemi && WhenDropInsert) {
			int y = l.y;
			int cy = item[l.itemi].GetSize().cy;
			if(py < y + cy / 4 && DnDInserti(ii, d, false))
				return;
			if(py >= y + 3 * cy / 4 && DnDInserti(ii, d, true))
				return;
		}
		WhenDropItem(l.itemi, d);
		if(d.IsAccepted()) {
			DnD(l.itemi, 0);
			return;
		}
		WhenDropInsert(l.itemi, GetChildCount(l.itemi), d);
		if(d.IsAccepted()) {
			DnD(l.itemi, 0);
			return;
		}
	}
	WhenDrop(d);
	DnD(-1, 0);
}

void TreeCtrl::DragRepeat(Point p)
{
	if(IsReadOnly())
		return;
	sb = sb + GetDragScroll(this, p, 16);
	p.y += sb.y;
	if(p == repoint) {
		if(GetTimeClick() - retime > 1000 && p.y < sb.GetTotal().cy) {
			int ii = FindLine(p.y);
			const Line& l = line[ii];
			int y = l.y;
			int cy = item[l.itemi].GetSize().cy;
			if(p.y >= y + cy / 4 && p.y < y + 3 * cy / 4 &&
			   !IsOpen(l.itemi) && GetChildCount(l.itemi)) {
				Open(l.itemi, true);
				retime = GetTimeClick();
			}
		}
	}
	else {
		retime = GetTimeClick();
		repoint = p;
	}
}

void TreeCtrl::DragLeave()
{
	DnD(-1, 0);
	RefreshSel();
	repoint = Null;
}

void TreeCtrl::AdjustAction(int parent, PasteClip& d)
{
	if(IsDragAndDropSource() && IsSelDeep(parent) && d.GetAction() == DND_MOVE)
		d.SetAction(DND_COPY);
}

void TreeCtrl::InsertDrop(int parent, int ii, const TreeCtrl& src, PasteClip& d)
{
	TreeCtrl copy;
	Vector<int> sel = src.GetSel();
	for(int i = 0; i < sel.GetCount(); i++)
		Copy(copy, 0, i, src, sel[i]);
	Vector<int> did;
	for(int i = 0; i < copy.GetChildCount(0); i++) {
		did.Add(Copy(*this, parent, ii + i, copy, copy.GetChild(0, i)));
		SetCursor(did.Top());
	}
	if(&src == this && d.GetAction() == DND_MOVE) {
		Remove(sel);
		d.SetAction(DND_COPY);
	}
	for(int i = 0; i < did.GetCount(); i++)
		SelectOne(did[i], true);
}

void TreeCtrl::InsertDrop(int parent, int ii, PasteClip& d)
{
	InsertDrop(parent, ii, GetInternal<TreeCtrl>(d), d);
}

void OptionTree::SetRoot(const Image& img, Option& opt, const char *text)
{
	if(text)
		opt.SetLabel(text);
	TreeCtrl::SetRoot(img, opt);
	opt.NoNotNull().BlackEdge();
	option.At(0) = &opt;
	opt <<= THISBACK1(SetOption, 0);
}

void OptionTree::Clear()
{
	TreeCtrl::Clear();
	aux.Clear();
	option.Clear();
	aux.Add();
}

void OptionTree::SetRoot(Option& option, const char *text)
{
	SetRoot(Null, option, text);
}

void OptionTree::SetRoot(const Image& img, const char *text)
{
	SetRoot(img, aux[0], text);
}

void OptionTree::SetRoot(const char *text)
{
	SetRoot(Null, aux[0], text);
}

int OptionTree::Add(int parentid, const Image& img, Option& opt, const char *text)
{
	if(text)
		opt.SetLabel(text);
	int id = TreeCtrl::Add(parentid, img, opt);
	option.At(id, NULL) = &opt;
	opt.NoNotNull().BlackEdge();
	opt <<= THISBACK1(SetOption, id);
	SetOption(id);
	return id;
}

int OptionTree::Add(int parentid, Option& opt, const char *text)
{
	return Add(parentid, Null, opt.NoNotNull(), text);
}

int OptionTree::Add(int parentid, const Image& img, const char *text)
{
	return Add(parentid, img, aux.Add().NoNotNull(), text);
}

int OptionTree::Add(int parentid, const char *text)
{
	return Add(parentid, aux.Add().NoNotNull(), text);
}

void OptionTree::SetChildren(int id, bool b)
{
	for(int i = 0; i < GetChildCount(id); i++) {
		int chid = GetChild(id, i);
		Option *opt = option[chid];
		if(opt)
			opt->Set(b);
		SetChildren(chid, b);
	}
}

void OptionTree::SetOption(int id)
{
	Option *opt = option[id];
	ASSERT(opt);
	SetChildren(id, opt->Get());
	for(;;) {
		id = GetParent(id);
		if(id < 0)
			break;
		bool t = false;
		bool f = false;
		bool n = false;
		for(int i = 0; i < GetChildCount(id); i++) {
			int chid = GetChild(id, i);
			Option *opt = option[chid];
			if(opt) {
				int x = opt->Get();
				if(x == 1)
					t = true;
				else
				if(x == 0)
					f = true;
				else
					n = true;
			}
		}
		opt = option[id];
		if(opt) {
			if(t && f || n) {
				opt->Set(Null);
			}
			else
				opt->Set(t);
		}
	}
	WhenOption();
}

OptionTree::OptionTree() { aux.Add(); }
OptionTree::~OptionTree() {}

int Copy(TreeCtrl& dst, int did, int i, const TreeCtrl& src, int id)
{
	did = dst.Insert(did, i, src.GetNode(id));
	dst.Open(did, src.IsOpen(id));
	for(int i = 0; i < src.GetChildCount(id); i++)
		Copy(dst, did, i, src, src.GetChild(id, i));
	return did;
}

END_UPP_NAMESPACE
