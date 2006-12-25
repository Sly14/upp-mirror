#include "ide.h"
//#include "Install.h"

#ifdef PLATFORM_WIN32

String GetShellFolder(const char *name, HKEY type)
{
	return GetWinRegString(name, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", type);
}

void DelKey(const char *dir, const char *key)
{
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, dir, 0, KEY_READ, &hkey) != ERROR_SUCCESS)
		return;
	RegDeleteKey(hkey, key);
	RegCloseKey(hkey);
}

void RemoveWindowsItems()
{
	DeleteFile(AppendFileName(GetShellFolder("Common Programs", HKEY_LOCAL_MACHINE), "Ultimate++ IDE.lnk"));
	DeleteFile(AppendFileName(GetShellFolder("Desktop", HKEY_CURRENT_USER), "TheIde.lnk"));

	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Ultimate++", "DisplayName");
	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Ultimate++", "UninstallString");
	DelKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", "Ultimate++");
}

void Uninstall()
{
	String path = GetExeFilePath();
	String bat = AppendFileName(GetShellFolder("Desktop", HKEY_CURRENT_USER), "removepp.bat");
	String dir = GetFileFolder(path);
	if(!PromptYesNo("[*3 Do you wish to uninstall Ultimate`+`+ development system ?&&]"
	                "Uninstall will remove [* " + DeQtf(dir) + "] directory and all "
	                "registry and desktop items associated with Ultimate`+`+.")) return;
	DeleteFolderDeep(dir);
	RemoveWindowsItems();
	SaveFile(bat,
		":Repeat\r\n"
		"del \"" + path + "\"\r\n"
		"if exist \"" + path + "\" goto Repeat\r\n"
		"del \"" + GetExeDirFile("theide.log") + "\"\r\n"
		"del \"" + GetExeDirFile("dbghelp.dll") + "\"\r\n"
		"rmdir \"" + dir + "\"\r\n"
		"del \"" + bat + "\"\r\n"
	);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	char h[512];
	strcpy(h, bat);
	if(CreateProcess(NULL, h, NULL, NULL, FALSE,
	                  IDLE_PRIORITY_CLASS, NULL, GetShellFolder("Desktop", HKEY_CURRENT_USER),
	                  &si, &pi))
		Exclamation("Uninstall successful.");
	else
		Exclamation("Uninstall failed to remove some files...&" + DeQtf(GetLastErrorMessage()));
}

#define Ptr Ptr_
#define byte byte_

#include <winnls.h>
#include <winnetwk.h>

#include <wincon.h>
#include <shlobj.h>

#undef Ptr
#undef byte

bool CreateShellLink(const char *filepath, const char *linkpath, const char *desc, int icon)
{
	HRESULT hres;
	IShellLink* psl;
	IPersistFile* ppf;
	CoInitialize(NULL);
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink,
	                        (PVOID *) &psl);
	if(SUCCEEDED(hres)) {
		psl->SetPath(filepath);
		psl->SetDescription(desc);
		if(icon >= 0)
			psl->SetIconLocation(filepath, icon);
		hres = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf);
		if (SUCCEEDED(hres)) {
			WCHAR szPath[_MAX_PATH] = { 0 };
			MultiByteToWideChar(CP_ACP, 0, linkpath, strlen(linkpath), szPath, _MAX_PATH);
			hres = ppf->Save(szPath, TRUE);
			ppf->Release();
		}
	}
	psl->Release();
	CoUninitialize();
	return SUCCEEDED(hres);
}

bool InstallDesktopIcon(const char *exe, const char *lnk, const char *desc)
{
	return CreateShellLink(exe,
	                       AppendFileName(GetShellFolder("Desktop", HKEY_CURRENT_USER), lnk),
	                       desc, -1);
}

bool InstallProgramGroup(const char *exe, const char *groupname, const char *lnk, int icon = -1)
{
	String dir = GetShellFolder("Common Programs", HKEY_LOCAL_MACHINE);
	if(groupname) {
		dir = AppendFileName(dir, groupname);
		CreateDirectory(dir, NULL);
	}
	return CreateShellLink(exe, AppendFileName(dir, lnk), "", icon);
}

void InstallUninstall(const char *name, const char *dname, const char *cmdline)
{
	String path = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + String(name);
	SetWinRegString(dname, "DisplayName", path);
	SetWinRegString(cmdline, "UninstallString", path);
}

