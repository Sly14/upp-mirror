#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

#ifdef GUI_WIN
void AvoidPaintingCheck__();

struct FileIconMaker : ImageMaker {
	String file;
	bool   exe;
	bool   dir;
	bool   large;

	virtual String Key() const {
		return file + (exe ? "1" : "0") + (dir ? "1" : "0");
	}

	virtual Image Make() const {
		Color c = White();
		Image m[2];
		for(int i = 0; i < 2; i++) {
			SHFILEINFO info;
			AvoidPaintingCheck__();
			SHGetFileInfo(ToSystemCharset(file), dir ? FILE_ATTRIBUTE_DIRECTORY : FILE_ATTRIBUTE_NORMAL,
			              &info, sizeof(info),
			              SHGFI_ICON|(large ? SHGFI_LARGEICON : SHGFI_SMALLICON)|(exe ? 0 : SHGFI_USEFILEATTRIBUTES));
			HICON icon = info.hIcon;
			ICONINFO iconinfo;
			if(!icon || !GetIconInfo(icon, &iconinfo))
				return Image();
			BITMAP bm;
			::GetObject((HGDIOBJ)iconinfo.hbmMask, sizeof(BITMAP), (LPVOID)&bm);
			Size sz(bm.bmWidth, bm.bmHeight);
			ImageDraw iw(sz);
			iw.DrawRect(sz, c);
			::DrawIconEx(iw.GetHandle(), 0, 0, info.hIcon, 0, 0, 0, NULL, DI_NORMAL|DI_COMPAT);
			::DeleteObject(iconinfo.hbmColor);
			::DeleteObject(iconinfo.hbmMask);
			::DestroyIcon(info.hIcon);
			c = Black();
			m[i] = iw;
		}
		return RecreateAlpha(m[0], m[1]);
	}
};


Image GetFileIcon(const char *path, bool dir, bool force, bool large, bool quick = false)
{
	FileIconMaker m;
	String ext = GetFileExt(path);
	m.exe = false;
	m.dir = false;
	m.file = path;
	m.large = large;
	if(force)
		m.exe = true;
	else
	if(dir) {
		m.dir = true;
		m.exe = true;
	}
	else
	if(ext == ".exe")
		m.exe = true;
	else
		m.file = "x." + ext;
	if(quick) {
		m.exe = false;
		m.file = "x." + ext;
	}
	return MakeImage(m);
}

#define GETFILEICON_DEFINED

#endif

#if defined(GUI_X11) && !defined(flagNOGTK)

Image GtkThemeIcon(const char *name, bool large);

Image GnomeImage(const char *s, bool large = false)
{
	return GtkThemeIcon(String("gnome-") + s, large);
}

struct ExtToMime {
	Index<String> major;
	Index<String> minor;
	VectorMap<String, dword> map;

	void Load(const char *file);
	void Refresh();
	bool GetMime(const String& ext, String& maj, String& min);
};

void ExtToMime::Load(const char *fn)
{
	FileIn in(fn);
	if(in)
		while(!in.IsEof()) {
			String ln = TrimLeft(TrimRight(in.GetLine()));
			if(ln[0] != '#') {
				int q = ln.Find(':');
				if(q >= 0) {
					String h = ln.Mid(0, q);
					int w = h.Find('/');
					if(w >= 0) {
						int x = ln.Find("*.", q);
						if(x >= 0) {
							String ext = ln.Mid(x + 2);
							if(ext.GetCount() && map.Find(ext) < 0)
								map.Add(ext, MAKELONG(minor.FindAdd(h.Mid(w + 1)), major.FindAdd(h.Mid(0, w))));
						}
					}
				}
			}
		}
}

void ExtToMime::Refresh()
{
	major.Clear();
	minor.Clear();
	map.Clear();
	Load("/usr/local/share/mime/globs");
	Load("/usr/share/mime/globs");
}

bool ExtToMime::GetMime(const String& ext, String& maj, String& min)
{
	ONCELOCK {
		Refresh();
	}
	int q = map.Find(ext);
	if(q < 0)
		return false;
	dword x = map[q];
	maj = major[HIWORD(x)];
	min = minor[LOWORD(x)];
	return true;
}

struct FileExtMaker : ImageMaker {
	String ext;
	bool   large;

	virtual String Key() const {
		return ext;
	}

	virtual Image Make() const {
		String major;
		String minor;
		if(!Single<ExtToMime>().GetMime(ext, major, minor))
			return Null;
		Image img = GnomeImage("mime-" + major + '-' + minor, large);
		return IsNull(img) ? GnomeImage("mime-" + major) : img;
	}
};

Image PosixGetDriveImage(String dir, bool large)
{
	static Image cdrom = GnomeImage("dev-cdrom");
	static Image lcdrom = GnomeImage("dev-cdrom", true);
	static Image harddisk = GnomeImage("dev-harddisk");
	static Image lharddisk = GnomeImage("dev-harddisk", true);
	static Image floppy = GnomeImage("dev-floppy");
	static Image lfloppy = GnomeImage("dev-floppy", true);
	static Image computer = GnomeImage("dev-computer");
	static Image lcomputer = GnomeImage("dev-computer", true);
	if(dir.GetCount() == 0 || dir == "/") {
		Image m = large ? lcomputer : computer;
		return IsNull(m) ? CtrlImg::Computer() : m;
	}
	if(dir.Find("cdrom") == 0 || dir.Find("cdrecorder") == 0) {
		Image m = large ? lcdrom : cdrom;
		return IsNull(m) ? CtrlImg::CdRom() : m;
	}
	if(dir.Find("floppy") == 0 || dir.Find("zip") == 0) {
		Image m = large ? lfloppy : floppy;
		return IsNull(m) ? CtrlImg::Diskette() : m;
	}
	Image m = large ? lharddisk : harddisk;
	return IsNull(m) ? CtrlImg::Hd() : m;
}

Image GetFileIcon(const String& folder, const String& filename, bool isdir, bool isexe, bool large)
{
	static Image file = GnomeImage("fs-regular");
	static Image lfile = GnomeImage("fs-regular", true);
	static Image dir = GnomeImage("fs-directory");
	static Image ldir = GnomeImage("fs-directory", true);
	static Image exe = GnomeImage("fs-executable");
	static Image lexe = GnomeImage("fs-executable", true);
	static Image home = GnomeImage("fs-home");
	static Image lhome = GnomeImage("fs-home", true);
	static Image desktop = GnomeImage("fs-desktop");
	static Image ldesktop = GnomeImage("fs-desktop", true);
	if(isdir) {
		Image img = dir;
		if(AppendFileName(folder, filename) == GetHomeDirectory())
			return large ? lhome : home;
		else
		if(folder == GetHomeDirectory() && filename == "Desktop")
			return large ? ldesktop : desktop;
		else
		if(folder == "/media" || filename.GetCount() == 0)
			return PosixGetDriveImage(filename, large);
		return dir;
	}
	FileExtMaker m;
	m.ext = GetFileExt(filename);
	Image img;
	if(m.ext.GetCount()) {
		m.ext = m.ext.Mid(1);
		m.large = large;
		img = MakeImage(m);
	}
	return IsNull(img) ? isexe ? (large ? lexe : exe) : (large ? lfile : file) : img;
}

