#include "ide.h"

void Ide::MakeTitle()
{
	String title;
	if(!main.IsEmpty())
		title << main;
	if(mainconfigname == mainconfigparam)
		title << " - " << mainconfigname;
	else
	if(!mainconfigname.IsEmpty()) {
		title << " - " << mainconfigname;
		title << " ( " << mainconfigparam << " )";
	}
	title << " - TheIDE";
	if(designer) {
		title << " - [" << designer->GetFileName();
		int cs = designer->GetCharset();
		if(cs >= 0)
			title << " " << CharsetName(cs);
		title << "]";
	}
	else
	if(!editfile.IsEmpty()) {
		title << " - [" << editfile;
		int chrset = editor.GetCharset();
		title << " " << (chrset == TextCtrl::CHARSET_UTF8_BOM ? "UTF-8 BOM" : CharsetName(chrset));
		if(editor.IsReadOnly())
			title << " (Read Only)";
		if(editor.IsDirty())
			title << " *";
		title << "]";
	}
	if(!IsNull(editfile))
		for(int i = 0; i < 10; i++)
			if(NormalizePath(editfile) == NormalizePath(bookmark[i].file))
				title << Format(" <%d>", i);
	title << " { " << GetVarsName() << " }";
	Title(title.ToWString());
}

void Ide::MakeIcon() {
#ifdef PLATFORM_WIN32
	String mp = main;
	Image li = IdeImg::PackageLarge();
	if(!IsNull(mp))
	{
		Size isz = li.GetSize();
		ImageDraw idraw(isz);
		Draw& mdraw = idraw.Alpha();
		idraw.DrawImage(0, 0, li);
		mdraw.DrawImage(0, 0, li, White);
		Font font = StdFont(9);
		Size sz = GetTextSize(mp, font);
		sz.cx = min(sz.cx + 4, isz.cx);
		sz.cy += 2;
		int x = (isz.cx - sz.cx) / 2;
		int y = isz.cy - sz.cy;
		idraw.DrawRect(x, y, sz.cx, sz.cy, White);
		mdraw.DrawRect(x, y, sz.cx, sz.cy, White);
		idraw.DrawText(x + 2, y + 1, mp, font, Black);
		DrawFrame(idraw, x, y, sz.cx, sz.cy, LtBlue);
		li = idraw;
	}
	LargeIcon(li);
#endif
}

bool Ide::CanToggleReadOnly()
{
	FindFile ff(editfile);
	if(ff && ff.IsReadOnly()) return false;
	return NormalizePath(GetActiveFilePath()) == NormalizePath(editfile);
}

void Ide::ToggleReadOnly()
{
	if(CanToggleReadOnly() && IsActiveFile()) {
		editor.SetEditable(editor.IsReadOnly());
		ActiveFile().readonly = editor.IsReadOnly();
		SavePackage();
		MakeTitle();
		SetBar();
	}
}

void Ide::AdjustMainConfig()
{
	const Workspace& wspc = IdeWorkspace();
	if(!wspc.GetCount())
		return;
	const Array<Package::Config>& f = wspc.GetPackage(0).config;
	for(int i = 0; i < f.GetCount(); i++)
		if(f[i].param == mainconfigparam)
			return;
	if(f.GetCount()) {
		mainconfigparam = f[0].param;
		mainconfigname = f[0].name;
		SetMainConfigList();
	}
}

String Ide::GetFirstFile()
{
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& p = wspc.GetPackage(i);
		for(int j = 0; j < p.GetCount(); j++)
			if(!p[j].separator)
				return SourcePath(wspc[i], p[j]);
	}
	return Null;
}

void Ide::SetMain(const String& package)
{
	FlushFile();
	SaveWorkspace();
	transferfilecache.Clear();
	main = package;
	export_dir = GetHomeDirFile(main);
	mainconfigname.Clear();
	mainconfigparam.Clear();
	ScanWorkspace();
	SyncWorkspace();
	LoadFromFile(THISBACK(SerializeWorkspace), WorkspaceFile());
	editorsplit.Zoom(0);
	UpdateFormat();
	String e = editfile;
	editfile.Clear();
	MakeTitle();
	MakeIcon();
	SyncMainConfigList();
	AdjustMainConfig();
	SyncBuildMode();
	SetHdependDirs();
	SetBar();
	HideBottom();
	if(IsNull(e))
		e = GetFirstFile();
	SyncRefs();
	StartCodeBase();
	EditFile(e);
}

void Ide::Exit()
{
	if(debugger)
		debugger->Stop();
	SaveFile();
	SaveWorkspace();
	FlushFile();
	console.Kill();
	Break(IDOK);
}