void InstallUninstall(const char *exe, const char *name)
{
	InstallUninstall(name, name, String(exe) + " -uninstall");
}

String Rdir(const char *s, const String& dir, const String& dir2)
{
	String r;
	while(*s) {
		if(*s == '#')
			r.Cat(dir);
		else
		if(*s == '@')
			r.Cat(dir2);
		else
			r.Cat(*s);
		s++;
	}
	return r;
}

bool CheckLicense()
{
	if(!FileExists((GetExeDirFile("license.chk"))))
		return true;
	ShowSplash();
	Ctrl::ProcessEvents();
	Sleep(2000);
	HideSplash();
	Ctrl::ProcessEvents();
	WithLicenseLayout<TopWindow> d;
	CtrlLayoutOKCancel(d, "License agreement");
	d.license = GetTopic("ide/app/BSD$en-us").text;
	d.license.Margins(4);
	d.license.SetZoom(Zoom(18, 100));
	d.ActiveFocus(d.license);
	if(d.Run() != IDOK) {
		Uninstall();
		return false;
	}
	DeleteFile(GetExeDirFile("license.chk"));
	return true;
}

bool Install()
{
	{
		WithInfoLayout<TopWindow> d;
		CtrlLayoutOKCancel(d, "Installation guide");
		d.info = GetTopic("ide/app/install$en-us").text;
		d.info.Margins(4);
		d.info.SetZoom(Zoom(18, 100));
		d.ActiveFocus(d.info);
		if(d.Run() != IDOK)
			return false;
		WithInstallLayout<TopWindow> dlg;
		CtrlLayoutOKCancel(dlg, "Ultimate++ user-code setup");
		dlg.myapps <<= GetExeFilePath().Mid(0, 3) + "MyApps";
		dlg.ActiveFocus(dlg.myapps);
		if(dlg.Run() != IDOK)
			return false;
		String ipp = GetExeDirFile("install.upp");
		String exe = GetExeFilePath();
		String dir = GetFileFolder(exe);
		String dir2 = ~dlg.myapps;
		RealizeDirectory(dir2);
		FileIn in(ipp);
		while(!in.IsEof()) {
			Vector<String> ln = Split(in.GetLine(), '|');
			if(ln.GetCount() != 4)
				break;
			SaveFile(AppendFileName(dir, ln[0]),
				"UPP = " + AsCString(Rdir(ln[1], dir, dir2)) + ";\r\n"
				"COMMON = " + AsCString(Rdir(ln[2], dir, dir2)) + ";\r\n"
				"OUTPUT = " + AsCString(Rdir(ln[3], dir, dir2)) + ";\r\n"
			);
		}
		in.Close();
		if(dlg.icon)
			InstallDesktopIcon(exe, "TheIde.lnk", "Ultimate++ IDE");
		InstallProgramGroup(exe, NULL, "Ultimate++ IDE.lnk");
		InstallUninstall(exe, "Ultimate++");
		DeleteFile(ipp);
	}
	AutoSetup();
	PromptOK("Ultimate`+`+ setup was finished.&Press OK to launch TheIDE.&"
	         "[* WARNING:] Do not put important files into the install directory as they "
	         "would be [* deleted] during [* uninstall] or [* upgrade] process!");;
	return true;
}

#else

bool CopyFolder(Progress& pi, const char *dst, const char *src)
{
	if(strcmp(src, dst) == 0)
		return true;
	RealizeDirectory(dst);
	pi.SetText(dst);
	FindFile ff(AppendFileName(src, "*"));
	while(ff) {
		if(pi.StepCanceled())
			return false;
		String s = AppendFileName(src, ff.GetName());
		String d = AppendFileName(dst, ff.GetName());
		if(ff.IsFolder())
			if(!CopyFolder(pi, d, s))
				return false;
		if(ff.IsFile())
			SaveFile(d, LoadFile(s));
		ff.Next();
	}
	return true;
}

void ChkSupp(const char *s, String& dir)
{
	if(IsNull(dir) && FileExists(AppendFileName(s, "GCC.bm")))
		dir = s;
}

struct XInstallDlg : public WithXInstallLayout<TopWindow> {

private:

	FrameRight<Button> pathbrowse;

	void	FindInstFolder();

public:

	typedef XInstallDlg CLASSNAME;