#define GETFILEICON_DEFINED

#endif

#ifndef GETFILEICON_DEFINED
Image PosixGetDriveImage(String dir, bool)
{
	if(dir.GetCount() == 0 || dir == "/")
		return CtrlImg::Computer();
	if(dir.Find("cdrom") == 0 || dir.Find("cdrecorder") == 0)
		return CtrlImg::CdRom();
	if(dir.Find("floppy") == 0 || dir.Find("zip") == 0)
		return CtrlImg::Diskette();
	return CtrlImg::Hd();
}

#ifdef PLATFORM_WIN32
Image GetFileIcon(const String& folder, bool, bool, bool, bool = false)
{
	return Null;
}
#else
Image GetFileIcon(const String& folder, const String& filename, bool isdir, bool isexe, bool = false)
{
	return Null;
}

#endif
#endif

Image NativePathIcon0(const char *path, bool folder, bool large)
{
#if defined(PLATFORM_WIN32)
	if (folder)
		return GetFileIcon(path, true, true, large);
	else
		return GetFileIcon(path, false, false, large);
#endif
#ifdef PLATFORM_POSIX
	String p = path;
	bool isdrive = folder && ((p == "/media") || (p == "/mnt"));
	FindFile ff(path);
	return isdrive ? PosixGetDriveImage(GetFileName(path), large)
				   : GetFileIcon(path, GetFileName(path), folder, ff.GetMode() & 0111, large);
#endif
}

Image NativePathIcon(const char *path, bool folder)
{
	return NativePathIcon0(path, folder, false);
}

Image NativePathIcon(const char *path)
{
	FindFile ff(path);
	return NativePathIcon(path, ff.IsFolder());
}

Image NativePathLargeIcon(const char *path, bool folder)
{
	return NativePathIcon0(path, folder, true);
}

Image NativePathLargeIcon(const char *path)
{
	FindFile ff(path);
	return NativePathLargeIcon(path, ff.IsFolder());
}

bool MatchSearch(const String& filename, const String& search)
{
	return search.GetCount() ? Filter(filename, CharFilterDefaultToUpperAscii).Find(search) >= 0 : true;
}

bool Load(FileList& list, const String& dir, const char *patterns, bool dirs,
          Callback3<bool, const String&, Image&> WhenIcon, FileSystemInfo& filesystem,
          const String& search, bool hidden, bool hiddenfiles, bool lazyicons)
{
	if(dir.IsEmpty()) {
		Array<FileSystemInfo::FileInfo> root = filesystem.Find(Null);
		for(int i = 0; i < root.GetCount(); i++)
			if(MatchSearch(root[i].filename, search))
				list.Add(root[i].filename,
			#ifdef PLATFORM_WIN32
					GetFileIcon(root[i].filename, false, true, false),
			#else
					GetDriveImage(root[i].root_style),
			#endif
					StdFont().Bold(), SColorText, true, -1, Null, SColorDisabled,
					root[i].root_desc, StdFont()
			);
		#ifdef PLATFORM_WIN32
			list.Add(t_("Network"), CtrlImg::Network(), StdFont().Bold(), SColorText,
			         true, -1, Null, SColorDisabled, Null, StdFont());
		#endif
	}
	else {
		Array<FileSystemInfo::FileInfo> ffi =
			filesystem.Find(AppendFileName(dir, filesystem.IsWin32() ? "*.*" : "*"));
		if(ffi.IsEmpty())
			return false;
	#ifdef PLATFORM_POSIX
		bool isdrive = dir == "/media" || dir == "/mnt";
	#endif
		for(int t = 0; t < ffi.GetCount(); t++) {
			const FileSystemInfo::FileInfo& fi = ffi[t];
			bool nd = dirs && !fi.is_directory;
			bool show = hidden;
			if(!show && filesystem.IsWin32() ? !fi.is_hidden : fi.filename[0] != '.')
				show = true;
			if(!show && hiddenfiles && fi.is_file)
				show = true;
			if(fi.filename != "." && fi.filename != ".." != 0 &&
			   (fi.is_directory || PatternMatchMulti(patterns, fi.filename)) &&
			   MatchSearch(fi.filename, search) && show) {
				Image img;
			#ifdef GUI_X11
				img = isdrive ? PosixGetDriveImage(fi.filename, false)
				              : GetFileIcon(dir, fi.filename, fi.is_directory, fi.unix_mode & 0111, false);
			#endif
			#ifdef GUI_WIN32
//				Image img = lazyicons ? fi.is_directory ? CtrlImg::Dir() : CtrlImg::File()
//				                      : GetFileIcon(AppendFileName(dir, fi.filename), fi.is_directory, fi.unix_mode & 0111, false);
				img = GetFileIcon(AppendFileName(dir, fi.filename), fi.is_directory, false, false, lazyicons);
			#endif
				if(IsNull(img))
					img = fi.is_directory ? CtrlImg::Dir() : CtrlImg::File();
				WhenIcon(fi.is_directory, fi.filename, img);
				list.Add(fi.filename, fi.is_hidden ? Contrast(img, 200) : img,
						 StdFont().Bold(fi.is_directory),
						 nd ? SColorDisabled : fi.is_hidden ? Blend(SColorText, Gray, 200) : SColorText, fi.is_directory,
						 fi.is_directory ? -1 : fi.length,
						 Null, nd ? SColorDisabled
						          : fi.is_directory ? SColorText
						                            : fi.is_hidden ? Blend(SColorMark, Gray, 200)
						                                           : SColorMark,
				         Null, Null, Null, Null,
#ifdef PLATFORM_WIN32
                         false,
#else
						 fi.unix_mode & 0111,
#endif
				         fi.is_hidden
				);
			}
		}
	}
	return true;
}

void LazyFileIcons::Do()
{
	if(Ctrl::IsWaitingEvent()) {
		Restart(5);
		return;
	}
	int tm = GetTickCount();
	while(pos < ndx.GetCount()) {
		if((int)GetTickCount() - tm > ptime) {
			ptime = 50;
			Restart(0);
			return;
		}
		int ii = ndx[pos];
		if(ii < 0 || ii >= list->GetCount())
			return;
		const FileList::File& f = list->Get(ii);
		int t0 = GetTickCount();
#ifdef PLATFORM_WIN32
		Image img = GetFileIcon(AppendFileName(dir, f.name), f.isdir, f.unixexe, false, quick);
#else
		Image img = GetFileIcon(dir, f.name, f.isdir, f.unixexe, false);
#endif
		WhenIcon(f.isdir, f.name, img);
		if(f.hidden)
			img = Contrast(img, 200);
		list->SetIcon(ii, img);
		if(GetTickCount() - t0 > 700)
			return;
		pos++;
	}
}

