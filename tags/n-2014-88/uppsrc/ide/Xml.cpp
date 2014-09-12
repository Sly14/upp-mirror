#include "ide.h"

struct XmlView : public TopWindow {
	TreeCtrl              xml;
	LineEdit              view;
	FrameTop<StaticRect>  errorbg;
	Label                 error;
	ParentCtrl            data;

	virtual bool Key(dword key, int count);
	virtual void Close();

	void Load(int parent, XmlParser& p);
	void Load(const String& txt);
	void CopyPath();

	typedef XmlView CLASSNAME;

	void Serialize(Stream& s);

	XmlView();
};

bool XmlView::Key(dword key, int count)
{
	if(key == K_ESCAPE) {
		Close();
		return true;
	}
	return false;
}

void XmlView::Load(int parent, XmlParser& p)
{
	if(p.IsTag()) {
		String tag = p.ReadTag();
		String txt = tag;
		for(int i = 0; i < p.GetAttrCount(); i++)
			txt << ' ' << p.GetAttr(i) << "=\"" << p[i] << "\"";
		parent = xml.Add(parent, IdeImg::XmlTag(), tag, txt);
		while(!p.End()) {
			if(p.IsEof())
				throw XmlError("Unexpected end of text.");
			Load(parent, p);
		}
	}
	else
	if(p.IsText())
		xml.Add(parent, IdeImg::XmlText(), Null, NormalizeSpaces(p.ReadText()));
	else
	if(p.IsPI())
		xml.Add(parent, IdeImg::XmlPI(), Null, NormalizeSpaces(p.ReadPI()));
	else
	if(p.IsDecl())
		xml.Add(parent, IdeImg::XmlDecl(), Null, NormalizeSpaces(p.ReadDecl()));
	else
	if(p.IsComment())
		xml.Add(parent, IdeImg::XmlComment(), Null, NormalizeSpaces(p.ReadComment()));
	else
		throw XmlError("Unexpected input.");
}

void XmlView::Load(const String& txt)
{
	XmlParser p(txt);
	xml.Clear();
	try {
		while(!p.IsEof())
			Load(0, p);
	}
	catch(XmlError e) {
		error = "XML parsing error: " + e;
		view.Show();
		view <<= txt;
		view.SetCursor(view.GetPos(p.GetLine() - 1, p.GetColumn() - 1));
		view.SetFocus();
		return;
	}
	xml.Show();
	xml.SetFocus();
}

void XmlView::Serialize(Stream& s)
{
	int version = 0;
	s / version;
	SerializePlacement(s);
}

void XmlView::CopyPath()
{
	String path;
	int id = xml.GetCursor();
	while(id >= 0) {
		String tag = xml.Get(id);
		if(tag.GetCount())
			path = "[" + AsCString(tag) + "]" + path;
		id = xml.GetParent(id);
	}
	WriteClipboardText(path);
}

void XmlView::Close()
{
	StoreToGlobal(*this, "XML view");
	TopWindow::Close();
}

XmlView::XmlView()
{
	Title("XML view");

	xml.NoRoot();

	error.SetFont(Arial(20)).SetInk(Red);
	errorbg.Height(25).Add(error.SizePos());
	view.AddFrame(errorbg);
	view.SetReadOnly();
	view.SetColor(LineEdit::PAPER_READONLY, SColorPaper());

	xml.Hide();
	view.Hide();

	Add(xml.SizePos());
	Add(view.SizePos());

	Sizeable().Zoomable();

	Icon(IdeCommonImg::xml());
	
	xml.WhenLeftDouble = THISBACK(CopyPath);
}

void Ide::Xml()
{
	static XmlView dlg;
	dlg.Load(editor.IsSelection() ? editor.GetSelection() : editor.Get());
	if(!dlg.IsOpen()) {
		LoadFromGlobal(dlg, "XMLview");
		dlg.OpenMain();
	}
}
