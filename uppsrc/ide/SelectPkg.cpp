#include "ide.h"

class BaseSetupDlg : public WithBaseSetupLayout<TopWindow>
{
public:
	typedef BaseSetupDlg CLASSNAME;
	BaseSetupDlg();

	bool Run(String& vars);

private:
	void OnUpp();
	void OnBrowseUpp();

private:
	FrameRight<Button> browse_upp;
	SelectDirButton    browse_out;
	SelectDirButton    browse_common;
	bool               new_base;
};

bool BaseSetup(String& vars) { return BaseSetupDlg().Run(vars); }

BaseSetupDlg::BaseSetupDlg()
: browse_out("Output & intermediate files")
, browse_common("Common files")
{
	CtrlLayoutOKCancel(*this, "Assembly setup");
	browse_upp.SetImage(CtrlImg::right_arrow());
	browse_upp <<= THISBACK(OnBrowseUpp);
	upp.AddFrame(browse_upp);
	upp <<= THISBACK(OnUpp);
	browse_out.Attach(output);
	browse_common.Attach(common);
}

bool BaseSetupDlg::Run(String& vars)
{
	upp <<= GetVar("UPP");
	output <<= GetVar("OUTPUT");
	common <<= GetVar("COMMON");
	base <<= vars;
	new_base = IsNull(vars);
	while(TopWindow::Run() == IDOK)
	{
		String varname = ~base;
		String varfile = VarFilePath(varname);
		if(varname != vars)
		{
			if(FileExists(varfile) && !PromptOKCancel(NFormat("Overwrite existing base file [* \1%s\1]?", varfile)))
				continue;
			if(!SaveVars(varname))
			{
				Exclamation(NFormat("Error writing base file [* \1%s\1].", VarFilePath(varname)));
				continue;
			}
		}
		SetVar("UPP", ~upp);
		SetVar("OUTPUT", ~output);
		SetVar("COMMON", ~common);
		Vector<String> paths = SplitDirs(upp.GetText().ToString());
		for(int i = 0; i < paths.GetCount(); i++)
			RealizeDirectory(paths[i]);
		RealizeDirectory(output);
		vars = varname;
		return true;
	}
	return false;
}

void BaseSetupDlg::OnBrowseUpp()
{
	String s = ~upp;
	int b, e;
	if(upp.HasFocus())
		upp.GetSelection(b, e);
	else
		e = s.GetLength();
	b = e;
	while(b > 0 && s[b - 1] != ';')
		b--;
	while(e < s.GetLength() && s[e] != ';')
		e++;
	FileSel fsel;
	String pre = s.Left(b), post = s.Mid(e);
	fsel.ActiveDir(s.Mid(b, e - b));
	if(fsel.ExecuteSelectDir()) {
		String newdir = ~fsel;
		upp <<= pre + newdir + Nvl(post, ";");
		upp.SetWantFocus();
		OnUpp();
	}
}

void BaseSetupDlg::OnUpp()
{
	if(new_base) {
		String s = ~upp;
		int f = s.Find(';');
		if(f >= 0) s.Trim(f);
		base <<= GetFileTitle(s);
	}
}

inline bool PackageLess(String a, String b)
{
	int nc = CompareNoCase(a, b);
	if(nc) return nc < 0;
	return a < b;
};


struct SelectPackageDlg : public WithListLayout<TopWindow> {
	virtual bool Key(dword key, int count);

	typedef SelectPackageDlg CLASSNAME;

	SelectPackageDlg(const char *title, bool selectvars, bool main);

	String         Run(String startwith);

	void           Serialize(Stream& s);

	ArrayCtrl      base;
	ParentCtrl     list;
	FileList       clist;
	ArrayCtrl      alist;

	bool           selectvars;
	bool           loading;
	String         selected;
	int            update;

	struct PkInfo {
		String package;
		String description;
		String path;
		Vector<String> ws;

		bool operator<(const PkInfo& b) const { return PackageLess(package, b.package); }
	};

	Array<PkInfo> packages;

	String         GetCurrentName();
	int            GetCurrentIndex();
	void           SyncBrief();

	void           ToolBase(Bar& bar);

	void           OnBaseAdd();
	void           OnBaseEdit();
	void           OnBaseRemove();

	void           OnOK();

	void           OnNew();
	void           OnBase();
	void           OnAll();

	void           ListCursor();
	void           ChangeDescription();