void LazyFileIcons::ReOrder()
{
	ndx.Clear();
	Vector<int> len;
	for(int i = 0; i < list->GetCount(); i++) {
		const FileList::File& f = list->Get(i);
		if(ToLower(GetFileExt(f.name)) == ".exe" && !f.isdir) {
			ndx.Add(i);
			len.Add((int)min((int64)INT_MAX, f.length));
		}
	}
	IndexSort(len, ndx);
	Restart(0);
}

void LazyFileIcons::Start(FileList& list_, const String& dir_, Callback3<bool, const String&, Image&> WhenIcon_)
{
	list = &list_;
	dir = dir_;
	WhenIcon = WhenIcon_;
	pos = 0;
	quick = false;
	ptime = 150;
	start = GetTickCount();
	ReOrder();
}

class FileListSortName : public FileList::Order {
public:
	virtual bool operator()(const FileList::File& a, const FileList::File& b) const {
		if(a.isdir != b.isdir)
			return a.isdir;
		if(a.name == "..")
			return b.name != "..";
		if(b.name == "..")
			return false;
		return stricmp(a.name, b.name) < 0;
	}
};

void SortByName(FileList& list)
{
	list.Sort(FileListSortName());
}

class FileListSortExt : public FileList::Order {
public:
	virtual bool operator()(const FileList::File& a, const FileList::File& b) const {
		if(a.isdir != b.isdir)
			return a.isdir;
		if(a.name == "..")
			return b.name != "..";
		if(b.name == "..")
			return false;
		const char *ae = strrchr(a.name, '.');
		const char *be = strrchr(b.name, '.');
		int q;
		if(ae == NULL || be == NULL) {
			if(ae) return false;
			if(be) return true;
			q = 0;
		}
		else
			q = stricmp(ae, be);
		return (q ? q : stricmp(a.name, b.name)) < 0;
	}
};

void SortByExt(FileList& list)
{
	list.Sort(FileListSortExt());
}

String FileSel::GetDir() {
	String s = ~dir;
	if(s.IsEmpty()) return basedir;
	if(basedir.IsEmpty()) return s;
	return AppendFileName(basedir, s);
}

void FileSel::SetDir(const String& _dir) {
#ifdef PLATFORM_WIN32
	netstack.Clear();
	netnode.Clear();
#endif
	dir <<= _dir;
	Load();
	Update();
}

String FileSel::FilePath(const String& fn) {
	return AppendFileName(GetDir(), fn);
}

Image GetDriveImage(char drive_style)
{
	switch(drive_style)
	{
	case FileSystemInfo::ROOT_NO_ROOT_DIR: return Null;
	case FileSystemInfo::ROOT_COMPUTER:  return CtrlImg::Computer();
	case FileSystemInfo::ROOT_REMOVABLE: return CtrlImg::Diskette();
	case FileSystemInfo::ROOT_CDROM:     return CtrlImg::CdRom();
	default:                             return CtrlImg::Hd();
	}
}

String FileSel::GetMask()
{
	String emask = "*";
	if(!IsNull(type)) {
		if(IsString(~type))
			emask = ~type;
		else {
			int q = ~type;
			if(q >= 0 && q < mask.GetCount())
				emask = mask[q];
		}
	}
	return emask;
}

void FileSel::Load()
{
	search <<= Null;
	SearchLoad();
}

void FileSel::LoadNet()
{
#ifdef PLATFORM_WIN32
	list.Clear();
	for(int i = 0; i < netnode.GetCount(); i++) {
		Image m = CtrlImg::Group();
		switch(netnode[i].GetDisplayType()) {
		case NetNode::NETWORK:
			m = CtrlImg::Network();
			break;
		case NetNode::SHARE:
			m = CtrlImg::Share();
			break;
		case NetNode::SERVER:
			m = CtrlImg::Computer();
			break;
		}
		list.Add(netnode[i].GetName(), m);
	}
	places.FindSetCursor("\\");
#endif
}

void FileSel::SelectNet()
{
#ifdef PLATFORM_WIN32
	int q = list.GetCursor();
	if(q >= 0 && q < netnode.GetCount()) {
		NetNode& n = netnode[q];
		String p = n.GetPath();
		if(p.GetCount())
			SetDir(p);
		else {
			netstack.Add() = netnode[q];		
			netnode = netstack.Top().Enum();
			LoadNet();
		}
	}
#endif
}

void FileSel::SearchLoad()
{
	list.EndEdit();
	list.Clear();
	String d = GetDir();
#ifdef PLATFORM_WIN32
	if(d == "\\") {
		netnode = NetNode::EnumRoot();
		netnode.Append(NetNode::EnumRemembered());
		LoadNet();
		return;
	}
#endif
	String emask = GetMask();
	if(!UPP::Load(list, d, emask, mode == SELECTDIR, WhenIcon, *filesystem, ~search, ~hidden, ~hiddenfiles, true)) {
		Exclamation(t_("[A3* Unable to read the directory !]&&") + DeQtf((String)~dir) + "&&" +
		            GetErrorMessage(GetLastError()));
		if(!basedir.IsEmpty() && String(~dir).IsEmpty()) {
			Break(IDCANCEL);
			return;
		}
		dir <<= olddir;
		olddir = Null;
		SearchLoad();
	}

	places.KillCursor();
	if(d.GetCount())
		places.FindSetCursor(d);
	hiddenfiles.Enable(!hidden);
	if(d.IsEmpty()) {
		if(filesystem->IsWin32()) {
			mkdir.Disable();
			plus.Disable();
			minus.Disable();
			toggle.Disable();
			list.Renaming(false);
		}
		dir <<= d;
		dirup.Disable();
	}
	else {
		dirup.Enable();
		mkdir.Enable();
		plus.Enable();
		minus.Enable();
		toggle.Enable();
		list.Renaming(true);
	}
	if(filesystem->IsPosix())
		if(d == "/" || !IsEmpty(basedir) && String(~dir).IsEmpty())
			dirup.Disable();
	if(filesystem->IsWin32())
		if(!IsEmpty(basedir) && String(~dir).IsEmpty())
			dirup.Disable();
	olddir = ~dir;
	if(olddir.GetCount() || basedir.GetCount())
		if(sortext && mode != SELECTDIR)
			SortByExt(list);
		else
			SortByName(list);
	Update();
#ifdef PLATFORM_WIN32
	lazyicons.Start(list, d, WhenIcon);
#endif
}

