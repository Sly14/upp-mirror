#include "SqlCtrl.h"
#include "SqlDlg.h"

NAMESPACE_UPP

int SqlError(
#ifdef PLATFORM_WIN32
	HWND parent,
#endif
	const char *text, const char *error, const char *statement, bool retry) {
	WithSqlErrorLayout<TopWindow> r;
	CtrlLayoutCancel(r, "Chyba p�i pr�ci s datab�z�");
	r.text = text;
	r.error.AutoHideSb().SetReadOnly() <<= error;
	r.statement.AutoHideSb().SetReadOnly() <<= statement;
	r.retry.Show(retry);
	r.Acceptor(r.retry, IDRETRY);
	r.ActiveFocus(r);
#ifdef PLATFORM_WIN32
	if(parent) r.Open(parent);
#endif
	return r.Run();
}

int SqlError(
#ifdef PLATFORM_WIN32
	HWND parent,
#endif
	const char *text, const SqlSession& session, bool retry) {
	return SqlError(
#ifdef PLATFORM_WIN32
		parent,
#endif
		text, session.GetLastError(), session.GetErrorStatement(), retry);
}

int SqlError(
#ifdef PLATFORM_WIN32
	HWND parent,
#endif
	const char *text, const Sql& sql, bool retry) {
	return SqlError(
#ifdef PLATFORM_WIN32
		parent,
#endif
		text, sql.GetSession(), retry);
}

#ifdef PLATFORM_WIN32
int  SqlError(const char *text, const char *error, const char *statement, bool retry) {
	return SqlError(HWND(NULL), text, error, statement, retry);
}

int  SqlError(const char *text, const SqlSession& session, bool retry) {
	return SqlError(HWND(NULL), text, session, retry);
}

int  SqlError(const char *text, const Sql& sql, bool retry) {
	return SqlError(HWND(NULL), text, sql, retry);
}
#endif

bool   DisplayError(const SqlSession& session, const char *msg) {
	if(!session.WasError()) return false;
	SqlError(msg ? msg : "P�i prov�d�n� operace do�lo k chyb�",
			 session.GetLastError(), session.GetErrorStatement());
	return true;
}

bool   ShowError(SqlSession& session, const char *msg) {
	if(DisplayError(session, msg)) {
		session.ClearError();
		return true;
	}
	return false;
}

bool ErrorRollback(SqlSession& session, const char *msg) {
	if(!ShowError(session, msg ? msg : "P�i prov�d�n� operace do�lo k chyb�.\nOperace je stornov�na."))
		return false;
	session.Rollback();
	return true;
}

bool OkCommit(SqlSession& session, const char *msg) {
	if(ErrorRollback(session, msg))
		return false;
	session.Commit();
	if(ShowError(session, msg ? msg : t_("SQL error!")))
		return false;
	return true;
}

bool DisplayError(const Sql& sql, const char *msg) {
	return DisplayError(sql.GetSession(), msg);
}

bool ShowError(Sql& sql, const char *msg) {
	return ShowError(sql.GetSession(), msg);
}

bool ErrorRollback(Sql& sql, const char *emsg) {
	return ErrorRollback(sql.GetSession(), emsg);
}

bool OkCommit(Sql& sql, const char *emsg) {
	return OkCommit(sql.GetSession(), emsg);
}

#ifndef NOAPPSQL
bool   DisplayError(const char *msg)    { return DisplayError(SQL, msg); }
bool   ShowError(const char *msg)       { return ShowError(SQL, msg); }
bool   ErrorRollback(const char *emsg)  { return ErrorRollback(SQL, emsg); }
bool   OkCommit(const char *emsg)       { return OkCommit(SQL, emsg); }
#endif

END_UPP_NAMESPACE
