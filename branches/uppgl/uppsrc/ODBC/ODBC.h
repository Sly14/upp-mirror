#ifndef _ODBC_ODBC_h
#define _ODBC_ODBC_h

#include <Sql/Sql.h>
#include <sql.h>
#include <sqlext.h>

NAMESPACE_UPP

bool   ODBCPerformScript(const String& text, StatementExecutor& executor, Gate2<int, int> progress_canceled = false);

class ODBCConnection;

class ODBCSession : public SqlSession {
public:
	virtual void           Begin();
	virtual void           Commit();
	virtual void           Rollback();

	virtual String         Savepoint();
	virtual void           RollbackTo(const String& savepoint);

	virtual bool           IsOpen() const;

	virtual Vector<String> EnumUsers();
	virtual Vector<String> EnumDatabases();
	virtual Vector<String> EnumTables(String database);
	virtual Vector<String> EnumViews(String database);
	virtual Vector<String> EnumSequences(String database);
	virtual Vector<String> EnumPrimaryKeys(String database, String table);
	virtual String         EnumRowID(String database, String table);

	virtual RunScript      GetRunScript() const                   { return &ODBCPerformScript; }

protected:
	virtual SqlConnection *CreateConnection();

private:
	friend class ODBCConnection;
	HENV                  henv;
	HDBC                  hdbc;
	HSTMT                 hstmt;
	String                user;
	int                   tlevel;
	ODBCConnection       *current;

	void   FlushConnections();
	bool   IsOk(SQLRETURN ret);

public:
	bool Connect(const char *cs);
	void Close();

	ODBCSession();
	~ODBCSession();
};

END_UPP_NAMESPACE

#endif