	XInstallDlg();

};

void XInstallDlg::FindInstFolder() {
	FileSel *fs = &OutputFs();
	fs->Set(path);
	if(! fs->ExecuteSelectDir("Select output directory ..."))
		return;
	path <<= ~(*fs);
}

XInstallDlg::XInstallDlg() {
	CtrlLayoutOKCancel(*this, "Ultimate++ user setup");
	pathbrowse <<= THISBACK(FindInstFolder);
	pathbrowse.SetMonoImage(CtrlImg::smallright()).NoWantFocus();
	uppsrc = true;
	reference = true;
	examples = true;
	tutorial = true;
	path.AddFrame(pathbrowse);
	path <<= AppendFileName(FromSystemCharset(getenv("HOME")), "upp");
}

bool Install()
{
	XInstallDlg dlg;
	String supp;
	ChkSupp(GetHomeDirFile("upp"), supp);
	ChkSupp("/usr/local/share/upp", supp);
	ChkSupp("/usr/X11R6/share/upp", supp);
	ChkSupp("/usr/local/lib/upp", supp);
	ChkSupp("/usr/local/bin/upp", supp);
	ChkSupp("/usr/share/upp", supp);
	ChkSupp("/usr/lib/upp", supp);
	ChkSupp("/usr/bin/upp", supp);
	if(IsNull(supp)) {
		Exclamation("'upp' directory not found.");
		return true;
	}
	Progress pi;
	for (;;) {
		if (dlg.Run() == IDOK) {
			if (DirectoryExists((String)dlg.path)) {
				int dlgres = PromptYesNoCancel("Directory exists, overwrite ?");
				if (dlgres == -1 ) return false;
				if (dlgres) break;
			} else break;
		} else return false;
	}
	String upp(dlg.path);
	String uppsrc;
	String pp;
	String out = AppendFileName(upp, "out");
	String Common = AppendFileName(upp, "Common");
	pp << "\nOUTPUT = " << AsCString(out)
	   << "\nCOMMON = " << AsCString(AppendFileName(upp, "Common"));
	DirectoryCreate(out);
	if(!CopyFolder(pi, Common, AppendFileName(supp, "Common")))
		return false;
	if(dlg.uppsrc) {
		if(!CopyFolder(pi, AppendFileName(upp, "uppsrc"), AppendFileName(supp, "uppsrc")))
			return false;
		uppsrc = AppendFileName(upp, "uppsrc");
	}
	else
		uppsrc = AppendFileName(supp, "uppsrc");
	SaveFile(ConfigFile("uppsrc.var"), "UPP = " + AsCString(uppsrc) + pp);
	String u;
	if(dlg.reference) {
		if(!CopyFolder(pi, AppendFileName(upp, "reference"), AppendFileName(supp, "reference")))
			return false;
		u = AppendFileName(upp, "reference");
	}
	else
		u = AppendFileName(supp, "reference");
	SaveFile(ConfigFile("reference.var"), "UPP = " + AsCString(u + ';' + uppsrc) + pp);
	if(dlg.examples) {
		if(!CopyFolder(pi, AppendFileName(upp, "examples"), AppendFileName(supp, "examples")))
			return false;
		u = AppendFileName(upp, "examples");
	}
	else
		u = AppendFileName(supp, "examples");
	SaveFile(ConfigFile("examples.var"), "UPP = " + AsCString(u + ';' + uppsrc) + pp);
	if(dlg.tutorial) {
		if(!CopyFolder(pi, AppendFileName(upp, "tutorial"), AppendFileName(supp, "tutorial")))
			return false;
		u = AppendFileName(upp, "tutorial");
	}
	else
		u = AppendFileName(supp, "tutorial");
	SaveFile(ConfigFile("tutorial.var"), "UPP = " + AsCString(u + ';' + uppsrc) + pp);
	SaveFile(ConfigFile("MyApps.var"), "UPP = " + AsCString(AppendFileName(upp, "MyApps;" + uppsrc)) + pp);
	String bm = ConfigFile("GCC.bm");
	if(IsNull(LoadFile(bm)))
		SaveFile(bm, LoadFile(AppendFileName(supp, "GCC.bm")));
	PromptOK("Ultimate`+`+ user setup was finished.&Press OK to launch TheIDE.");
	return true;
}

void Uninstall() {}

#endif