String TrimDot(String f) {
	int i = f.Find('.');
	if(i >= 0 && i == f.GetLength() - 1)
		f.Trim(i);
	return f;
}

void FileSel::AddName(Vector<String>& fn, String& f) {
	if(!f.IsEmpty()) {
		if(f.Find('.') < 0) {
			String t = GetMask();
			int q = t.Find('.');
			if(q >= 0 && IsAlNum(t[q + 1])) {
				int w = q + 2;
				while(IsAlNum(t[w]))
					w++;
				f << t.Mid(q, w - q);
			}
			else
			if(defext.GetCount())
				f << '.' << defext;
		}
		f = TrimDot(f);
		if(f[0] == '\"' && f.GetCount() > 2)
			fn.Add(f.Mid(1, f.GetCount() - 2));
		else
			fn.Add(f);
	}
	f.Clear();
}

void FileSel::Finish() {
	if(filesystem->IsWin32())
		if(GetDir().IsEmpty()) {
			file.Clear();
			return;
		}
	fn.Clear();
	if(mode == SELECTDIR) {
		String p = GetDir();
		if(list.GetCursor() >= 0) {
			const FileList::File& m = list[list.GetCursor()];
			if(m.isdir)
				p = AppendFileName(p, m.name);
		}
		fn.Add(p);
		Break(IDOK);
		return;
	}
	String f = file.GetText().ToString();
	if(f.IsEmpty()) return;
	String o;
	if(mode == OPEN && IsMulti()) {
		for(const char *s = f; *s; s++) {
			if(*s == ' ')
				AddName(fn, o);
			else
			if(*s == '\"') {
				AddName(fn, o);
				s++;
				for(;;) {
					if(*s == '\0')
						AddName(fn, o);
					else
					if(*s == '\"') {
						AddName(fn, o);
						break;
					}
					o.Cat(*s++);
				}
			}
			else
				o.Cat(*s);
		}
		AddName(fn, o);
	}
	else {
		o = f;
		AddName(fn, o);
	}
	if(!IsMulti() && fn.GetCount())
		fn.SetCount(1);
	String d = GetDir();
	String nonexist;
	int ne = 0;
	for(int i = 0; i < fn.GetCount(); i++) {
		String p = fn[i];
		if(!IsFullPath(p))
			p = NormalizePath(AppendFileName(d, fn[i]));
		Array<FileSystemInfo::FileInfo> ff = filesystem->Find(p, 1);
		p = DeQtf(p);
		if(!ff.IsEmpty() && ff[0].is_directory) {
			Exclamation(p + t_(" is directory."));
			return;
		}
		if(asking)
			if(mode == SAVEAS) {
				if(!ff.IsEmpty() && !PromptOKCancel(p + t_(" already exists.&Do you want to continue ?")))
					return;
			}
			else
			if(ff.IsEmpty()) {
				if(ne) nonexist << '&';
				nonexist << p;
				ne++;
			}
	}
	if(ne) {
		nonexist << (ne == 1 ? t_(" does not exist.") : t_("&do not exist."));
		if(!PromptOKCancel(nonexist + t_("&Do you want to continue ?")))
			return;
	}
	Break(IDOK);
}

bool FileSel::OpenItem() {
	if(list.IsCursor()) {
	#ifdef PLATFORM_WIN32
		if(netnode.GetCount()) {
			SelectNet();
			return true;
		}
	#endif
		const FileList::File& m = list.Get(list.GetCursor());
	#ifdef PLATFORM_WIN32
		if(IsNull(dir) && m.name == t_("Network")) {
			netnode = NetNode::EnumRoot();
			netnode.Append(NetNode::EnumRemembered());
			LoadNet();
			return true;
		}
	#endif
		if(m.isdir) {
			SetDir(AppendFileName(~dir, m.name));
			return true;
		}
	}
	if(mode != SELECTDIR)
		Finish();
	return false;
}

void FileSel::Open() {
	if(mode == SELECTDIR) {
	#ifdef PLATFORM_WIN32
		if(netnode.GetCount())
			return;
	#endif
		Finish();
		return;
	}
	if(list.HasFocus() || type.HasFocus()) {
		if(OpenItem()) list.SetCursor(0);
	}
	else
	if(list.IsSelection())
		for(int i = 0; i < list.GetCount(); i++) {
			const FileList::File& m = list[i];
			if(!m.isdir) Finish();
		}
	else
	if(file.HasFocus()) {
		String fn = file.GetText().ToString();
	#ifdef PLATFORM_WIN32
		if(fn[0] == '\\' && fn[1] == '\\') {
			FindFile ff(AppendFileName(fn, "*.*"));
			if(ff)
				SetDir(TrimDot(fn));
			return;
		}
	#endif
		if(fn == "." || fn == "..") {
			DirUp();
			return;
		}
		if(HasWildcards(fn)) {
			file.Clear();
			int q = FindIndex(mask, fn);
			if(q >= 0)
				type.SetIndex(q);
			else {
				type.Add(fn, t_("Custom file type (") + fn + ')');
				type.SetIndex(type.GetCount() - 1);
			}
			Load();
			return;
		}
		if(fn.Find(' ') < 0 && fn.Find('\"') < 0) {
			if(filesystem->IsWin32())
			{
				if(fn.GetLength() >= 2 && fn[1] == ':' && fn.GetLength() <= 3) {
					fn.Set(0, ToUpper(fn[0]));
					if(fn.GetLength() == 2)
						fn.Cat('\\');
					SetDir(fn);
					return;
				}
			}
			if(!IsFullPath(fn))
				fn = AppendFileName(~dir, fn);
			if(filesystem->IsWin32() && (!fn.IsEmpty() && (*fn.Last() == '\\' || *fn.Last() == '/'))
			|| filesystem->IsPosix() && (fn != "/" && (*fn.Last() == '\\' || *fn.Last() == '/')))
			{
				fn.Trim(fn.GetLength() - 1);
				SetDir(TrimDot(fn));
				return;
			}
			Array<FileSystemInfo::FileInfo> ff = filesystem->Find(fn, 1);
			if(!ff.IsEmpty()) {
				if(ff[0].is_directory) {
					SetDir(TrimDot(fn));
					return;
				}
				else {
					SetDir(TrimDot(GetFileFolder(fn)));
					file.SetText(GetFileName(fn).ToWString());
				}
			}
		}
		if(mode != SELECTDIR) Finish();
	}
}