bool Ide::OpenMainPackage()
{
	String tt = "Select main package";
//	tt << " (IDE " << IDE_VER_MAJOR << "." << IDE_VER_MINOR << ".r" << IDE_VER_BUILD
//	<< ", " << ::AsString(IDE_VER_DATE) << ")";
#ifdef bmYEAR
	tt << " (TheIDE " << IDE_VERSION
	   << Format(" %d-%02d-%02d %d:%02d)", bmYEAR , bmMONTH, bmDAY, bmHOUR, bmMINUTE);
#else
	tt << " (TheIDE " << IDE_VERSION << ')';
#endif
	String p = SelectPackage(tt, main, true, true);
	if(p.IsEmpty()) return false;
	main.Clear();
	if(!IsOpen())
		Open();
	SetMain(p);
	return true;
}

void Ide::NewMainPackage()
{
	SaveCodeBase();
	OpenMainPackage();
}

void Ide::PackageCursor()
{
	WorkspaceWork::PackageCursor();
	String p = GetActivePackage();
	if(p.IsEmpty()) return;
	String ef = ToLower(NormalizePath(editfile));
	for(int i = 0; i < filelist.GetCount(); i++)
		if(ToLower(NormalizePath(SourcePath(p, filelist[i]))) == ef) {
			filelist.SetCursor(i);
			break;
		}
	SetBar();
}

void Ide::EditWorkspace()
{
	EditPackages(main, GetActivePackage(), pocfg);
	ScanWorkspace();
	SyncWorkspace();
}

String Ide::WorkspaceFile()
{
	String nm;
	for(const char *s = main; *s; s++)
		nm.Cat(*s == '\\' || *s == '/' ? '$' : *s);
	String cfg = ConfigFile("cfg");
	RealizeDirectory(cfg);
	return AppendFileName(cfg, ForceExt(nm + '@' + GetVarsName(), ".cfg"));
}

void Ide::SaveWorkspace()
{
	if(console.console) return;
	if(main.IsEmpty()) return;
	StoreToFile(THISBACK(SerializeWorkspace), WorkspaceFile());
}

void Ide::SyncMainConfigList()
{
	mainconfiglist.Clear();
	const Workspace& wspc = IdeWorkspace();
	if(wspc.GetCount() <= 0) return;
	const Array<Package::Config>& f = wspc.GetPackage(0).config;
	for(int i = 0; i < f.GetCount(); i++)
		mainconfiglist.Add(f[i].param, Nvl(f[i].name, f[i].param));
	SetMainConfigList();
}

void Ide::SetMainConfigList()
{
	mainconfiglist <<= mainconfigparam;
	mainconfigname = mainconfiglist.GetValue();
	mainconfiglist.Tip("Main configuration: " + mainconfigparam);
}

void Ide::OnMainConfigList()
{
	mainconfigparam = ~mainconfiglist;
	SetMainConfigList();
	MakeTitle();
}

void Ide::UscFile(const String& file)
{
	try {
		ParseUscFile(file);
	}
	catch(CParser::Error& e) {
		ShowConsole();
		console << e << "\n";
	}
}

void Ide::UscProcessDir(const String& dir)
{
	for(FindFile ff(AppendFileName(dir, "*.usc")); ff; ff.Next())
		UscFile(AppendFileName(dir, ff.GetName()));
}

void Ide::SyncUsc()
{
	CleanUsc();
	::Workspace wspc;
	wspc.Scan(main);
	int i;
	for(i = 0; i < wspc.GetCount(); i++) {
		const Package& p = wspc.GetPackage(i);
		for(int j = 0; j < p.file.GetCount(); j++) {
			String file = SourcePath(wspc[i], p.file[j]);
			if(ToLower(GetFileExt(file)) == ".usc")
				UscFile(file);
		}
	}
	UscProcessDir(GetLocalDir());
	UscProcessDir(GetFileFolder(ConfigFile("x")));
}

void Ide::SyncWorkspace()
{
	SyncUsc();
	SyncCodeBase();
}

bool IsTextFile(const String& file) {
	byte buffer[16384];
	FileIn fi(file);
	if(!fi.IsOpen())
		return false;
	int count = fi.Get(buffer, sizeof(buffer) - 1);
	buffer[count] = 0;
	const byte *end = buffer + count;
	const byte *q = buffer;
	const byte *x = q;
	while(q < end) {
		if(*q < 32) {
			int c = *q;
			if(c == '\n') {
				if(q - x > 2000) return false;
				x = q;
			}
			else
			if(c != '\r' && c != '\t' && c != '\v' && c != '\f' && c != 0x1a)
				return false;
		}
		q++;
	}
	return true;
}