	void           Load();
	void           Load(String upp, String dir, int progress_pos, int progres_count, String& case_fixed);
	void           SyncBase(String initvars);
	void           SyncList();
	static bool    Pless(const SelectPackageDlg::PkInfo& a, const SelectPackageDlg::PkInfo& b);
};

bool SelectPackageDlg::Key(dword key, int count)
{
	if((clist.HasFocus() || alist.HasFocus()) && search.Key(key, count))
		return true;
	return TopWindow::Key(key, count);
}

void SelectPackageDlg::Serialize(Stream& s)
{
	int version = 0;
	SerializePlacement(s);
	s % brief;
}

String SelectPackageDlg::GetCurrentName()
{
	if(clist.IsShown())
		return clist.GetCurrentName();
	else
	if(alist.IsCursor())
		return alist.Get(0);
	return Null;
}

int   SelectPackageDlg::GetCurrentIndex()
{
	String s = GetCurrentName();
	for(int i = 0; i < packages.GetCount(); i++)
		if(packages[i].package == s)
			return i;
	return -1;
}

void SelectPackageDlg::ChangeDescription()
{
	int ii = GetCurrentIndex();
	if(ii >= 0 && ii < packages.GetCount()) {
		PkInfo& p = packages[ii];
		WithDescriptionLayout<TopWindow> dlg;
		CtrlLayoutOKCancel(dlg, "Package description");
		dlg.text <<= p.description;
		if(dlg.Run() != IDOK)
			return;
		Package pkg;
		pkg.Load(p.path);
		pkg.description = ~dlg.text;
		pkg.Save(p.path);
		p.description = description <<= ~dlg.text;
		if(alist.IsCursor())
			alist.Set(1, ~dlg.text);
	}
}

void SelectPackageDlg::ListCursor()
{
	int c = GetCurrentIndex();
	if(c >= 0 && c < packages.GetCount())
		description <<= packages[c].description;
	else
		description <<= Null;
}

void SelectPackageDlg::SyncBrief()
{
	bool b = brief;
	alist.Show(!b);
	clist.Show(b);
}

SelectPackageDlg::SelectPackageDlg(const char *title, bool selectvars_, bool main)
: selectvars(selectvars_)
{
	CtrlLayoutOKCancel(*this, title);
	Sizeable().Zoomable();
	Icon(IdeImg::Package(), IdeImg::Package());
	base.AutoHideSb();
	base.NoGrid();
	base.AddColumn("Assembly");
	base.WhenCursor = THISBACK(OnBase);
	base.WhenBar = THISBACK(ToolBase);
	base.WhenLeftDouble = THISBACK(OnBaseEdit);
	ok <<= clist.WhenLeftDouble = alist.WhenLeftDouble = THISBACK(OnOK);
	clist.Columns(4);
	clist.WhenEnterItem = clist.WhenKillCursor = THISBACK(ListCursor);
	clist.NoAccelKey();
	alist.AddColumn("Package");
	alist.AddColumn("Description", 3);
	alist.WhenCursor = THISBACK(ListCursor);
	alist.EvenRowColor();
	alist.SetLineCy(max(16, Draw::GetStdFontCy()));
	list.Add(clist.SizePos());
	list.Add(alist.SizePos());
	splitter.Horz(base, list);
	splitter.SetPos(2000);
	splitter.Zoom(selectvars ? -1 : 1);
	newu <<= THISBACK(OnNew);
	all <<= THISBACK(OnAll);
	all.Show(main);
	all = !main;
	progress.Hide();
	brief <<= THISBACK(SyncBrief);
	search <<= THISBACK(SyncList);
	search.SetFilter(CharFilterDefaultToUpperAscii);
	SyncBrief();
	description <<= THISBACK(ChangeDescription);
	ActiveFocus(brief ? (Ctrl&)clist : (Ctrl&)alist);
}

String SelectPackageDlg::Run(String startwith)
{
	if(!IsSplashOpen())
		Open();
	if(selectvars)
		SyncBase(GetVarsName());
	else
		OnBase();
	alist.FindSetCursor(startwith);
	clist.FindSetCursor(startwith);
	ActiveFocus(alist.IsShown() ? (Ctrl&)alist : (Ctrl&)clist);
	switch(TopWindow::Run())
	{
	case IDOK:  return GetCurrentName();
	case IDYES: return selected;
	default:    return Null;
	}
}