String DirectoryUp(String& dir, bool basedir)
{
	while(*dir.Last() == '\\' || *dir.Last() == '/')
		dir.Trim(dir.GetCount() - 1);
	String s = dir;
	String name;
#ifdef PLATFORM_WIN32
	if(s.GetLength() < 3 || s.GetLength() == 3 && s[1] == ':') {
		dir.Clear();
		name = s;
	}
	else
#endif
#ifdef PLATFORM_POSIX
	if(s != "/")
#endif
	{
#ifdef PLATFORM_WIN32
		int i = max(s.ReverseFind('/'), s.ReverseFind('\\'));
#endif
#ifdef PLATFORM_POSIX
		int i = s.ReverseFind('/');
#endif
		if(basedir)
			if(i < 0)
				dir.Clear();
			else {
				dir = s.Mid(0, i);
				name = s.Mid(i + 1);
			}
		else {
#ifdef PLATFORM_WIN32
			if(s.GetLength() && s[1] == ':')
				if(i > 3) {
					dir = s.Mid(0, i);
					name = s.Mid(i + 1);
				}
				else {
					dir = s.Mid(0, 3);
					name = s.Mid(3);
				}
			if(s.GetLength() && s[0] == DIR_SEP && s[1] == DIR_SEP)
				if(i > 2) {
					dir = s.Mid(0, i);
					name = s.Mid(i + 1);
				}
				else {
					dir.Clear();
					name = s;
				}
#endif
#ifdef PLATFORM_POSIX
			if(i == 0 && s.GetLength() > 1) {
				dir = "/";
				name = s.Mid(1);
			}
			else
			if(s.GetLength() && s[0] == '/' && s[1] != '/') {
				dir = s.Mid(0, i);
				name = s.Mid(i + 1);
			}
#endif
		}
	}
	return name;
}

void FileSel::DirUp() {
#ifdef PLATFORM_WIN32
	if(netstack.GetCount()) {
		netstack.Drop();
		if(netstack.GetCount()) {
			netnode = netstack.Top().Enum();
			LoadNet();
		}
		netnode = NetNode::EnumRoot();
		return;
	}
	if(netnode.GetCount()) {
		netnode.Clear();
		SetDir("");
		return;
	}
#endif
	String s = ~dir;
	String name = DirectoryUp(s, !basedir.IsEmpty());
#ifdef PLATFORM_WIN32
	if(s[0] == '\\' && s[1] == '\\' && s.Find('\\', 2) < 0) {
		s.Clear();
		name.Clear();
	}
#endif
	SetDir(s);
	if(list.HasFocus())
		list.FindSetCursor(name);
}

void FileSel::MkDir() {
	if(String(~dir).IsEmpty() && basedir.IsEmpty()) return;
	String name, error;
	if(EditText(name, t_("New directory"), t_("Name")) && !name.IsEmpty())
		if(filesystem->CreateFolder(FilePath(name), error))
		{
			Load();
			list.FindSetCursor(name);
		}
		else
			Exclamation(t_("[A3* Creating directory failed !&&]") + error);
}

void FileSel::PlusMinus(const char *title, bool sel) {
	String pattern;
	if(EditText(pattern, title, t_("Mask")) && !pattern.IsEmpty())
		for(int i = 0; i < list.GetCount(); i++)
			if(!list.Get(i).isdir)
				if(PatternMatchMulti(pattern, list.Get(i).name))
					list.SelectOne(i, sel);
}

void FileSel::Plus() {
	PlusMinus(t_("Add to selection"), true);
}

void FileSel::Minus() {
	PlusMinus(t_("Remove from selection"), false);
}

void FileSel::Toggle() {
	for(int i = 0; i < list.GetCount(); i++)
		if(!list.Get(i).isdir)
			list.SelectOne(i, !list.IsSelected(i));
}

bool FileSel::Key(dword key, int count) {
	switch(key) {
	case '.':
	case K_CTRL_UP:
		list.SetFocus();
		dirup.PseudoPush();
		return true;
	case '+':
		plus.PseudoPush();
		return true;
	case '-':
		minus.PseudoPush();
		return true;
	case '*':
		toggle.PseudoPush();
		return true;
	case K_F6:
		list.StartEdit();
		return true;
	case K_F7:
		mkdir.PseudoPush();
		return true;
	case K_ENTER:
		if(mode == SELECTDIR && OpenItem())
			return true;
		break;
	case K_UP:
	case K_DOWN:
	case K_PAGEUP:
	case K_PAGEDOWN:
		list.SetFocus();
		return list.Key(key, count);
	}
	if(CharFilterDefaultToUpperAscii(key) || key == K_BACKSPACE)
		return search.Key(key, count);
	return TopWindow::Key(key, count);
}

void Catq(String& s, const String& fn) {
	if(!s.IsEmpty())
		s << ' ';
	if(fn.Find(' ') >= 0)
		s << '"' << fn << '"';
	else
		s << fn;
}

String FormatFileSize(int64 n)
{
	if(n < 10000)
		return Format("%d B  ", n);
	else
	if(n < 10000 * 1024)
		return Format("%d K  ", n >> 10);
	else
	if(n < I64(10000000) * 1024)
		return Format("%d M  ", n >> 20);
	else
		return Format("%d G  ", n >> 30);
}

void FileSel::Update() {
	String fn;
	if(list.IsSelection()) {
		for(int i = 0; i < list.GetCount(); i++)
			if(list.IsSelected(i))
				Catq(fn, list[i].name);
	}
	else
	if(list.IsCursor()) {
		const FileList::File& m = list[list.GetCursor()];
		if(!m.isdir)
			Catq(fn, m.name);
	}
	if(mode == OPEN)
		file <<= fn;
	filename = String();
	filesize = String();
	filetime = String();
	if(preview)
		*preview <<= Null;
	if(list.IsCursor()) {
		fn = list[list.GetCursor()].name;
		if(fn[1] == ':' && fn.GetLength() <= 3)
			filename = t_("  Drive");
		else {
			String path = FilePath(fn);
			Array<FileSystemInfo::FileInfo> ff = filesystem->Find(path, 1);
			if(!ff.IsEmpty()) {
				filename = "  " + fn;
				if(ff[0].is_directory)
					filesize = t_("Directory  ");
				else {
					if(mode == SAVEAS)
						file <<= fn;
					filesize = FormatFileSize(ff[0].length);
					if(preview)
						*preview <<= path;
				}
				Time tm = ff[0].last_write_time;
				filetime = "     " + Format(tm);
			}
		}
	}
	else {
		int drives = 0;
		int dirs = 0;
		int files = 0;
		int64 length = 0;
		for(int i = 0; i < list.GetCount(); i++)
			if(!list.IsSelection() || list.IsSelected(i)) {
				const FileList::File& f = list[i];
				if(f.isdir)
#ifdef PLATFORM_WIN32
					(*f.name.Last() == ':' ? drives : dirs)++;
#else
					dirs++;
#endif
				else {
					files++;
					length += f.length;
				}
			}
		String s;
		if(drives)
			s << drives << t_(" drive(s)");
		else {
			if(dirs)
				s << dirs << t_(" folder(s)");
			if(files) {
				if(s.GetCount())
					s << ", ";
				s << files << t_(" file(s)");
			}
		}
		filename = "  " + s;
		if(length >= 0)
			filesize = FormatFileSize(length);
	}
	FileUpdate();
}

