#include "ide.h"

struct OptionDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink,
	                   Color paper, dword style) const
	{
		WString txt = q;
		w.DrawRect(r, paper);
		w.Clipoff(r.left + 10, r.top, r.Width() - 20, r.Height());
		int tcy = GetTLTextHeight(w, txt, StdFont());
		DrawTLText(w, 0, max((r.Height() - tcy) / 2, 0), r.Width(), txt, StdFont(), ink);
		w.End();
	}
};

struct OptionHeaderDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink,
	                   Color paper, dword style) const
	{
		WString txt = q;
		w.DrawRect(r, SColorInfo);
		w.Clipoff(r.left + 5, r.top, r.Width(), r.Height());
		int tcy = GetTLTextHeight(w, txt, StdFont().Bold());
		DrawTLText(w, 0, max((r.Height() - tcy) / 2, 0), r.Width(), txt,
		           StdFont().Bold(), SColorText);
		w.End();
	}
};

struct DependsDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink,
	                   Color paper, dword style) const
	{
		WString txt = q;
		w.DrawRect(r, paper);
		w.Clipoff(r.left + 10, r.top, r.Width() - 20, r.Height());
		Image img = IdeFileImage(q);
		Size isz = img.GetSize();
		w.DrawImage(0, (r.Height() - isz.cy) / 2, img);
		int tcy = GetTLTextHeight(w, txt, StdFont());
		DrawTLText(w, isz.cx + 4, max((r.Height() - tcy) / 2, 0), r.Width(), txt, StdFont(), ink);
		w.End();
	}
};

struct UsesDisplay : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink,
	                   Color paper, dword style) const
	{
		WString txt = q;
		w.DrawRect(r, paper);
		w.Clipoff(r.left + 10, r.top, r.Width() - 20, r.Height());
		Size isz = IdeImg::Package().GetSize();
		w.DrawImage(0, (r.Height() - isz.cy) / 2, IdeImg::Package());
		int tcy = GetTLTextHeight(w, txt, StdFont());
		DrawTLText(w, isz.cx + 4, max((r.Height() - tcy) / 2, 0), r.Width(), txt, StdFont(), ink);
		w.End();
	}
};

int FlagFilter(int c) {
	return IsAlNum(c) || c == '_' || c == ' ' ? ToUpper(c) : 0;
}

int FlagFilterM(int c) {
	return c == '.' ? '.' : FlagFilter(c);
}

int CondFilter(int c) {
	return c == '!' || c == '(' || c == ')' || c == '&' || c == '|' ? c : FlagFilter(c);
}

void UsesDlg::New()
{
	text <<= SelectPackage("Select package");
}

UsesDlg::UsesDlg()
{
	CtrlLayoutOKCancel(*this, "Uses");
	when.SetFilter(CondFilter);
	text.SetDisplay(Single<UsesDisplay>());
	text.WhenPush = THISBACK(New);
}

void PackageEditor::SaveOptions() {
	if(!actualpackage.IsEmpty()) {
		actual.charset = (byte)(int)~charset;
		actual.accepts = Split(accepts.GetText().ToString(), ' ');
		actual.optimize_speed = optimize_speed;
		if(IsActiveFile())
			ActiveFile().optimize_speed = optimize_speed_file;
		SavePackage();
	}
}

bool PackageEditor::Key(dword key, int count)
{
	if(key == K_ESCAPE) {
		Close();
		return true;
	}
	return TopWindow::Key(key, count);
}

void PackageEditor::Empty()
{
	FileEmpty();
	charset.Disable();
	optimize_speed.Disable();
	filelist.Clear();
	filelist.Disable();
	option.Clear();
	option.Disable();
}

void PackageEditor::FileEmpty()
{
	fileoption.Clear();
	fileoption.Disable();
	optimize_speed_file = false;
	optimize_speed_file.Disable();
}

void PackageEditor::OptionAdd(ArrayCtrl& option, int type, const char *title, const Array<OptItem>& o)
{
	if(o.GetCount() == 0)
		return;
	option.Add(-1, -1, "when", title);
	option.SetDisplay(option.GetCount() - 1, 0, Single<OptionHeaderDisplay>());
	option.SetDisplay(option.GetCount() - 1, 1, Single<OptionHeaderDisplay>());
	for(int i = 0; i < o.GetCount(); i++) {
		option.Add(type, i, o[i].when, o[i].text);
		if(type == USES)
			option.SetDisplay(option.GetCount() - 1, 1, Single<UsesDisplay>());
		if(type == FILEDEPENDS)
			option.SetDisplay(option.GetCount() - 1, 1, Single<DependsDisplay>());
	}
}

