#ifdef PLATFORM_POSIX
inline bool MoveFile(const char *oldpath, const char *newpath)
{
	return !rename(oldpath, newpath);
}
#endif

struct FileTemplate {
	String filename;
	String condition;
	String text;
};

enum { TEMPLATEITEM_ID, TEMPLATEITEM_FILENAME, TEMPLATEITEM_OPTION, TEMPLATEITEM_SELECT };

struct TemplateItem {
	String         label;
	int            type;
	String         id;
	Vector<String> value;
	String         init;
};

struct PackageTemplate {
	String              name;
	bool                main, sub;
	Array<TemplateItem> item;
	Array<FileTemplate> file;
};

struct AppPreview : Ctrl {
	virtual void Paint(Draw& w);
	virtual void Layout();
	virtual void MouseWheel(Point p, int zdelta, dword keyflags);

	ScrollBar      sb;
	struct Line : Moveable<Line> {
		String text;
		bool   header;
	};
	Vector<Line> line;

	void Scroll() { Refresh(); }

	void Clear() { line.Clear(); Refresh(); sb.Set(0); }
	void Add(const String& data, bool header = false);

	typedef AppPreview CLASSNAME;

	AppPreview();
	virtual ~AppPreview();
};

struct TemplateDlg : public WithNewPackageLayout<TopWindow> {
	AppPreview      preview;

	Array<PackageTemplate> pt;

	Array<Label>  label;
	Array<Ctrl>   ctrl;
	DelayCallback delay;

	const PackageTemplate& ActualTemplate();

	String GetPackageFile();
	void Preview();
	void EnterTemplate();
	void LoadNest(const char *dir, bool main, bool recurse = true);
	void Load(const Vector<String>& path, bool main);
	void EnableCreate() { ok.Enable(!IsNull(package)); Inits(); delay.Invoke(); }
	void Inits();

	ArrayMap<String, EscValue> MakeVars0();
	ArrayMap<String, EscValue> MakeVars();
	void Create();

	void Serialize(Stream& s) { SerializePlacement(s); }

	typedef TemplateDlg CLASSNAME;
	TemplateDlg();
	virtual ~TemplateDlg();
};

String SelectPackage(const char *title, const char *startwith = NULL,
	bool selectvars = false, bool all = false);

int CondFilter(int c);
int FlagFilter(int c);
int FlagFilterM(int c);

struct UsesDlg : public WithUppOptPushDlg<TopWindow> {
	void New();

	typedef UsesDlg CLASSNAME;

	UsesDlg();
};

struct DependsDlg : public WithUppOptPushDlg<TopWindow> {
	String package;

	void New();

	typedef DependsDlg CLASSNAME;

	DependsDlg();
};

struct WorkspaceWork {
	static    Font ListFont();

	FileList    package;
	FileList    filelist;
	Vector<int> fileindex;

	String    main;
	String    actualpackage;
	int       actualfileindex;
	Package   actual;

	struct Sepfo : Moveable<Sepfo> {
		String package;
		String separator;

		bool operator==(const Sepfo& s) const { return package == s.package && separator == s.separator; }
		unsigned GetHashValue() const         { return CombineHash(::GetHashValue(package),
		                                                           ::GetHashValue(separator)); }
		void Serialize(Stream& s)             { s % package % separator; }
		Sepfo(const String& package, const String& separator) : package(package), separator(separator) {}
		Sepfo() {}
	};

	Index<Sepfo> closed;

	struct Backup {
		FileTime time;
		String   data;
	};

	ArrayMap<String, Backup> backup;

	bool         organizer;

	virtual void   PackageCursor();
	virtual void   FileCursor();
	virtual void   BuildFileMenu(Bar& bar)            {}
	virtual void   FilePropertiesMenu(Bar& bar)       {}
	virtual String GetOutputDir()                     { return Null; }
	virtual void   SyncWorkspace()                    {}
	virtual void   FileSelected()                     {}
	virtual void   FileRename(const String& nm)       {}

	void   ScanWorkspace();
	void   SavePackage();
	void   RestoreBackup();

	void SerializeFileSetup(Stream& s)                { s % filelist % package; }

	Sepfo  GetActiveSepfo();
	void   Group();
	void   OpenAllGroups();
	void   CloseAllGroups();
	void   GroupOrFile(Point pos);

	void   SetMain(const String& m)                           { main = m; }
	void   FindSetPackage(const String& s)                    { package.FindSetCursor(s); }

	void   ShowFile(int pi);

	String         GetActivePackage() const                   { return package.GetCurrentName(); }
	String         GetActiveFileName() const;
	bool           IsActiveFile() const;
	Package::File& ActiveFile();
	String         FileName(int i) const;
	bool           IsSeparator(int i) const;

	void LoadActualPackage();
	void SaveLoadPackage();
	void SaveLoadPackageNS();

	void MoveFile(int d);
	void Move(Vector<String>& v, FileList& ta, int d);

	enum ADDFILE { PACKAGE_FILE, OUTPUT_FILE, HOME_FILE, COMMON_FILE, LOCAL_FILE, ANY_FILE };
	void AddFile(ADDFILE type);
	void AddSeparator();
	void RemoveFile();
	void DelFile();
	void RenameFile();
	void ToggleFileSpeed();

	void AddNormalUses();
	void AddAnyUses();
	void TogglePackageSpeed();

	void RemovePackageMenu(Bar& bar);
	void RemovePackage(String from_package);

	void PackageMenu(Bar& bar);
	void FileMenu(Bar& bar);

	String PackagePathA(const String& pn);

	void SerializeClosed(Stream& s);

	typedef WorkspaceWork CLASSNAME;

	WorkspaceWork();
};

struct PackageEditor : WorkspaceWork, WithUppLayout<TopWindow> {
	virtual bool Key(dword key, int);
	virtual void FileCursor();
	virtual void PackageCursor();

	enum OptionType {
		FLAG = 0, USES, TARGET, LIBRARY, LINK, COMPILER,
		FILEOPTION, FILEDEPENDS
	};

	Vector<Array<OptItem> *> opt;
	Vector<String>           opt_name;

	void Add(const char *name, Array<OptItem>& m);

	void Serialize(Stream& s);
	void SaveOptions();
	void SaveOptionsLoad();

	void Empty();
	void FileEmpty();
	void OptionAdd(ArrayCtrl& option, int type, const char *title, const Array<OptItem>& o);
	void FindOpt(ArrayCtrl& option, int type, const String& when, const String& text);
	void FindOpt(ArrayCtrl& option, int type, int ii);
	void AdjustOptionCursor(ArrayCtrl& option);
	void Init(ArrayCtrl& option);
	void SetOpt(ArrayCtrl& opt, int type, OptItem& m, const String& when, const String& text);

	void OptionAdd(int type, const char *title, const Array<OptItem>& o);
	void AdjustPackageOptionCursor();
	void AddOption(int type);
	void OptionMenu(Bar& bar);
	void RemoveOption();
	void EditOption();
	void MoveOption(int d);

	void AdjustFileOptionCursor();
	void FindFileOpt(int type, const String& when, const String& text);
	void FileOptionMenu(Bar& bar);
	void MoveFileOption(int d);
	void AddDepends();
	void AddFileOption();
	void EditFileOption();
	void RemoveFileOption();

	typedef PackageEditor CLASSNAME;

	PackageEditor();
};

void EditPackages(const char *main, const char *startwith, String& cfg);