void FileSel::FileUpdate() {
	if(mode == SELECTDIR) {
		ok.Enable(!IsNull(~dir));
		return;
	}
	bool b = list.IsCursor() || !String(file).IsEmpty();
	ok.Enable(b);
	if(mode != SAVEAS || list.IsCursor() && list[list.GetCursor()].isdir)
		ok.SetLabel(t_("Open"));
	else
		ok.SetLabel(t_("Save"));
}

void FileSel::Rename(const String& on, const String& nn) {
#ifdef PLATFORM_WIN32
	if(FileMove(FilePath(on), FilePath(nn)))
#else
	if(rename(FilePath(on), FilePath(nn)) == 0)
#endif
	{
		Load();
		list.FindSetCursor(nn);
	}
	else
		Exclamation(t_("[A3* Renaming of file failed !&&]") + GetErrorMessage(GetLastError()));
}

void FileSel::Choice() {
	Load();
}

FileSel& FileSel::Type(const char *name, const char *ext) {
	type.Add(type.GetCount(), name);
	mask.Add(ext);
	if(IsNull(type))
		type.SetIndex(0);
	return *this;
}

FileSel& FileSel::Types(const char *d) {
	Vector<String> s = Split(d, '\n');
	for(int i = 0; i < s.GetCount(); i++) {
		Vector<String> h = Split(s[i], '\t');
		if(h.GetCount() == 2)
			Type(h[0], h[1]);
		if(h.GetCount() == 1)
			Type(h[0], h[0]);
	}
	return *this;
}

FileSel& FileSel::ClearTypes()
{
	type.Clear();
	mask.Clear();
	return *this;
}

FileSel& FileSel::ActiveType(int i)
{
	activetype.Clear();
	if(i >= 0 && i < type.GetCount())
		activetype = type.GetValue(i);
	return *this;
}

FileSel& FileSel::AllFilesType() {
	return Type(t_("All files"), "*.*");
}

struct FolderDisplay : public Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
	                   Color ink, Color paper, dword style) const;
};

Image GetDirIcon(const String& s)
{
	Image img;
#ifdef GUI_X11
	img = GetFileIcon(GetFileFolder(s), GetFileName(s), true, false, false);
#endif
#ifdef GUI_WIN32
	if((byte)*s.Last() == 255)
		img = CtrlImg::Network();
	else
		img = s.GetCount() ? GetFileIcon(s, false, true, false) : CtrlImg::Computer();
#endif
	if(IsNull(img))
		img = CtrlImg::Dir();
	return img;
}

void FolderDisplay::Paint(Draw& w, const Rect& r, const Value& q,
                          Color ink, Color paper, dword style) const
{
	String s = q;
	w.DrawRect(r, paper);
	Image img = GetDirIcon(s);
	w.DrawImage(r.left, r.top + (r.Height() - img.GetSize().cx) / 2, img);
	w.DrawText(r.left + 20,
	           r.top + (r.Height() - StdFont().Bold().Info().GetHeight()) / 2,
			   ~s, StdFont().Bold(), ink);
}

struct HomeDisplay : public Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q,
	                   Color ink, Color paper, dword style) const {
		w.DrawRect(r, paper);
		w.DrawImage(r.left, r.top + (r.Height() - CtrlImg::Home().GetSize().cx) / 2,
			        CtrlImg::Home());
		w.DrawText(r.left + 20,
		           r.top + (r.Height() - StdFont().Bold().Info().GetHeight()) / 2,
				   String(q), StdFont().Bold(), ink);
	}
};

void FileSel::Set(const String& s)
{
	fn.Clear();
	if(IsFullPath(s)) {
		ActiveDir(GetFileFolder(s));
		fn.Add(GetFileName(s));
	}
	else
		fn.Add(s);
	bidname = true;
}

void FileSel::GoToPlace()
{
	if(places.IsCursor()) {
		dir <<= places.GetKey();
		Load();
	}
}

