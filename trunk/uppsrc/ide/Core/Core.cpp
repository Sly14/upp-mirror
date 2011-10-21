#include "Core.h"

typedef VectorMap<String, Builder *(*)()> BuilderMapType;
GLOBAL_VAR(BuilderMapType, BuilderMap)

void RegisterBuilder(const char *name, Builder *(*create)())
{
	ASSERT(BuilderMap().Find(name) < 0);
	BuilderMap().Add(name, create);
}

String FindInDirs(const Vector<String>& dir, const String& file)
{
	if(!IsFullPath(file))
		for(int i = 0; i < dir.GetCount(); i++) {
			String ef = CatAnyPath(dir[i], file);
			if(FileExists(ef))
				return ef;
		}
	return file;
}

String FindCommand(const Vector<String>& exedir, const String& cmdline)
{
	String app;
	const char *s = cmdline;
	while(*s && (byte)*s <= ' ')
		s++;
	if(*s == '\"')
	{
		const char *b = ++s;
		while(*s && *s != '\"')
			s++;
		app = String(b, s);
		if(*s)
			s++;
	}
	else
	{
		const char *b = s;
		while(*s && (byte)*s > ' ')
			s++;
		app = String(b, s);
	}
	String tail = s;
	String fn = FindInDirs(exedir, app);
	if(!FileExists(fn))
#ifdef PLATFORM_WIN32
		fn = FindInDirs(exedir, ForceExt(app, ".exe"));
#endif
#ifdef PLATFORM_POSIX
		fn = FindInDirs(exedir, app);
#endif
	if(fn.Find(' ') >= 0)
		fn = '\"' + fn + '\"';
	return fn + tail;
}

String GetMakePath(String fn, bool win32)
{
	fn = UnixPath(fn);
	if(!win32)
		return fn;
	String out;
	for(const char *p = fn; *p; p++)
		if(*p == '/')
			out << "\\\\";
		else
			out.Cat(*p);
	return out;
}

String AdjustMakePath(const char *fn)
{
	String out;
	for(; *fn; fn++)
		if(*fn == '$')
			out << '$' << '$';
		else
			out << *fn;
	return out;
}

static IdeContext *the_ide;

IdeContext *TheIde() { return the_ide; }
void        TheIde(IdeContext *context) { the_ide = context; }

void PutConsole(const char *s) { if(the_ide) the_ide->PutConsole(s); }
void PutVerbose(const char *s) { if(the_ide) the_ide->PutVerbose(s); }

const Workspace& GetIdeWorkspace()
{
	if(the_ide)
		return the_ide->IdeWorkspace();
	static Workspace x;
	return x;
}

String IdeContext::GetDefaultMethod()
{
	return LoadFile(ConfigFile("default_method"));
}

VectorMap<String, String> IdeContext::GetMethodVars(const String& method)
{
	VectorMap<String, String> map;
	LoadVarFile(ConfigFile((String)~method + ".bm"), map);
	return map;
}


String GetDefaultMethod()
{
	return the_ide ? the_ide->GetDefaultMethod() : String();
}

VectorMap<String, String> GetMethodVars(const String& method)
{
	return the_ide ? the_ide->GetMethodVars(method) : VectorMap<String, String>();
}

bool IdeIsBuilding()
{
	return the_ide && the_ide->IdeIsBuilding();
}

void IdeSetBar()
{
	if(the_ide)
		the_ide->IdeSetBar();
}

String IdeGetOneFile()
{
	return the_ide ? the_ide->IdeGetOneFile() : String(Null);
}

int IdeConsoleExecute(const char *cmdline, Stream *out, const char *envptr, bool quiet)
{
	return the_ide ? the_ide->IdeConsoleExecute(cmdline, out, envptr, quiet) : -1;
}

int IdeConsoleExecuteWithInput(const char *cmdline, Stream *out, const char *envptr, bool quiet)
{
	return the_ide ? the_ide->IdeConsoleExecuteWithInput(cmdline, out, envptr, quiet) : -1;
}

int IdeConsoleExecute(One<SlaveProcess> process, const char *cmdline, Stream *out, bool quiet)
{
	return the_ide ? the_ide->IdeConsoleExecute(process, cmdline, out, quiet) : -1;
}

int IdeConsoleAllocSlot()
{
	return the_ide ? the_ide->IdeConsoleAllocSlot() : 0;
}

bool IdeConsoleRun(const char *cmdline, Stream *out, const char *envptr, bool quiet, int slot, String key, int blitz_count)
{
	return the_ide && the_ide->IdeConsoleRun(cmdline, out, envptr, quiet, slot, key, blitz_count);
}

bool IdeConsoleRun(One<SlaveProcess> process, const char *cmdline, Stream *out, bool quiet, int slot, String key, int blitz_count)
{
	return the_ide && the_ide->IdeConsoleRun(process, cmdline, out, quiet, slot, key, blitz_count);
}

void IdeConsoleFlush()
{
	if(the_ide) the_ide->IdeConsoleFlush();
}

void IdeConsoleBeginGroup(String group)
{
	if(the_ide) the_ide->IdeConsoleBeginGroup(group);
}

void IdeConsoleEndGroup()
{
	if(the_ide) the_ide->IdeConsoleEndGroup();
}

bool IdeConsoleWait()
{
	return the_ide && the_ide->IdeConsoleWait();
}

void IdeGotoCodeRef(String s)
{
	if(the_ide) the_ide->IdeGotoCodeRef(s);
}

void IdeSetBottom(Ctrl& ctrl)
{
	if(the_ide) the_ide->IdeSetBottom(ctrl);
}

void  IdeActivateBottom() {
	if(the_ide) the_ide->IdeActivateBottom();
}