void PackageEditor::OptionAdd(int type, const char *title, const Array<OptItem>& o)
{
	OptionAdd(option, type, title, o);
}

void PackageEditor::PackageCursor()
{
	WorkspaceWork::PackageCursor();
	if(IsNull(actualpackage))
		Empty();
	else {
		charset <<= (int)actual.charset;
		optimize_speed = actual.optimize_speed;
		String s;
		for(int i = 0; i < actual.accepts.GetCount(); i++) {
			if(i) s << ' ';
			s << actual.accepts[i];
		}
		accepts = s.ToWString();
		charset.Enable();
		optimize_speed.Enable();
		accepts.Enable();
		option.Enable();
		option.Clear();
		for(int i = FLAG; i <= COMPILER; i++)
			OptionAdd(i, opt_name[i], *opt[i]);
	}
}

void PackageEditor::AdjustOptionCursor(ArrayCtrl& option)
{
	int q = option.GetCursor();
	if(q < 0)
		return;
	if((int)option.Get(q, 0) < 0 && q + 1 < option.GetCount())
		option.SetCursor(q + 1);
}

void PackageEditor::AdjustPackageOptionCursor()
{
	AdjustOptionCursor(option);
}

void PackageEditor::FindOpt(ArrayCtrl& option, int type, const String& when, const String& text)
{
	for(int i = 0; i < option.GetCount(); i++)
		if((int)option.Get(i, 0) == type &&
		   (String)option.Get(i, 2) == when && (String)option.Get(i, 3) == text) {
			option.SetCursor(i);
			option.SetFocus();
			break;
		 }
}

void PackageEditor::FindOpt(ArrayCtrl& option, int type, int ii)
{
	for(int i = 0; i < option.GetCount(); i++)
		if((int)option.Get(i, 0) == type && (int)option.Get(i, 1) == ii) {
			option.SetCursor(i);
			option.SetFocus();
			break;
		 }
}

void PackageEditor::SetOpt(ArrayCtrl& opt, int type, OptItem& m, const String& when, const String& text)
{
	m.when = when;
	m.text = text;
	SaveLoadPackage();
	FindOpt(opt, type, when, text);
}

void PackageEditor::AddOption(int type)
{
	if(IsNull(actualpackage))
		return;
	if(type == USES) {
		UsesDlg dlg;
		dlg.New();
		if(dlg.Run() == IDOK)
			SetOpt(option, USES, actual.uses.Add(), ~dlg.when, ~dlg.text);
		return;
	}
	WithUppOptDlg<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, opt_name[type]);
	dlg.when.SetFilter(CondFilter);
	if(dlg.Run() != IDOK)
		return;
	SetOpt(option, type, opt[type]->Add(), ~dlg.when, ~dlg.text);
}

void PackageEditor::RemoveOption()
{
	if(!option.IsCursor() || IsNull(actualpackage))
		return;
	int type = option.Get(0);
	if(type >= FLAG && type <= COMPILER) {
		Array<OptItem>& m = *opt[type];
		int i = option.Get(1);
		if(i >= 0 && i < m.GetCount())
			m.Remove(i);
		SaveLoadPackage();
	}
}

void PackageEditor::EditOption()
{
	if(!option.IsCursor() || IsNull(actualpackage))
		return;
	int type = option.Get(0);
	if(type == USES) {
		Array<OptItem>& m = *opt[type];
		int i = option.Get(1);
		if(i >= 0 && i < m.GetCount()) {
			UsesDlg dlg;
			dlg.when <<= m[i].when;
			dlg.text <<= m[i].text;
			if(dlg.Run() == IDOK)
				SetOpt(option, USES, m[i], ~dlg.when, ~dlg.text);
		}
		return;
	}
	if(type >= FLAG && type <= COMPILER) {
		Array<OptItem>& m = *opt[type];
		int i = option.Get(1);
		if(i >= 0 && i < m.GetCount()) {
			WithUppOptDlg<TopWindow> dlg;
			CtrlLayoutOKCancel(dlg, opt_name[type]);
			dlg.when <<= m[i].when;
			dlg.text <<= m[i].text;
			if(dlg.Run() != IDOK)
				return;
			SetOpt(option, type, m[i], ~dlg.when, ~dlg.text);
		}
	}
}

void PackageEditor::MoveOption(int d)
{
	if(!option.IsCursor() || IsNull(actualpackage))
		return;
	int type = option.Get(0);
	if(type >= FLAG && type <= COMPILER) {
		Array<OptItem>& m = *opt[type];
		int i = option.Get(1);
		if(min(i, i + d) >= 0 && max(i, i + d) < m.GetCount()) {
			Swap(m[i], m[i + d]);
			SaveLoadPackage();
			FindOpt(option, type, i + d);
		}
	}
}