bool FileSel::Execute(int _mode) {
	mode = _mode;
	if(mode == SELECTDIR) {
		if(!fn.IsEmpty())
			dir <<= NormalizePath(fn[0]);
		type.Hide();
		type_lbl.Hide();
		file.Hide();
		file_lbl.Hide();
		sortext.Hide();
		sort_lbl.Hide();
		ok.SetLabel(t_("&Select"));
		Logc p = filename.GetPos().y;
		int q = ok.GetPos().y.GetA() + ok.GetPos().y.GetB() + 8;
		p.SetA(q);
		filename.SetPosY(p);
		filesize.SetPosY(p);
		filetime.SetPosY(p);
		p = splitter.Ctrl::GetPos().y;
		p.SetB(q + 20);
		splitter.SetPosY(p);
		LogPos ps = search.GetPos();
		LogPos pl = sort_lbl.GetPos();
		pl.x.SetB(ps.x.GetB());
		pl.y.SetA(ok.GetPos().y.GetA());
		pl.y.SetB(ps.y.GetB());
		search.SetPos(pl);
		bidname = false;
	}
	else {
		for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
			if(q != &mkdir)
				q->Show();
		Rect r = GetRect();
		CtrlLayout(*this);
		ArrangeOKCancel(ok, cancel);
		SetRect(r);
	}
	readonly.Show(rdonly && mode == OPEN);
	list.Multi(multi && mode == OPEN);
	dir.ClearList();
	file <<= Null;
	int i;
	if(basedir.IsEmpty()) {
		dir.Add(GetHomeDirectory());
	#ifdef PLATFORM_POSIX
		Array<FileSystemInfo::FileInfo> root = filesystem->Find("/media/*");
		dir.Add(GetDesktopFolder());
		dir.Add("/");
		for(i = 0; i < root.GetCount(); i++) {
			String ugly = root[i].filename;
			if(ugly[0] != '.') {
				dir.Add("/media/" + root[i].filename);
			}
		}
	#else
		dir.Add(GetDesktopFolder());
		Array<FileSystemInfo::FileInfo> root = filesystem->Find(Null);
		for(i = 0; i < root.GetCount(); i++) {
			String ugly = root[i].filename;
			if(ugly != "A:\\" && ugly != "B:\\") {
				ugly.Cat('\0');
				ugly.Cat(root[i].root_style);
				dir.Add(root[i].filename, ugly);
			}
		}
		if(filesystem == &StdFileSystemInfo())
			dir.Add("\\", String(t_("Network")) + String(0, 1) + "\xff");
	#endif
		if(filesystem->IsPosix() && String(~dir).IsEmpty())
			dir <<= GetHomeDirectory();
		if(lru.GetCount())
			dir.AddSeparator();
		for(i = 0; i < lru.GetCount(); i++)
			if(IsFullPath(lru[i]) && filesystem->FolderExists(lru[i]))
				dir.Add(lru[i]);
		dir.SetDisplay(Single<FolderDisplay>(), max(16, Draw::GetStdFontCy()));
	}
	else {
		dir.SetDisplay(Single<HomeDisplay>(), max(16, Draw::GetStdFontCy()));
		if(filesystem->IsPosix()) {
			if(String(~dir)[0] == '/')
				dir <<= "";
		}
	}
	Rect lr = splitter.GetRect();
	Rect dr = dir.GetRect();
	int dp = max(20, dir.Ctrl::GetPos().y.GetB());
	int px = GetSize().cx - lr.right;
/*	if(IsMulti()) { // Cxl: Have we ever used these?!
		toggle.RightPos(px, dp).TopPos(dr.top, dp);
		minus.RightPos(px + 2 * dp, dp).TopPos(dr.top, dp);
		plus.RightPos(px + 3 * dp, dp).TopPos(dr.top, dp);
		px += 3 * dp;
		toggle.Show();
		minus.Show();
		plus.Show();
	}
	else {*/
		toggle.Hide();
		minus.Hide();
		plus.Hide();
//	}
	if(mkdir.IsShown()) {
		mkdir.RightPos(px, dp).TopPos(dr.top, dp);
		dirup.RightPos(px + dp, dp).TopPos(dr.top, dp);
		px += 2 * dp;
	}
	else {
		dirup.RightPos(px, dp).TopPos(dr.top, dp);
		px += dp;
	}
	dir.HSizePos(dr.left, px + 4);
	int q = type.FindValue(activetype);
	if(q >= 0)
		type <<= q;
	else
	if(type.GetCount())
		type.SetIndex(0);
	int dlc = type.GetCount();
	Load();
	ActiveFocus(file.IsEditable() ? (Ctrl&)file : (Ctrl&)list);
	if(bidname) {
		String s;
		for(int i = 0; i < fn.GetCount(); i++)
			Catq(s, fn[i]);
		file <<= s;
		ActiveFocus(file);
		bidname = false;
	}
	list.SetSbPos(lastsby);
	if(preselect.GetCount()) {
		for(int i = 0; i < list.GetCount(); i++)
		    if(list[i].name == preselect) {
				list.SetCursor(i);
				ActiveFocus(list);
				break;
			}
		preselect.Clear();
	}
	FileUpdate();
	Update();
	int c = TopWindow::Run(appmodal);
	TopWindow::Close();
	lastsby = list.GetSbPos();
	if(IsNumber(~type)) {
		int ti = ~type;
		type.Trim(dlc);
		if(ti >= 0 && ti < type.GetCount())
			activetype = type.GetValue(ti);
	}
	else
		type.Trim(dlc);
	if(c == IDOK) {
		String d = ~dir;
		if(filesystem->IsWin32())
			if(d.GetLength() == 3 && d[1] == ':') return true;
		if(filesystem->IsPosix())
			if(d == "/") return true;
		if(!IsFullPath(d)) return true;
		LruAdd(lru, d, 8);
		return true;
	}
	return false;
}

bool FileSel::ExecuteOpen(const char *title) {
	Title(title ? title : t_("Open"));
	return Execute(OPEN);
}

bool FileSel::ExecuteSaveAs(const char *title) {
	Title(title ? title : t_("Save as"));
	ok.SetLabel(t_("Save"));
	return Execute(SAVEAS);
}

bool FileSel::ExecuteSelectDir(const char *title)
{
	Title(title ? title : t_("Select directory"));
	return Execute(SELECTDIR);
}

void FileSel::Serialize(Stream& s) {
#ifdef PLATFORM_WIN32
	if(s.IsLoading()) {
		netnode.Clear();
		netstack.Clear();
	}
#endif
	int version = 10;
	s / version;
	String ad = ~dir;
	int dummy;
	if(version < 10)
		s / dummy;
	else
		s % activetype;
	s % ad;
	dir <<= ad;
	if(version < 1) {
		String n = fn.At(0);
		s % n;
		fn.At(0) = n;
	}
	else {
		if(version < 4)
			s % fn;
		else {
			Vector<String> __;
			s % __;
		}
	}
	if(version >= 2) {
		SerializePlacement(s);
		list.SerializeSettings(s);
	}
	if(version >= 3) {
		s % lastsby;
	}
	if(version >= 4) {
		s % lru;
	}
	if(version >= 5) {
		s % sortext;
	}
	if(version >= 6) {
		if(version >= 9)
			s % splitter;
		else {
			Splitter dummy;
			s % dummy;
		}
	}
	if(version >= 7) {
		s % hidden;
	}
	if(version >= 8) {
		s % hiddenfiles;
	}
}

String FileSel::GetFile(int i) const {
	String p;
	if(i >= 0 && i < fn.GetCount()) {
		p = fn[i];
		if(!IsFullPath(p))
			p = AppendFileName(dir.GetData(), p);
	}
	return p;
}

void FileSel::SyncSplitter()
{
	splitter.Clear();
	if(places.GetCount() && basedir.IsEmpty())
		splitter.Add(places);
	splitter.Add(list);
	if(preview)
		splitter.Add(*preview);
}

FileSel& FileSel::PreSelect(const String& path)
{
	ActiveDir(GetFileFolder(path));
	preselect = GetFileName(path);
	return *this;
}

void FileSel::InitSplitter()
{
	int n = splitter.GetCount();
	int i = 0;
	if(places.GetCount())
		splitter.SetPos(2000, i++);
	splitter.SetPos(10000 - 2000 * (n - 1), i);
}

FileSel& FileSel::Preview(Ctrl& ctrl)
{
	if(!preview) {
		Size sz = GetRect().GetSize();
		sz.cx = 5 * sz.cx / 3;
		SetRect(sz);
	}
	preview = &ctrl;
	SyncSplitter();
	InitSplitter();
	return *this;
}

FileSel& FileSel::Preview(const Display& d)
{
	preview_display.SetDisplay(d);
	return Preview(preview_display);
}

void FileSel::AddPlaceRaw(const String& path, const Image& m, const String& name)
{
	if(path.GetCount()) {
		places.Add(path, m, name);
		places.SetLineCy(places.GetCount() - 1, max(m.GetSize().cy + 4, GetStdFontCy() + 4));
		SyncSplitter();
		InitSplitter();
	}
}

FileSel& FileSel::AddPlace(const String& path, const Image& m, const String& name)
{
	AddPlaceRaw(NormalizePath(path), m, name);
	return *this;
}

