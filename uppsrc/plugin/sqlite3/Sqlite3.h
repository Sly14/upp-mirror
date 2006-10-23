#ifndef __Plugin_Sqlite3__
#define __Plugin_Sqlite3__

#include <Sql/Sql.h>

// Not yet implemented.  What is this supposed to do??
bool Sqlite3PerformScript(const String& text,
#ifdef NOAPPSQL
	StatementExecutor& se,
#else
	StatementExecutor& se = SQLStatementExecutor(),
#endif
	Gate2<int, int> progress_canceled = false
);

class Sqlite3Session : public SqlSession {
public:
	virtual bool           IsOpen() const               { return NULL != db; }
	virtual RunScript      GetRunScript() const         { return &Sqlite3PerformScript; }
	virtual Vector<String> EnumUsers()                  { NEVER(); return Vector<String>(); } // No users in sqlite!
	virtual Vector<String> EnumDatabases();				// Currently does not detect attached DBs, just returns current filename
	virtual Vector<String> EnumTables(String database);	// Currently ignores database string
//	virtual Vector<SqlColumnInfo> EnumColumns(String database, String table);	// Currently ignores database string

	// Some opaque structures used by the sqlite3 library
	typedef struct sqlite3 sqlite3;
	typedef struct sqlite3_stmt sqlite3_stmt;

protected:
	virtual SqlConnection *CreateConnection();

private:
	sqlite3 *db;
	String current_filename;
	String current_dbname;

public:
	bool Open(const char *filename);
	void Close();

	operator sqlite3 *()   { return db; }

	virtual void   Begin();
	virtual void   Commit();
	virtual void   Rollback();

	Sqlite3Session()       { db = NULL; Dialect(SQLITE3); }
	~Sqlite3Session()      { Close(); }
};


#endif