Console& Ide::GetConsole()
{
	int q = btabs.GetCursor();
	return q == BCONSOLE2 ? console2 : console;
}

bool Ide::FindLineError(int l, Host& host) {
	String file;
	int lineno;
	int error;
	Console& c = GetConsole();
	if (FindLineError(c.GetUtf8Line(l), host, file, lineno, error)) {
		file = NormalizePath(file);
		editastext.FindAdd(file);
		EditFile(file);
		editor.SetCursor(editor.GetPos(editor.GetLineNo(lineno - 1)));
		editor.CenterCursor();
		editor.SetFocus();
		Sync();
		c.SetSelection(c.GetPos(l), c.GetPos(l + 1));
		if(btabs.GetCursor() != BCONSOLE && btabs.GetCursor() != BCONSOLE2)
			ShowConsole();
		return true;
	}
	return false;
}

bool Ide::FindLineError(String ln, Host& host, String& file, int& lineno, int& error) {
	Vector<String> wspc_paths;
	VectorMap<String, String> bm = GetMethodVars(method);
	bool is_java = (bm.Get("BUILDER", Null) == "JDK");
	const char *s = ln;
	error = ln.Find("error", 0) > 0 ? 1 : (ln.Find("warning", 0) > 0 ? 2 : 3);
	while(*s == ' ' || *s == '\t')
		s++;
	for(; s < ln.End(); s++) {
		if(*s != '\"' && (byte)*s >= 32 && *s != '(' && (file.GetLength() < 3 || *s != ':'))
			file.Cat(*s);
		else {
			if(*s == '\"') {
				file = Null;
				s++;
				while(*s && *s != '\"')
					file.Cat(*s++);
				if(*s)
					s++;
			}
			int e = file.GetLength();
			while(e > 0 && file[e - 1] == ' ')
				e--;
			file.Trim(e);
			file = TrimLeft(file);
			String upp = GetUppDir();
			file = host.GetLocalPath(file);
		#ifdef PLATFORM_WIN32
			if(file[0] == '\\' || file[0] == '/')
				file = String(upp[0], 1) + ':' + file;
		#endif
			if(!IsFullPath(file) && *file != '\\' && *file != '/') {
				if(wspc_paths.IsEmpty()) {
					::Workspace  wspc;
					wspc.Scan(main);
					for(int i = 0; i < wspc.GetCount(); i++)
						wspc_paths.Add(GetFileDirectory(PackagePath(wspc[i])));
				}
				for(int i = 0; i < wspc_paths.GetCount(); i++) {
					String path = AppendFileName(wspc_paths[i], file);
					FindFile ff;
					if(ff.Search(path) && ff.IsFile()) {
						file = path;
						break;
					}
				}
			}
			file = FollowCygwinSymlink(file);
			if(IsFullPath(file) && FileExists(file) && IsTextFile(file)) {
				while(*s && !IsDigit(*s))
					s++;
				lineno = 0;
				if(IsDigit(*s))
					lineno = stou(s);
				Vector<String> conf = SplitFlags(mainconfigparam, true);
				String uppout = GetVar("OUTPUT");
				int upplen = uppout.GetLength();
				if(is_java && file.GetLength() > upplen
				&& !MemICmp(file, uppout, upplen) && file[upplen] == DIR_SEP) { // check for preprocessed file
					FileIn fi(file);
					if(fi.IsOpen())
					{
						String fake_file = file;
						int fake_line = 1;
						int file_line = 1;
						while(!fi.IsEof())
						{
							String line = fi.GetLine();
							const char *p = line;
							if(p[0] == '/' && p[1] == '/' && p[2] == '#')
							{
								p += 3;
								if(p[0] == 'l' && p[1] == 'i' && p[2] == 'n' && p[3] == 'e')
									p += 4;
								while(*p == ' ' || *p == '\t')
									p++;
								if(IsDigit(*p))
								{
									fake_line = stou(p, &p);
									while(*p == ' ' || *p == '\t')
										p++;
									if(*p == '\"')
										p++;
									fake_file.Clear();
									while(*p && *p != '\"')
										if(*p == '/')
										{
											fake_file.Cat('/');
											if(p[1] == '/')
												p++;
											p++;
										}
										else
											fake_file.Cat(*p++);
								}
								file_line++;
								continue;
							}
							if(lineno <= file_line) {
								file = fake_file;
								lineno = fake_line;
								break;
							}
							file_line++;
							fake_line++;
						}
					}
				}
				if(lineno > 0)
					return true;
			}
			file.Clear();
		}
	}
	return false;
}