void SelectPackageDlg::OnOK()
{
	loading = false;
	AcceptBreak(IDOK);
}

void SelectPackageDlg::OnAll()
{
	OnBase();
}

void SelectPackageDlg::OnBase()
{
	Load();
}

void SelectPackageDlg::OnNew() {
	TemplateDlg dlg;
	LoadFromGlobal(dlg, "NewPackage");
	dlg.Load(GetUppDirs(), !all);
	while(dlg.Run() == IDOK) {
		String nest = ~dlg.nest;
		String name = NativePath(String(~dlg.package));
		String path = AppendFileName(nest, AppendFileName(name, GetFileName(name) + ".upp"));
		if(FileExists(path) && !PromptYesNo("Package [* \1" + path + "\1] already exists.&"
		                                    "Do you wish to recreate the files?"))
			continue;
		RealizePath(path);
		if(!SaveFile(path, Null)) {
			Exclamation("Error writing the file [* \1" + path + "\1].");
			continue;
		}
		dlg.Create();
		selected = name;
		Break(IDYES);
		break;
	}
	StoreToGlobal(dlg, "NewPackage");
}

void SelectPackageDlg::ToolBase(Bar& bar)
{
	bar.Add("New assembly", THISBACK(OnBaseAdd))
		.Key(K_INSERT);
	bar.Add(base.IsCursor(), "Edit assembly", THISBACK(OnBaseEdit))
		.Key(K_CTRL_ENTER);
	bar.Add(base.IsCursor(), "Remove assembly", THISBACK(OnBaseRemove))
		.Key(K_CTRL_DELETE);
}

void SelectPackageDlg::OnBaseAdd()
{
	String vars;
	if(BaseSetup(vars))
		SyncBase(vars);
}

void SelectPackageDlg::OnBaseEdit()
{
	if(!base.IsCursor())
		return;
	String vars = base.Get(0), oldvars = vars;
	if(BaseSetup(vars))
	{
		if(vars != oldvars)
			DeleteFile(VarFilePath(oldvars));
		SyncBase(vars);
	}
}

void SelectPackageDlg::OnBaseRemove()
{
	int c = base.GetCursor();
	if(c < 0)
		return;
	String next;
	if(c + 1 < base.GetCount())
		next = base.Get(c + 1);
	else if(c > 0)
		next = base.Get(c - 1);
	String vars = base.Get(0);
	String varpath = VarFilePath(vars);
	if(PromptOKCancel(NFormat("Remove base file [* \1%s\1]?", varpath)) && !FileDelete(varpath))
		Exclamation(NFormat("Error deleting file [* \1%s\1].", varpath));
	SyncBase(next);
}

void SelectPackageDlg::Load()
{
	update = msecs();
	if(selectvars)
	{
		list.Enable(base.IsCursor());
		if(!base.IsCursor())
			return;
		LoadVars((String)base.Get(0));
	}
	Vector<String> upp = GetUppDirs();
	packages.Clear();
	all.Hide();
	search_txt.Hide();
	search.Hide();
	brief.Hide();
	progress.Show();
	loading = true;
	String case_fixed;
	for(int i = 0; i < upp.GetCount() && (all || i < 1) && loading; i++) {
		if(!IsSplashOpen() && !IsOpen())
			Open();
		Load(upp[i], Null, 1000 * i, 1000, case_fixed);
	}
	if(!IsNull(case_fixed))
		PromptOK("Case was fixed in some of the files:[* " + case_fixed);
	progress.Hide();
	while(IsSplashOpen())
		ProcessEvents();
	if(!IsOpen())
		Open();
	all.Show();
	search_txt.Show();
	search.Show();
	brief.Show();
	if(loading)
	{
		loading = false;
		SyncList();
	}
}

void FixName(const String& dir, const String& name, String& case_fixed)
{
	if(IsFullPath(name))
		return;
	String rp = AppendFileName(dir, name);
	FindFile ff(rp);
	if(!ff || ff.GetName() == GetFileName(name))
		return;
	MoveFile(rp, rp);
	case_fixed << '&' << DeQtf(rp);
}

int DirSep(int c)
{
	return c == '\\' || c == '/' ? c : 0;
}

