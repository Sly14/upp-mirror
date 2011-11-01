#include <Core/Core.h>
#include <plugin/sqlite3/Sqlite3.h>

using namespace Upp;

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <sqlite3sch/schema.sch>
#include "Sql/sch_header.h"

#include <Sql/sch_schema.h>
#include <Sql/sch_source.h>

CONSOLE_APP_MAIN
{
	Sqlite3Session sqlite3;
	if(!sqlite3.Open(ConfigFile("simple.db"))) {
		Cout() << "Can't create or open database file\n";
		return;
	}
	
#ifdef _DEBUG
	sqlite3.SetTrace();
#endif


	SQL = sqlite3;
	
	SQL.Execute("drop table SUPPLIER");
	
	SqlSchema sch(SQLITE3);
	All_Tables(sch);
	SqlPerformScript(sch.Upgrade());
	SqlPerformScript(sch.Attributes());
	SQL.ClearError();
}
