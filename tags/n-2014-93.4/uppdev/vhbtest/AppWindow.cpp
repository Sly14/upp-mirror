#include <CtrlLib/CtrlLib.h>
#pragma hdrstop

#ifndef UPP605
using namespace Upp;
#endif

class DlgVodniToky : public TopWindow {
public:
	typedef DlgVodniToky CLASSNAME;
	DlgVodniToky();

	virtual bool       Key(dword key, int repcnt) {
		if(key == K_ENTER)
			PromptOK("Subdialog2!");
		return false;
	}
};

DlgVodniToky::DlgVodniToky()
{
	Title("Vodn� toky");
	Sizeable()
		.Zoomable()
//		.MaximizeBox()
	;
//	Add(list.SizePos());
}

class AppWindow : public TopWindow {
public:
	typedef AppWindow CLASSNAME;
	AppWindow();

	void Paint(Draw& draw);
	bool Key(dword key, int);

private:
	RichText window_text;
};

bool AppWindow::Key(dword key, int)
{
	if(key == K_ENTER) {
		DlgVodniToky().Run();
		return true;
	}
	return false;
}

AppWindow::AppWindow()
{
	Title("Vodohospod��sk� bilance povrchov�ch vod");
	Sizeable().Zoomable();

	AddFrame(InsetFrame());

	String qtf;
	qtf << "[A3 [7*/ Demonstrace chyby s focusem]&&"
	"Pomoc� menu otev�i okno [* Vodn� toky].&"
	"Pak stiskni Enter.&"
	"Objev� se Prompt, ten "
	"odklepni a okno [* Vodn� toky] op�t zav�i (my�� nebo pomoc� Alt+F4).&"
	"Spr�vn� by se m�lo "
	"objevit op�t toto hlavn� okno aplikace, ale to m�sto toho zaleze �pln� dol� "
	"pod okna ostatn�ch aplikac�."
	;

	window_text = ParseQTF(qtf);
	window_text.ApplyZoom(GetRichTextStdScreenZoom());
}

void AppWindow::Paint(Draw& draw)
{
	Size client = GetSize();
	draw.DrawRect(client, Yellow());
	window_text.Paint(draw, 2, 2, client.cx - 4);
}

GUI_APP_MAIN
{
	SetLanguage(LNGC_('C', 'S', 'C', 'Z', CHARSET_WIN1250));
	AppWindow().Run();
}