void PackageEditor::OptionMenu(Bar& bar)
{
	bool b = !IsNull(actualpackage);
	bar.Add(b, "Add package..", IdeImg::package_add(), THISBACK1(AddOption, USES));
	for(int j = FLAG; j <= COMPILER; j++)
		if(j != USES)
			bar.Add(b, "New " + opt_name[j] + "..", THISBACK1(AddOption, j));
	bar.Separator();
	b = option.IsCursor() && (int)option.Get(0) >= 0;
	bar.Add(b, "Edit..", THISBACK(EditOption));
	bar.Add(b, "Remove", THISBACK(RemoveOption))
	   .Key(K_DELETE);
	bar.Separator();
	int type = option.IsCursor() ? (int)option.Get(0) : -1;
	int i = -1;
	Array<OptItem> *m = NULL;
	if(type >= FLAG && type <= COMPILER) {
		m = opt[type];
		i = option.Get(1);
	}
	bar.Add(i >= 0 && min(i, i - 1) >= 0,
	        "Move up", THISBACK1(MoveOption, -1))
	   .Key(K_CTRL_UP);
	bar.Add(m && i >= 0 && max(i, i + 1) < m->GetCount(),
	        "Move down", THISBACK1(MoveOption, 1))
	   .Key(K_CTRL_DOWN);
}

void PackageEditor::FileCursor()
{
	WorkspaceWork::FileCursor();
	if(IsActiveFile()) {
		Package::File& f = ActiveFile();
		if(!f.separator) {
			optimize_speed_file.Enable();
			optimize_speed_file = actual.file[actualfileindex].optimize_speed;
			fileoption.Enable();
			fileoption.Clear();
			OptionAdd(fileoption, FILEOPTION, "Additional compiler options for the file",
			          f.option);
			OptionAdd(fileoption, FILEDEPENDS, "Additional dependencies for the file",
			          f.depends);
			return;
		}
	}
	FileEmpty();
}

void PackageEditor::AdjustFileOptionCursor()
{
	AdjustOptionCursor(fileoption);
}

void PackageEditor::FileOptionMenu(Bar& bar)
{
	bool b = IsActiveFile();
	bar.Add(b, "Add compiler flags..", THISBACK(AddFileOption));
	bar.Add(b, "Add dependency..", THISBACK(AddDepends));
	bar.Separator();
	b = fileoption.IsCursor() && (int)fileoption.Get(0) >= 0;
	int type = b ? (int)fileoption.Get(0) : -1;
	bar.Add(b, "Edit..", THISBACK(EditFileOption));
	bar.Add(b, "Remove", THISBACK(RemoveFileOption));
	bar.Separator();
	int i = -1;
	Array<OptItem> *m = NULL;
	if(type >= 0) {
		m = type == FILEDEPENDS ? &ActiveFile().depends : &ActiveFile().option;
		i = fileoption.Get(1);
	}
	bar.Add(i >= 0 && min(i, i - 1) >= 0,
	        "Move up", THISBACK1(MoveFileOption, -1))
	   .Key(K_CTRL_UP);
	bar.Add(m && i >= 0 && max(i, i + 1) < m->GetCount(),
	        "Move down", THISBACK1(MoveFileOption, 1))
	   .Key(K_CTRL_DOWN);
}

void PackageEditor::MoveFileOption(int d)
{
	if(!fileoption.IsCursor() || !IsActiveFile())
		return;
	int type = fileoption.Get(0);
	if(type < 0)
		return;
	Array<OptItem>& m = type == FILEDEPENDS ? ActiveFile().depends : ActiveFile().option;
	int i = fileoption.Get(1);
	if(min(i, i + d) >= 0 && max(i, i + d) < m.GetCount()) {
		Swap(m[i], m[i + d]);
		SaveLoadPackage();
		FindOpt(fileoption, type, i + d);
	}
}

void DependsDlg::New()
{
	FileSel& fs = BasedSourceFs();
	fs.BaseDir(GetFileFolder(PackagePath(package)));
	fs.Multi(false);
	if(fs.ExecuteOpen("Additional file dependency"))
		text <<= ~fs;
	fs.Multi();
}

DependsDlg::DependsDlg()
{
	CtrlLayoutOKCancel(*this, "Additional file dependency");
	when.SetFilter(CondFilter);
	text.SetDisplay(Single<DependsDisplay>());
	text.WhenPush = THISBACK(New);
}

