#include <Core/Core.h>
#include <plugin/bz2/bz2.h>

using namespace Upp;

void Log(const char *txt)
{
	Cout() << txt << "\r\n";
}

void Error(const char *e)
{
	Log(e);
	abort();
}

String Syx(const char *s)
{
	Log(s);
	String r;
	int q = Sys(s, r);
	if(q)
		Error(String().Cat() << "Failed: " << s << "\r\n" <<
		      "Exit code: " << q << "\r\n" <<
		      "Output: " << r);
	return r;
}

void CopyFolder(const char *src, const char *dst, bool deep = true)
{
	if(GetFileName(src) == ".svn")
		return;
	Cout() << Sprintf("Directory %s\n", src);
	RealizeDirectory(dst);
	FindFile ff(String(src) + "/*.*");
	while(ff) {
		String s = AppendFileName(src, ff.GetName());
		String d = AppendFileName(dst, ff.GetName());
		if(ff.IsFile()) {
			String q = LoadFile(s);
			String ext = ToLower(GetFileExt(s));
			if(ext != ".aux" && ext != ".$old")
				SaveFile(d, q);
		}
		else
		if(ff.IsFolder() && *ff.GetName() != '.') {
			if(deep || *GetFileExt(s))
				CopyFolder(s, d, deep);
		}
		ff.Next();
	}
}

int CrLfSm(int c)
{
	return c == ';' || c == '\r' || c == '\n';
}

void CopyFolders(const char *src, const char *dst, const char *folders, bool deep = true)
{
	Vector<String> folder = Split(LoadFile(folders), CrLfSm);
	for(int i = 0; i < folder.GetCount(); i++)
		CopyFolder(AppendFileName(src, folder[i]), AppendFileName(dst, folder[i]), deep);
}

String tmp = "u:/upp.tmp";
String upptmp = tmp + "/u";
String ass = upptmp + "/uppsrc";
String upp = "u:/upp.src";
String uppsrc = upp + "/uppsrc";

void CopyIdeFile(const String& fn)
{
	SaveFile(upptmp + '/' + fn, LoadFile("c:/theide/" + fn));
}

int NoDigit(int c) { return IsDigit(c) ? 0 : c; }
int FilterVersion(int c) { return c == ':' ? '_' : c; }

void Make(String pkg, String exe, String method = "MSC9")
{
	Syx("c:/theide/umk " + ass + " " + pkg + " c:/theide/" + method + ".bm -arv " + upptmp + "/" + exe);
	FileDelete(upptmp + "/" + ForceExt(exe, ".map"));
}

CONSOLE_APP_MAIN
{
	Vector<String> s = Split(Syx("svnversion " + upp), NoDigit);
	if(s.GetCount() == 0)
		Error("Invalid version");
	String version = s.Top();
	Log("version: " + version);
	
	DeleteFolderDeep(tmp);
	RealizeDirectory(tmp);
	
	RealizeDirectory("u:/upload");
	
	CopyFolders(uppsrc, ass, uppsrc + "/packages");
	CopyFolders(uppsrc, ass, uppsrc + "/packages1", false);
	CopyFolders(upp, upptmp, uppsrc + "/assemblies");
	SaveFile(upptmp + "/uppsrc/guiplatform.h", "");
	SaveFile(upptmp + "/uppsrc/uppconfig.h", LoadFile(uppsrc + "/uppconfig.h"));
	SaveFile(upptmp + "/uppsrc/ide/version.h", "#define IDE_VERSION \"" + version + "\"\r\n");
	Make("ide", "theide32.exe");
	Make("ide", "theide64.exe", "MSC9X64");
	Make("umk", "umk.exe");

	CopyIdeFile("c:/theide/dbghelp.dll");
	CopyIdeFile("c:/theide/en-us.scd");
	CopyIdeFile("c:/theide/en-gb.scd");

	SetCurrentDirectory(upptmp);
	
	SaveFile("install.upp", LoadFile(uppsrc + "/install.upp"));
	SaveFile("license.chk", "1");

	Syx("7z a " + tmp + "/upp.7z * -r -mx -m0fb=255 -mf=off");
	SetCurrentDirectory(tmp);
	Syx("c:/theide/umk " + upp + "/uppbox," + upp +
	    "/uppsrc WinInstaller2 c:/theide/MSC9.bm -ar u:/upload/upp-win32-" +
	    Filter(version, FilterVersion) + ".exe");
}
