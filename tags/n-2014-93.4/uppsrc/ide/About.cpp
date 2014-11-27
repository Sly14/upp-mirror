#include "ide.h"

#ifdef PLATFORM_WIN32
#include "shellapi.h"
#endif

#define TOPICFILE <ide/app.tpp/all.i>
#include <Core/topic_group.h>

Size MakeLogo(Ctrl& parent, Array<Ctrl>& ctrl)
{
	Image logo = IdeImg::logo();
	Size  isz = logo.GetSize();
	ImageCtrl& l = ctrl.Create<ImageCtrl>();
	Label& v = ctrl.Create<Label>();
	Label& v1 = ctrl.Create<Label>();
	l.SetImage(logo);
	Size sz = Size(isz.cx, isz.cy + 80);
	String h;
	h = IDE_VERSION;
	if(sizeof(void *) == 8)
		h << " (64 bit)";
#ifdef GUI_GTK
	h << " (Gtk)";
#endif
	v = h;
	v.RightPos(10, Ctrl::MINSIZE).TopPos(70, 40);
	l.Add(v);
	v.SetFont(Arial(20));
	v.SetInk(Blend(Gray, Blue));
	v1 = Format("%d`KB", MemoryUsedKb());
	v1.LeftPos(300, 100).BottomPos(20, 12);
	v1.SetFont(Arial(10));
	l.Add(v1);
	parent.Add(ctrl.Create<StaticRect>().Color(White).SizePos());
	parent.Add(l.TopPos(0, isz.cy).LeftPos(0, isz.cx));
	parent.Add(ctrl.Create<StaticRect>().Color(Blue).LeftPos(2, isz.cx - 4).TopPos(isz.cy, 1));
	RichTextView& w = ctrl.Create<RichTextView>();
	w.SetQTF(GetTopic("ide/app/Sponsor$en-us"));
	w.PageWidth(2900);
	w.NoSb();
	w.SetFrame(NullFrame());
	w.NoLazy();
	parent.Add(w.TopPos(isz.cy + 3, 99).LeftPos(0, isz.cx));
	return sz;
}

struct Splash : Ctrl {
	Array<Ctrl> ctrl;

	Splash() {
		SetRect(GetWorkArea().CenterRect(MakeLogo(*this, ctrl) + 2));
		SetFrame(BlackFrame());
	}
};

void HideSplash()
{
	if(Single<Splash>().IsOpen())
		Single<Splash>().Close();
}

void ShowSplash()
{
	Single<Splash>().PopUp(NULL, false, false);
	SetTimeCallback(750, callback(HideSplash));
}

bool IsSplashOpen()
{
	return Single<Splash>().IsOpen();
}

struct AboutDlg : TopWindow {
	Array<Ctrl>  ctrl;
	RichTextView about;

	typedef AboutDlg CLASSNAME;

	virtual bool Key(dword key, int) {
		if(key == K_ALT_M)
			MemoryProfileInfo();
		return false;
	}

	AboutDlg() {
		Size isz = MakeLogo(*this, ctrl);
		SetRect(0, 0, 1000, isz.cy);
		about.SetQTF(GetTopic("ide/app/About$en-us"), Zoom(130, 1024));
		about.SetZoom(Zoom(1, 1));
		about.RightPos(0, 1000 - isz.cx).VSizePos();
		about.HMargins(4);
		about.SetFrame(NullFrame());
		about.NoLazy();
		Background(PaintRect(ColorDisplay(), SColorPaper()));
		Add(about);
		Title("About TheIDE");
	}
};

void Ide::About()
{
	AboutDlg().Execute();
}