void SelectPackageDlg::Load(String upp, String dir, int progress_pos, int progress_count, String& case_fixed)
{
	if(msecs(update) >= 200)
	{
		progress.Set(progress_pos);
		SyncList();
		ProcessEvents();
		if(!loading)
			return;
		update = msecs();
	}

	String d = AppendFileName(upp, dir);
	Vector<String> folders;
	for(FindFile ff(AppendFileName(d, "*.*")); ff; ff.Next())
		if(ff.IsFolder())
			folders.Add(ff.GetName());
	Sort(folders, &PackageLess);
	for(int i = 0; i < folders.GetCount() && loading; i++)
	{
		String fo = folders[i];
		String dd = AppendFileName(d, fo);
		String nm = fo + ".upp";
		String pkg = AppendFileName(dir, fo);
		String desc;
		String pf = AppendFileName(dd, nm);
		if(FileExists(pf)) {
			Package p;
			p.Load(pf);
			FixName(dd, nm, case_fixed);
			for(int i = 0; i < p.GetCount(); i++)
				if(!p[i].separator)
					FixName(dd, p[i], case_fixed);
			if(all || p.config.GetCount()) {
				PkInfo& pk = packages.Add();
				pk.package = pkg;
				pk.description = p.description;
				pk.path = pf;
				pk.ws = Split(ToUpper(pk.package), DirSep);
				pk.ws.Append(Split(ToUpper(pk.description), ' '));
			}
		}
		int ppos = progress_pos + i * progress_count / folders.GetCount();
		int npos = progress_pos + (i + 1) * progress_count / folders.GetCount();
		Load(upp, pkg, ppos, npos - ppos, case_fixed);
	}
}

void SelectPackageDlg::SyncBase(String initvars)
{
	Vector<String> varlist;
	for(FindFile ff(ConfigFile("*.var")); ff; ff.Next())
		if(ff.IsFile())
			varlist.Add(GetFileTitle(ff.GetName()));
	Sort(varlist, &PackageLess);
	base.Clear();
	Append(base, varlist);
	if(!base.FindSetCursor(initvars))
		if(base.GetCount() > 0)
			base.SetCursor(0);
		else
			OnBase();
}

bool SelectPackageDlg::Pless(const SelectPackageDlg::PkInfo& a, const SelectPackageDlg::PkInfo& b)
{
	return PackageLess(a.package, b.package);
}

struct PackageDisplay : Display {
	Font fnt;

	virtual Size GetStdSize(const Value& q) const {
		Size sz = GetTextSize(String(q), fnt);
		sz.cx += 20;
		sz.cy = max(sz.cy, 16);
		return sz;
	}

	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const {
		w.DrawRect(r, paper);
		w.DrawImage(r.left, r.top + (r.Height() - 16) / 2, IdeImg::Package());
		w.DrawText(r.left + 20, r.top + (r.Height() - Draw::GetStdFontCy()) / 2, String(q), fnt, ink);
	}

	PackageDisplay() { fnt = StdFont(); }
};

bool Match(const Vector<String>& l, const String& s)
{
	for(int i = 0; i < l.GetCount(); i++)
		if(memcmp(l[i], s, s.GetLength()) == 0)
			return true;
	return false;
}

void SelectPackageDlg::SyncList()
{
	Sort(packages);
	String n = GetCurrentName();
	int asc = alist.GetScroll();
	int csc = clist.GetSbPos();
	alist.Clear();
	clist.Clear();
	ListCursor();
	static PackageDisplay pd, bpd;
	bpd.fnt.Bold();
	String s = ~search;
	for(int i = 0; i < packages.GetCount(); i++)
		if(Match(packages[i].ws, s)) {
			clist.Add(packages[i].package, IdeImg::Package());
			alist.Add(packages[i].package, packages[i].description);
			alist.SetDisplay(alist.GetCount() - 1, 0, pd);
		}
	alist.ScrollTo(asc);
	clist.SetSbPos(csc);
	if(!alist.FindSetCursor(n))
		alist.GoBegin();
	if(!clist.FindSetCursor(n) && clist.GetCount())
		clist.SetCursor(0);
}

INITBLOCK
{
	RegisterGlobalConfig("SelectPkgMain");
	RegisterGlobalConfig("SelectPkg");
}

String SelectPackage(const char *title, const char *startwith, bool selectvars, bool main)
{
	SelectPackageDlg dlg(title, selectvars, main);
	const char *c = main ? "SelectPkgMain" : "SelectPkg";
	LoadFromGlobal(dlg, c);
	dlg.SyncBrief();
	String b = dlg.Run(startwith);
	StoreToGlobal(dlg, c);
	return b;
}
