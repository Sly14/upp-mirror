#include "LayDes.h"

struct VisGenDlg : public WithVisGenLayout<TopWindow> {
	LayoutData&  layout;
	Vector<int>  sel;

	void Refresh();
	bool HasItem(const char *id);
	void Type();
	String GetName();

	typedef VisGenDlg CLASSNAME;

	VisGenDlg(LayoutData& layout, const Vector<int>& cursor);
};

bool VisGenDlg::HasItem(const char *id)
{
	for(int i = 0; i < layout.item.GetCount(); i++)
		if(layout.item[i].variable == id)
			return true;
	return false;
}

String VisGenDlg::GetName()
{
	String n = layout.name;
	int l = n.GetCount() - 6;
	if(l > 0 && n.Mid(l) == "Layout")
		n = n.Mid(0, l);
	return n + "Dlg";
}

void VisGenDlg::Refresh()
{
	String s;
	int q = ~type;
	pars.Enable(q >= 2);
	toupper1.Enable(q >= 2);
	name1.Enable(q >= 2);
	quotes1.Enable(q >= 2);
	toupper2.Enable(q >= 3);
	name2.Enable(q >= 3);
	quotes2.Enable(q >= 3);
	String oce = "\tCtrlLayout";
	bool ok = false;
	if(HasItem("ok")) {
		ok = true;
		oce << "OK";
	}
	if(HasItem("cancel"))
		oce << "Cancel";
	if(HasItem("exit"))
		oce << "Exit";
	String n = ~name;
	if(IsNull(n))
		n = GetName();
	if(q == 0) {
		s << "class " << n << " : public With" << layout.name << "<TopWindow> {\n"
		  << "\ttypedef " << n << " CLASSNAME;\n"
		  << "\n"
		  << "public:\n"
		  << "\t" << n << "();\n"
		  << "};\n"
		  << "\n"
		  << n << "::" << n << "()\n"
		  << "{\n"
		  << oce;
		s << "(*this, \"\");\n";
		s << "}\n";
	}
	else
	if(q == 1) {
		s << "struct " << n << " : With" << layout.name << "<TopWindow> {\n"
		  << "\ttypedef " << n << " CLASSNAME;\n"
		  << "\t" << n << "();\n"
		  << "};\n"
		  << "\n"
		  << n << "::" << n << "()\n"
		  << "{\n"
		  << oce;
		s << "(*this, \"\");\n";
		s << "}\n";
	}
	else
	if(q == 2) {
		String n = ~name;
		if(IsNull(n))
			n = "dlg";
		s << "\tWith" << layout.name << "<TopWindow> " << n << ";\n"
		  << oce << '(' << n << ", \"\");\n";
		if(ok)
			s << "\tif(" << n << ".Execute() != IDOK)\n\t\treturn;\n";
		else
			s << '\t' << n << ".Execute();\n";
	}
	else
		for(int i = 0; i < sel.GetCount(); i++) {
			String id = layout.item[sel[i]].variable;
			if(!IsNull(id)) {
				if(pars)
					s << '(';
				String ss;
				if(name1 && !IsNull(~name))
					ss << ~name << '.';
				if(toupper1)
					ss << ToUpper(id);
				else
					ss << id;
				if(quotes1)
					ss = AsCString(ss);
				s << ss;
				if(q == 4) {
					s << ", ";
					String ss;
					if(name2 && !IsNull(~name))
						ss << ~name << '.';
					if(toupper2)
						ss << ToUpper(id);
					else
						ss << id;
					if(quotes2)
						ss = AsCString(ss);
					s << ss;
				}
				if(pars)
					s << ')';
				s << '\n';
			}
		}
	view <<= s;
}

void VisGenDlg::Type()
{
	String n = GetName();
	switch((int)~type) {
	case 0:
	case 1:
		name <<= GetName();
		break;
	case 2:
		name <<= "dlg";
		break;
	default:
		name <<= "";
	}
	Refresh();
}

VisGenDlg::VisGenDlg(LayoutData& layout, const Vector<int>& cursor)
:	layout(layout)
{
	type <<= 0;
	CtrlLayoutOKCancel(*this, "Code generator");
	type <<= THISBACK(Type);
	name <<=
	pars <<=
	toupper1 <<=
	quotes1 <<=
	name1 <<=
	toupper2 <<=
	quotes2 <<=
	name2 <<= THISBACK(Refresh);
	Refresh();
	view.Highlight(CodeEditor::HIGHLIGHT_CPP);
	view.HideBar();
	view.SetFont(Courier(12));
	if(cursor.GetCount())
		sel <<= cursor;
	else
		for(int i = 0; i < layout.item.GetCount(); i++)
			sel.Add(i);
}

void LayDes::VisGen()
{
	if(currentlayout < 0)
		return;
	VisGenDlg dlg(CurrentLayout(), cursor);
	if(dlg.Run() == IDOK)
		WriteClipboardText(~dlg.view);
}