void IdeRemoveBottom(Ctrl& ctrl)
{
	if(the_ide) the_ide->IdeRemoveBottom(ctrl);
}

void IdeSetRight(Ctrl& ctrl)
{
	if(the_ide) the_ide->IdeSetRight(ctrl);
}

void IdeRemoveRight(Ctrl& ctrl)
{
	if(the_ide) the_ide->IdeRemoveRight(ctrl);
}

bool IdeIsDebug()
{
	return the_ide && the_ide->IdeIsDebug();
}

void IdeEndDebug()
{
	if(the_ide)
		the_ide->IdeEndDebug();
}

void IdeSetDebugPos(const String& file, int line, const Image& img, int i)
{
	if(the_ide)
		the_ide->IdeSetDebugPos(file, line, img, i);
}

void IdeHidePtr()
{
	if(the_ide)
		the_ide->IdeHidePtr();
}

bool IdeDebugLock()
{
	return the_ide && the_ide->IdeDebugLock();
}

bool IdeDebugUnLock()
{
	return the_ide && the_ide->IdeDebugUnLock();
}

bool IdeIsDebugLock()
{
	return the_ide && the_ide->IdeIsDebugLock();
}

String IdeGetFileName()
{
	return the_ide ? the_ide->IdeGetFileName() : String(Null);
}

int    IdeGetFileLine()
{
	return the_ide ? the_ide->IdeGetFileLine() : 0;
}

String IdeGetLine(int i)
{
	return the_ide ? the_ide->IdeGetLine(i) : String(Null);
}

bool SaveChangedFile(const char *path, String data, bool delete_empty)
{
	if(LoadFile(path) == data)
		return true;
	if(delete_empty && IsNull(data))
		return FileDelete(path);
	else
		return SaveFile(path, data);
}

static int sReadCharc(CParser& p)
{
	p.PassChar('\'');
	if(!IsAlpha(p.PeekChar()))
		p.ThrowError("language code alphabetic character expected");
	char c = p.GetChar();
	p.PassChar('\'');
	return c;
}

static void sReadLNG(CParser& p, char *c)
{
	p.PassChar('(');
	c[0] = sReadCharc(p);
	p.PassChar(',');
	c[1] = sReadCharc(p);
	p.PassChar(',');
	c[2] = sReadCharc(p);
	p.PassChar(',');
	c[3] = sReadCharc(p);
}

int ReadLNG(CParser& p) {
	char c[4];
	if(p.Id("LNG_CZECH"))
		return LNG_CZECH;
	else
	if(p.Id("LNG_ENGLISH"))
		return LNG_ENGLISH;
	else
	if(p.Id("LNG_")) {
		sReadLNG(p, c);
		p.PassChar(')');
		return LNG_(c[0], c[1], c[2], c[3]);
	}
	else
	if(p.Id("LNGC_")) {
		sReadLNG(p, c);
		p.PassChar(',');
		dword l = LNGC_(c[0], c[1], c[2], c[3], p.ReadInt());
		p.PassChar(')');
		return l;
	}
	else
		p.ThrowError("invalid language code");
	return 0;
}

String MakeLNG(int lang)
{
	int cs = GetLNGCharset(lang);
	String str;
	if(cs)
		return str << "LNGC_('"
		       << char(((lang >> 15) & 31) + 'A' - 1) << "', '"
		       << char(((lang >> 10) & 31) + 'A' - 1) << "', '"
		       << char(((lang >>  5) & 31) + 'A' - 1) << "', '"
		       << char((lang & 31) + 'A' - 1) << "', " << cs << ")";
	else
		return str << "LNG_('"
		       << char(((lang >> 15) & 31) + 'A' - 1) << "', '"
		       << char(((lang >> 10) & 31) + 'A' - 1) << "', '"
		       << char(((lang >>  5) & 31)+ 'A' - 1) << "', '"
		       << char((lang & 31) + 'A' - 1) << "')";
}

String PrintTime(int time) {
	int q = time % 1000 / 10;
	time /= 1000;
	return Format("(%d:%02d.%02d)", time / 60, time % 60, q);
}

Point ReadNums(CParser& p) {
	Point pt;
	pt.x = p.ReadInt();
	p.PassChar(',');
	pt.y = p.ReadInt();
	return pt;
}

Point ReadPoint(CParser& p)
{
	Point pt;
	p.PassChar('(');
	pt = ReadNums(p);
	p.PassChar(')');
	return pt;
}

bool OldLang() {
	static int q = -1;
	if(q < 0)
		q = FileExists(ConfigFile("oldlang"));
	return q;
}

int CharFilterCid(int c)
{
	return IsAlNum(c) || c == '_' ? c : 0;
}


bool IsDoc(String s)
{
	s = ToLower(s);
	if(s.Find("svn") >= 0)
		return false;
	return s.Find("readme") >= 0 || s.Find("copying") >= 0 || s.Find("license") >= 0 ||
	       s.Find("authors") >= 0;
}

void CopyFolder(const char *_dst, const char *_src, Index<String>& used, bool all)
{
	String dst = NativePath(_dst);
	String src = NativePath(_src);
	if(dst == src)
		return;
	FindFile ff(AppendFileName(src, "*"));
	bool realize = true;
	while(ff) {
		String s = AppendFileName(src, ff.GetName());
		String d = AppendFileName(dst, ff.GetName());
		if(ff.IsFolder())
			CopyFolder(d, s, used, all);
		else
		if(ff.IsFile() && (all || IsDoc(s) || used.Find(s) >= 0)) {
			if(realize) {
				RealizeDirectory(dst);
				realize = false;
			}
			SaveFile(d, LoadFile(s));
			SetFileTime(d, ff.GetLastWriteTime());
		}
		ff.Next();
	}
}