void Ide::FindError() {
	int l = GetConsole().GetLine(GetConsole().GetCursor());
	One<Host> host = CreateHost(false);
	FindLineError(l, *host);
}

void Ide::FindNextError() {
	int ln = GetConsole().GetLine(GetConsole().GetCursor());
	int l = ln;
	One<Host> host = CreateHost(false);
	for(l = ln; l < GetConsole().GetLineCount(); l++)
		if(FindLineError(l, *host)) return;
	for(l = 0; l < ln; l++)
		if(FindLineError(l, *host)) return;
}

void Ide::FindPrevError() {
	int ln = GetConsole().GetLine(GetConsole().GetCursor());
	int l = ln;
	One<Host> host = CreateHost(false);
	for(l = ln - 2; l >= 0; l--)
		if(FindLineError(l, *host)) return;
	for(l = GetConsole().GetLineCount() - 1; l > ln; l--)
		if(FindLineError(l, *host)) return;
}

void Ide::ClearErrorEditor()
{
	if(!mark_lines)
		return;

	for(int i = 0; i < filedata.GetCount(); i++) {
		ClearErrorEditor(filedata.GetKey(i));
	}
	
	SetErrorFiles(Vector<String>());
}

void Ide::ClearErrorEditor(String file)
{
	if(!mark_lines)
		return;
	if(file == editfile)
		editor.ClearErrors();
	else {
		FileData& fd = Filedata(file);
		ClearErrors(fd.lineinfo);
	}
}

void Ide::SetErrorEditor()
{
	if(!mark_lines)
		return;

	bool refresh = false;
	String file;
	int lineno;
	int error;
	One<Host> host = CreateHost(false);
	String    hfile;
	EditorBar hbar;
	Vector<String> errorfiles;
	for(int i = 0; i < console.GetLineCount(); i++) {
		if(FindLineError(console.GetUtf8Line(i), *host, file, lineno, error)) {
			file = NormalizePath(file);
		#ifdef PLATFORM_WIN32
			errorfiles.Add(ToLower(file));
		#else
			errorfiles.Add(file);
		#endif
			if(editfile == file) {
				editor.SetError(lineno - 1, error);
				refresh = true;
			}
			else {
				if(hfile != file) {
					if(hfile.GetCount())
						Filedata(hfile).lineinfo = hbar.GetLineInfo();
					hbar.SetLineInfo(Filedata(file).lineinfo, -1);
					hfile = file;
				}
				hbar.SetError(lineno - 1, error);
			}
		}
	}
	if(hfile.GetCount())
		Filedata(hfile).lineinfo = hbar.GetLineInfo();
	if(refresh)
		editor.RefreshFrame();
	SetErrorFiles(errorfiles);
}

void Ide::Renumber() {
	for(int i = 0; i < filedata.GetCount(); i++)
		::Renumber(filedata[i].lineinfo);
	editor.Renumber();
}

void Ide::CycleFiles()
{
	if(++tabi >= tablru.GetCount())
		tabi = 0;
	if(tabi < tablru.GetCount()) {
		blocktabs = true;
		EditFile(tablru[tabi]);
	}
}

void Ide::Deactivate()
{
	if(deactivate_save) {
		DeactivationSave(true);
		SaveFile();
		DeactivationSave(false);
	}
}

bool Ide::Key(dword key, int count) {
	dword *k = IdeKeys::AK_DELLINE().key;
	if(key == k[0] || key == k[1]) {
		editor.DeleteLine();
		return true;
	}
	k = IdeKeys::AK_CUTLINE().key;
	if(key == k[0] || key == k[1]) {
		editor.CutLine();
		return true;
	}
	switch(key) {
	case K_SHIFT_CTRL_UP:
	case K_SHIFT_CTRL_DOWN:
	case K_ALT_DELETE:
		return filelist.Key(key, count);
	case K_ALT_UP:
		return filelist.Key(K_UP, 0);
	case K_ALT_DOWN:
		return filelist.Key(K_DOWN, 0);
	case K_ALT_PAGEUP:
		return package.Key(K_UP, 0);
	case K_ALT_PAGEDOWN:
		return package.Key(K_DOWN, 0);
	case K_CTRL|K_ALT_LEFT:
		TabsLR(-1);
		return true;
	case K_CTRL|K_ALT_RIGHT:
		TabsLR(1);
		return true;
	case K_SHIFT|K_CTRL_O:
		AddFile(WorkspaceWork::ANY_FILE);
		return true;
	case K_CTRL_KEY|K_KEYUP:
		if(tabi) {
			tabi = 0;
			AddLru();
		}
		return true;
	case K_CTRL_TAB:
		CycleFiles();
		return true;
	case K_ALT_RIGHT:
	default:
		if(key >= K_SHIFT_CTRL_0 && key <= K_SHIFT_CTRL_9) {
			Bookmark& b = bookmark[key - K_SHIFT_CTRL_0];
			b.file = editfile;
			b.pos = editor.GetEditPos();
			MakeTitle();
			return true;
		}
		if(key >= K_CTRL_0 && key <= K_CTRL_9) {
			GotoBookmark(bookmark[key - K_CTRL_0]);
			return true;
		}
	}
	return false;
}

