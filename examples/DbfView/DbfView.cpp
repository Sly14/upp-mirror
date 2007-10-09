#include <CtrlLib/CtrlLib.h>
#include <plugin/dbf/dbf.h>

using namespace Upp;

struct DbfView : public TopWindow {
	Splitter s;
	ArrayCtrl table;
	ArrayCtrl row;
	DbfStream dbf;

	void EnterRow();
	void Perform();

	typedef DbfView CLASSNAME;

	DbfView();
};

String FormatField(const DbfStream::Field& f)
{
	return f.name + Format(" (%c%d)", f.type, f.width);
}

void DbfView::EnterRow()
{
	row.Clear();
	if(!dbf.Fetch(table.GetKey()))
		return;
	for(int i = 0; i < dbf.GetFieldCount(); i++)
		row.Add(FormatField(dbf.GetField(i)), dbf[i]);
}

void DbfView::Perform()
{
	FileSel fs;
	LoadFromFile(fs);
	fs.AllFilesType();
	fs.Type("dbf", "*.dbf");
	if(!fs.ExecuteOpen("DBF..")) return;
	StoreToFile(fs);
	if(!dbf.Open(~fs))
		Exclamation("Can't open input file");
	table.AddKey();
	for(int i = 0; i < min(4, dbf.GetFieldCount()); i++)
		table.AddColumn(FormatField(dbf.GetField(i)));
	while(dbf.Fetch()) {
		Vector<Value> v;
		v.Add(dbf.GetPos());
		for(int i = 0; i < min(4, dbf.GetFieldCount()); i++)
			v.Add(dbf[i]);
		table.Add(v);
	}
	Run();
}

DbfView::DbfView()
{
	s.Set(table, row);
	s.SetPos(7000);
	Add(s.SizePos());
	Sizeable().Zoomable();
	table.WhenEnterRow = THISBACK(EnterRow);
	row.AddColumn("Column");
	row.AddColumn("Value", 2);
}

GUI_APP_MAIN
{
	SetDefaultCharset(CHARSET_WIN1252);
	Ctrl::NoLayoutZoom();

	DbfView().Perform();
}