void PackageEditor::AddDepends()
{
	if(!IsActiveFile())
		return;
	DependsDlg dlg;
	dlg.package = GetActivePackage();
	dlg.New();
	if(dlg.Run() == IDOK)
		SetOpt(fileoption, FILEDEPENDS, ActiveFile().depends.Add(), ~dlg.when, ~dlg.text);
}

void PackageEditor::AddFileOption()
{
	if(!IsActiveFile())
		return;
	WithUppOptDlg<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Compiler options for the file");
	dlg.when.SetFilter(CondFilter);
	if(dlg.Run() == IDOK)
		SetOpt(fileoption, FILEDEPENDS, ActiveFile().option.Add(), ~dlg.when, ~dlg.text);
}

void PackageEditor::EditFileOption()
{
	if(!IsActiveFile() || !fileoption.IsCursor())
		return;
	int ii = (int)fileoption.Get(1);
	Package::File& f = ActiveFile();
	if(ii < 0 || ii >= f.option.GetCount())
		return;
	OptItem& m = f.option[(int)fileoption.Get(1)];
	WithUppOptDlg<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Compiler options for the file");
	dlg.when.SetFilter(CondFilter);
	dlg.when <<= m.when;
	dlg.text <<= m.text;
	if(dlg.Run() != IDOK)
		return;
	m.when = dlg.when;
	m.text = dlg.text;
	SaveLoadPackage();
	FindOpt(fileoption, FILEOPTION, ~dlg.when, ~dlg.text);
}

void PackageEditor::RemoveFileOption()
{
	if(!IsActiveFile() || !fileoption.IsCursor())
		return;
	int ii = (int)fileoption.Get(1);
	Package::File& f = ActiveFile();
	int type = fileoption.Get(0);
	if(type == FILEOPTION && ii >= 0 && ii < f.option.GetCount())
		f.option.Remove(ii);
	if(type == FILEDEPENDS && ii >= 0 && ii < f.depends.GetCount())
		f.option.Remove(ii);
	SaveLoadPackage();
}

void PackageEditor::Serialize(Stream& s) {
	int version = 2;
	s / version;
	s % filelist % package % package;
	SerializePlacement(s);
	if(version >= 1 && version <= 2) {
		Splitter dummy;
		s % dummy % dummy % dummy % dummy % dummy % dummy;
	}
}

void PackageEditor::Add(const char *name, Array<OptItem>& m)
{
	opt.Add(&m);
	opt_name.Add(name);
}

void PackageEditor::Init(ArrayCtrl& option)
{
	option.AddIndex();
	option.AddIndex();
	option.AddColumn("").SetDisplay(Single<OptionDisplay>()).Margin(0);
	option.AddColumn("", 2).SetDisplay(Single<OptionDisplay>()).Margin(0);
	option.NoHeader();
	option.SetLineCy(max(IdeImg::Package().GetSize().cy, Draw::GetStdFontCy()));
}

void PackageEditor::SaveOptionsLoad()
{
	SaveOptions();
	SaveLoadPackage();
}

PackageEditor::PackageEditor()
{
	organizer = true;
	CtrlLayoutOKCancel(*this, "Package organizer");
	DlCharsetD(charset);
	charset.Disable();
	optimize_speed.Disable();
	filelist.Disable();
	accepts.SetFilter(FlagFilter);
	accepts <<=
	charset <<= THISBACK(SaveOptions);
	optimize_speed <<=
	optimize_speed_file <<= THISBACK(SaveOptionsLoad);

	Add("Add/remove flags", actual.flag);
	Add("Uses", actual.uses);
	Add("Targets", actual.target);
	Add("Libraries", actual.library);
	Add("Link options", actual.link);
	Add("Compiler options", actual.option);

	Init(option);
	option.WhenCursor = THISBACK(AdjustPackageOptionCursor);
	option.WhenBar = THISBACK(OptionMenu);
	option.WhenLeftDouble = THISBACK(EditOption);

	Init(fileoption);
	fileoption.WhenCursor = THISBACK(AdjustFileOptionCursor);
	fileoption.WhenBar = THISBACK(FileOptionMenu);
	fileoption.WhenLeftDouble = THISBACK(EditFileOption);
}

void EditPackages(const char *main, const char *startwith, String& cfg) {
	PackageEditor pe;
	pe.Sizeable();
	StringStream in(cfg);
	pe.Serialize(in);
	pe.SetMain(main);
	pe.ScanWorkspace();
	pe.SyncWorkspace();
	pe.FindSetPackage(startwith);
	if(pe.Run() != IDOK)
		pe.RestoreBackup();
	StringStream out;
	pe.Serialize(out);
	cfg = out;
}