void Ide::GotoBookmark(const Bookmark& b)
{
	if(b.file.IsEmpty()) return;
	EditFile(b.file);
	if(bookmark_pos)
		editor.SetEditPos(b.pos);
}

bool Ide::IsHistDiff(int i)
{
	Bookmark& b = history[i];
	return b.file != editfile || abs(editor.GetCursor() - b.pos.cursor) > 200;
}

void Ide::AddHistory()
{
	if(history.GetCount() && histi - 1 >= 0 && histi <= history.GetCount()) {
		if(IsHistDiff(histi - 1))
			++histi;
	}
	else
		histi = 1;
	history.SetCount(histi);
	Bookmark& b = history.Top();
	b.file = editfile;
	b.pos = editor.GetEditPos();
	TouchFile(editfile);
}

void Ide::HistoryBk()
{
	while(histi > 0 && --histi < history.GetCount())
		if(IsHistDiff(histi)) {
			GotoBookmark(history[histi]);
			break;
		}
}

void Ide::HistoryFw()
{
	if(histi < history.GetCount() - 1 && ++histi >= 0)
		GotoBookmark(history[histi]);
}

void Ide::BookKey(int key) {
	Key(key, 1);
}

void Ide::Display() {
	Point p = editor.GetColumnLine(editor.GetCursor());
	display.SetLabel(Format("Ln %d, Col %d", p.y + 1, p.x + 1));
}

void Ide::SetIdeState(int newstate) {
	if(newstate != idestate)
	{
		if(newstate == BUILDING)
			build_start_time = GetSysTime();
		else
		{
			if(idestate == BUILDING && !IsNull(build_start_time))
				stat_build_time += int(GetSysTime() - build_start_time);
			build_start_time = Null;
		}
	}
	idestate = newstate;
	MakeTitle();
	SetBar();
}

void Ide::SetIcon()
{
	int new_state_icon = 0;
	if((bool)debugger && !IdeIsDebugLock()) {
		new_state_icon = 1;
		return;
	}
	else
	if((GetTimeClick() / 800) & 1)
		if(debugger)
			new_state_icon = 2;
		else
		if(idestate == BUILDING)
			new_state_icon = 3;
	if(state_icon == new_state_icon)
		return;
	state_icon = new_state_icon;
	switch(state_icon) {
	case 1:  Icon(IdeImg::IconDebugging()); break;
	case 2:  Icon(IdeImg::IconRunning()); break;
	case 3:  Icon(IdeImg::IconBuilding()); break;
	default: Icon(IdeImg::Package());
	}
}

void Ide::Periodic()
{
	CheckFileUpdate();
	SetIcon();
	if(debugger && debugger->IsFinished() && !IdeIsDebugLock())
		IdeEndDebug();
}

const Workspace& Ide::IdeWorkspace() const
{
	static Workspace wspc;
	static String _main;
	if(main != _main || wspc.GetCount() == 0) {
		wspc.Scan(main);
		_main = main;
	}
	else {
		for(int i = 0; i < wspc.GetCount(); i++)
			if(wspc.GetPackage(i).time != FileGetTime(PackagePath(wspc[i]))) {
				wspc.Scan(main);
				break;
			}
	}
	return wspc;
}

int Ide::GetPackageIndex()
{
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		if(wspc[i] == package.GetCurrentName())
			return i;
	return -1;
}


void Ide::Diff()
{
	if(IsNull(editfile))
		return;
	FileDiff diffdlg(AnySourceFs());
	diffdlg.Execute(editfile);
}

void Ide::Patch()
{
	if(IsNull(editfile))
		return;
	PatchDiff diffdlg(AnySourceFs());
	diffdlg.Execute(editfile);
}

void Ide::SvnHistory()
{
	if(IsNull(editfile))
		return;
	RunSvnDiff(editfile);
}