FileSel& FileSel::AddPlace(const String& path, const String& name)
{
	return AddPlace(path, GetDirIcon(NormalizePath(path)), name);
}

FileSel& FileSel::AddPlace(const String& path)
{
	return AddPlace(path, GetFileTitle(path));
}

FileSel& FileSel::AddPlaceSeparator()
{
	places.AddSeparator();
	SyncSplitter();
	InitSplitter();
	return *this;
}

FileSel& FileSel::ClearPlaces()
{
	places.Clear();
	SyncSplitter();
	return *this;
}

FileSel& FileSel::AddStandardPlaces()
{
	AddPlace(GetHomeDirectory(), t_("Home"));
	AddPlace(GetDesktopFolder(), t_("Desktop"));
	AddPlace(GetMusicFolder(), t_("Music"));
	AddPlace(GetPicturesFolder(), t_("Pictures"));
	AddPlace(GetVideoFolder(), t_("Videos"));
	AddPlace(GetDocumentsFolder(), t_("Documents"));
	AddPlace(GetDownloadFolder(), t_("Downloads"));
	AddPlaceSeparator();
	Array<FileSystemInfo::FileInfo> root = filesystem->Find(Null);
	for(int i = 0; i < root.GetCount(); i++) {
		String desc = root[i].root_desc;
		String n = root[i].filename;
		if(n != "A:\\" && n != "B:\\") {
		#ifdef PLATFORM_WIN32
			if(*n.Last() == '\\')
				n.Trim(n.GetCount() - 1);
		#endif
			if(desc.GetCount())
				desc << " (" << n << ")";
			else
				desc = n;
			AddPlace(root[i].filename, desc);
		}
	}

#ifdef PLATFORM_WIN32
	if(GetSystemMetrics(SM_REMOTESESSION))
		for(int drive = 'A'; drive < 'Z'; drive++) {
			String path = Format("\\\\tsclient\\%c", drive);
			if(FindFile(path + "\\*.*"))
				AddPlace(path, Format(t_("%c on client"), drive));
		}
#endif
#ifdef PLATFORM_POSIX
	root = filesystem->Find("/media/*");
	for(int i = 0; i < root.GetCount(); i++) {
		String fn = root[i].filename;
		if(*fn != '.' && fn.Find("floppy") < 0)
			AddPlace("/media/" + fn, fn);
	}
#endif
#ifdef PLATFORM_WIN32
	AddPlaceSeparator();
	AddPlaceRaw("\\", CtrlImg::Network(), t_("Network"));
#endif
	return *this;
}

struct DisplayPlace : Display {
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper,
	                   dword style) const
	{
		w.DrawRect(r, paper);
		ValueArray va = q;
		Image m = va[0];
		String txt = va[1];
		Size isz = m.GetSize();
		w.DrawImage(r.left, r.top + (r.Height() - isz.cy) / 2, m);
		w.DrawText(r.left + isz.cx + 2, r.top + (r.Height() - GetStdFontCy()) / 2, txt,
		           StdFont(), ink);
	}
	virtual Size GetStdSize(const Value& q) const {
		ValueArray va = q;
		Image m = va[0];
		String txt = va[1];
		Size isz = m.GetSize();
		return Size(isz.cx + GetTextSize(txt, StdFont()).cx + 2, max(isz.cy, GetStdFontCy()));
	}
};

FileSel::FileSel() {
	filesystem = &StdFileSystemInfo();
	CtrlLayout(*this);
	ArrangeOKCancel(ok, cancel);
	Acceptor(ok, IDOK); ok.Ok();
	Rejector(cancel, IDCANCEL); cancel.Cancel();
	list.IconWidth(16).Renaming().Columns(3).ClickKill();
	list.WhenLeftDouble = THISBACK(OpenItem2);
	dirup <<= THISBACK(DirUp);
	Add(dirup);
	sortext <<= THISBACK(SearchLoad);
	Add(sortext);
	hidden <<= THISBACK(SearchLoad);
	Add(hidden);
	hiddenfiles <<= THISBACK(SearchLoad);
	Add(hiddenfiles);
	mkdir <<= THISBACK(MkDir);
	Add(mkdir);
	plus <<= THISBACK(Plus);
	Add(plus);
	minus <<= THISBACK(Minus);
	Add(minus);
	toggle <<= THISBACK(Toggle);
	Add(toggle);

	ok <<= THISBACK(Open);
	list <<= THISBACK(Update);
	file <<= THISBACK(FileUpdate);
	list.WhenRename = THISBACK(Rename);
	Sizeable();
	dirup.SetImage(CtrlImg::DirUp()).NoWantFocus();
	dirup.Tip(t_("Dir up") + String(" (Ctrl+Up)"));
	mkdir.SetImage(CtrlImg::MkDir()).NoWantFocus();
	mkdir.Tip(t_("Create directory") + String(" (F7)"));
	plus.SetImage(CtrlImg::Plus()).NoWantFocus();
	plus.Tip(t_("Select files"));
	minus.SetImage(CtrlImg::Minus()).NoWantFocus();
	minus.Tip(t_("Unselect files"));
	toggle.SetImage(CtrlImg::Toggle()).NoWantFocus();
	toggle.Tip(t_("Toggle files"));
	type <<= THISBACK(Load);
	sortext <<= 0;

	search.NullText("Search", StdFont().Italic(), SColorDisabled());
	search.SetFilter(CharFilterDefaultToUpperAscii);
	search <<= THISBACK(SearchLoad);

	filename.SetFont(StdFont());
	filename.SetFrame(ThinInsetFrame());
	filesize.SetFont(StdFont()).SetAlign(ALIGN_RIGHT);
	filesize.SetFrame(ThinInsetFrame());
	filetime.SetFont(StdFont());
	filetime.SetFrame(ThinInsetFrame());

	dir <<= THISBACK(Choice);
	dir.DisplayAll();
	dir.SetDropLines(24);

	readonly.Hide();

	lastsby = 0;

	asking = true;
	rdonly = false;
	multi = false;
	bidname = false;
	appmodal = true;

	AddChildBefore(GetFirstChild(), &sizegrip);

	preview = NULL;
	preview_display.SetFrame(FieldFrame());

	SyncSplitter();

	BackPaintHint();
	
	places.AddKey();
	places.AddColumn().AddIndex().SetDisplay(Single<DisplayPlace>());
	places.NoHeader().NoGrid();
	places.WhenLeftClick = THISBACK(GoToPlace);
	places.NoWantFocus();

#ifdef PLATFORM_WIN32
	list.IconWidth(GetFileIcon(GetHomeDirectory(), true, false, false).GetSize().cx);
#endif

	AddStandardPlaces();
	
	list.AutoHideSb();
	places.AutoHideSb();
}

FileSel::~FileSel() {}

END_UPP_NAMESPACE
