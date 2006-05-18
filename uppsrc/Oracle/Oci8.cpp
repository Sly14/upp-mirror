#include "Oracle8.h"
#include "OciCommon.h"
#pragma hdrstop

#define LLOG(x) // RLOG(x)

//#define DLLFILENAME "ora803.dll"
#define DLLFILENAME "oci.dll"
#define DLIMODULE   OCI8
#define DLIHEADER   <Oracle/Oci8.dli>
#include <Core/dli.h>

static String OciError(OCIError *errhp)
{
	if(!OCI8()) return t_("Error running OCI8 Oracle connection dynamic library.");
	if(!errhp) return t_("Unknown error.");
	OraText errbuf[512];
	strcpy((char *)errbuf, t_("(unknown error)"));
	sb4 errcode;
	OCI8().OCIErrorGet(errhp, 1, NULL, &errcode, errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
	return (const char *) errbuf;
}

bool Oracle8::AllocOciHandle(void *hp, int type) {
	LLOG("AllocOciHandle(" << type << "), envhp = " << envhp);
	*(dvoid **)hp = NULL;
	return OCI8() && !OCI8().OCIHandleAlloc(envhp, (dvoid **)hp, type, 0, NULL);
}

void Oracle8::FreeOciHandle(void *hp, int type) {
	LLOG("FreeOciHandle(" << type << ")");
	if(OCI8() && hp) OCI8().OCIHandleFree(hp, type);
}

class OCI8Connection : public Link<OCI8Connection>, public OciSqlConnection {
protected:
	virtual void        SetParam(int i, const Value& r);
	virtual void        SetParam(int i, OracleRef r);
	virtual bool        Execute();
	virtual int         GetRowsProcessed() const;
	virtual bool        Fetch();
	virtual void        GetColumn(int i, Ref f) const;
	virtual void        Cancel();
	virtual SqlSession& GetSession() const;
	virtual String      GetUser() const;
	virtual String      ToString() const;
	virtual            ~OCI8Connection();

	struct Item {
		int            type;
		int16          len;
		sb2            ind;
		ub2            rl;
		ub2            rc;
		Vector<Value>  dynamic;
		bool           is_dynamic;
		bool           dyna_full;
		int            dyna_vtype;
		ub4            dyna_width;
		ub4            dyna_len;
		OCILobLocator *lob;
		OCIBind       *bind;
		OCIDefine     *define;
		OCI8Connection *refcursor;
		union {
			byte          *ptr;
			byte           buffer[8];
		};

		byte *Data()                 { return len > sizeof(buffer) ? ptr : buffer; }
		const byte *Data() const     { return len > sizeof(buffer) ? ptr : buffer; }
		bool  IsNull() const         { return ind < 0; }
		bool  Alloc(OCIEnv *envhp, int type, int len, int res = 0);
		void  Clear();
		void  DynaFlush();

		static sb4 DynaIn(dvoid *ictxp, OCIBind *bindp,
			ub4 iter, ub4 index, dvoid **bufpp, ub4 *alenp, ub1 *piecep, dvoid **indp) {
			Item *self = reinterpret_cast<Item *>(ictxp);
			ASSERT(self -> bind == bindp);
			*bufpp = NULL;
			*alenp = 0;
			*indp = NULL;
			*piecep = OCI_ONE_PIECE;
			return OCI_CONTINUE;
		}

		static sb4 DynaOut(dvoid *octxp, OCIBind *bindp,
			ub4 iter, ub4 index, dvoid **bufpp, ub4 **alenp, ub1 *piecep, dvoid **indp, ub2 **rcodep) {
			Item *self = reinterpret_cast<Item *>(octxp);
			ASSERT(self -> bind == bindp);
			return self -> Out(iter, index, bufpp, alenp, piecep, indp, rcodep);
		}

		sb4 Out(ub4 iter, ub4 index, dvoid **bufpp, ub4 **alenp, ub1 *piecep, dvoid **indp, ub2 **rcodep);

		Item();
		~Item();
	};

	Oracle8           *session;
	OCIStmt           *stmthp;
	OCIError          *errhp;
	Array<Item>        param;
	Array<Item>        column;
	String             parsed_cmd;
	Vector<int>        dynamic_param;
	int                dynamic_pos;
	int                dynamic_rows;
	int                fetched;
	int                fetchtime;
	bool               refcursor;

	OCISvcCtx *SvcCtx() const        { ASSERT(session); return session->svchp; }

	Item&     PrepareParam(int i, int type, int len, int reserve, int dynamic_vtype);
	void      SetParam(int i, const String& s);
	void      SetParam(int i, int integer);
	void      SetParam(int i, double d);
	void      SetParam(int i, Date d);
	void      SetParam(int i, Time d);
	void      SetParam(int i, Sql& refcursor);

	void      AddColumn(int type, int len);
	void      GetColumn(int i, String& s) const;
	void      GetColumn(int i, int& n) const;
	void      GetColumn(int i, double& d) const;
	void      GetColumn(int i, Date& d) const;
	void      GetColumn(int i, Time& t) const;

	void      SetError();
	bool      GetColumnInfo();

	void      Clear();

    OCI8Connection(Oracle8 *s);

	friend class Oracle8;
};

void OCI8Connection::Item::Clear() {
	if(type == SQLT_BLOB || type == SQLT_CLOB)
		OCI8().OCIDescriptorFree((dvoid *)lob, OCI_DTYPE_LOB);
	else
	if(len > sizeof(buffer))
		delete[] ptr;
	len = 0;
	lob = NULL;
	dynamic.Clear();
	dyna_full = false;
	dyna_vtype = VOID_V;
	dyna_width = 0;
}

OCI8Connection::Item::Item() {
	len = 0;
	lob = NULL;
	bind = NULL;
	define = NULL;
	refcursor = 0;
	dyna_full = false;
	is_dynamic = false;
}

OCI8Connection::Item::~Item() {
	Clear();
}

bool OCI8Connection::Item::Alloc(OCIEnv *envhp, int _type, int _len, int res) {
	if(_type == type && len >= _len) return false;
	Clear();
	type = _type;
	len = _len + res;
	if(type == SQLT_BLOB || type == SQLT_CLOB)
		OCI8().OCIDescriptorAlloc(envhp, (dvoid **) &lob, OCI_DTYPE_LOB, 0, NULL);
	if(len > sizeof(buffer))
		ptr = new byte[len];
	return true;
}

void OCI8Connection::Item::DynaFlush() {
	if(dyna_full) {
		dyna_full = false;
		Value v;
		if(ind == 0) {
			if(dyna_len > dyna_width)
				dyna_width = dyna_len;
			const byte *p = Data();
			switch(type) {
			case SQLT_INT:
				v = *(const int *)p;
				break;

			case SQLT_FLT:
				v = *(const double *)p;
				break;

			case SQLT_STR:
				v = String((const char *)p, dyna_len);
				break;

			case SQLT_DAT:
				v = OciDecodeTime(p);
				break;

			case SQLT_CLOB:
			case SQLT_BLOB:
			default:
				NEVER();
				break;
			}
		}
		dynamic.Add(v);
	}
}

sb4 OCI8Connection::Item::Out(ub4 iter, ub4 index, dvoid **bufpp, ub4 **alenp, ub1 *piecep, dvoid **indp, ub2 **rcodep) {
	DynaFlush();
	*bufpp = Data();
	*alenp = &dyna_len;
	dyna_len = len;
	*piecep = OCI_ONE_PIECE;
	ind = 0;
	*indp = &ind;
	rc = 0;
	*rcodep = &rc;
	dyna_full = true;
	return OCI_CONTINUE;
}

OCI8Connection::Item& OCI8Connection::PrepareParam(int i, int type, int len, int res, int dynamic_vtype) {
	Item& p = param.At(i);
	p.dyna_vtype = dynamic_vtype;
	p.is_dynamic = (dynamic_vtype != VOID_V);
	if(p.Alloc(session -> envhp, type, len, res))
		parse = true;
	return p;
}

void OCI8Connection::SetParam(int i, const String& s) {
	int l = s.GetLength();
	Item& p = PrepareParam(i, SQLT_STR, l + 1, 100, VOID_V);
	memcpy(p.Data(), s, l + 1);
	p.ind = l ? 0 : -1;
}

void OCI8Connection::SetParam(int i, int integer) {
	Item& p = PrepareParam(i, SQLT_INT, sizeof(int), 0, VOID_V);
	*(int *) p.Data() = integer;
	p.ind = IsNull(integer) ? -1 : 0;
}

void OCI8Connection::SetParam(int i, double d) {
	Item& p = PrepareParam(i, SQLT_FLT, sizeof(double), 0, VOID_V);
	*(double *) p.Data() = d;
	p.ind = IsNull(d) ? -1 : 0;
}

void OCI8Connection::SetParam(int i, Date d) {
	Item& w = PrepareParam(i, SQLT_DAT, 7, 0, VOID_V);
	w.ind = (OciEncodeDate(w.Data(), d) ? 0 : -1);
}

void OCI8Connection::SetParam(int i, Time t) {
	Item& w = PrepareParam(i, SQLT_DAT, 7, 0, VOID_V);
	w.ind = (OciEncodeTime(w.Data(), t) ? 0 : -1);
}

void OCI8Connection::SetParam(int i, OracleRef r) {
	PrepareParam(i, r.GetOraType(), r.GetMaxLen(), 0, r.GetType());
}

class Oracle8RefCursorStub : public SqlSource {
public:
	Oracle8RefCursorStub(SqlConnection *cn) : cn(cn) {}
	virtual SqlConnection *CreateConnection() { return cn; }

	virtual int            GetDialect() const { return SQLD_ORACLE; }

private:
	SqlConnection *cn;
};

void OCI8Connection::SetParam(int i, Sql& rc) {
	Item& w = PrepareParam(i, SQLT_RSET, -1, 0, VOID_V);
	w.refcursor = new OCI8Connection(session);
	w.refcursor -> refcursor = true;
	*(OCIStmt **)w.Data() = w.refcursor -> stmthp;
	w.ind = 0;
	Oracle8RefCursorStub stub(w.refcursor);
	rc = stub;
}

void  OCI8Connection::SetParam(int i, const Value& q) {
	if(q.IsNull())
		SetParam(i, String());
	else
		switch(q.GetType()) {
		case STRING_V:
		case WSTRING_V:
			SetParam(i, String(q));
			break;
		case INT_V:
			SetParam(i, int(q));
			break;
		case DOUBLE_V:
			SetParam(i, double(q));
			break;
		case DATE_V:
			SetParam(i, Date(q));
			break;
		case TIME_V:
			SetParam(i, (Time)q);
			break;
		case UNKNOWN_V:
			if(IsTypeRaw<Sql *>(q)) {
				SetParam(i, *ValueTo<Sql *>(q));
				break;
			}
			if(OracleRef::IsValue(q)) {
				SetParam(i, OracleRef(q));
				break;
			}
		default:
			NEVER();
		}
}

void OCI8Connection::AddColumn(int type, int len) {
	column.Add().Alloc(session -> envhp, type, len);
}

bool OCI8Connection::Execute() {
	ASSERT(session);
	int time = msecs();
	int args = 0;
	if(parse) {
//		Cancel();
		if((args = OciParse(statement, parsed_cmd, this, session)) < 0)
			return false;
		ub4 fr = fetchrows;

		OCI8().OCIAttrSet(stmthp, OCI_HTYPE_STMT, &fr, 0, OCI_ATTR_PREFETCH_ROWS, errhp);
		if(OCI8().OCIStmtPrepare(stmthp, errhp, (byte *)~parsed_cmd, parsed_cmd.GetLength(), OCI_NTV_SYNTAX,
			              OCI_DEFAULT)) {
			SetError();
			return false;
		}

		while(param.GetCount() < args)
			SetParam(param.GetCount(), String());
		param.SetCount(args);
		dynamic_param.Clear();
		for(int i = 0; i < args; i++) {
			Item& p = param[i];
			if(OCI8().OCIBindByPos(stmthp, &p.bind, errhp, i + 1, p.Data(), p.len, p.type,
				            &p.ind, NULL, NULL, 0, NULL, p.is_dynamic ? OCI_DATA_AT_EXEC : OCI_DEFAULT)) {
				SetError();
				return false;
			}
			if(p.is_dynamic) {
				dynamic_param.Add(i);
				if(OCI8().OCIBindDynamic(p.bind, errhp, &p, &Item::DynaIn, &p, &Item::DynaOut)) {
					SetError();
					return false;
				}
			}
		}
	}
	ub2 type;
	if(OCI8().OCIAttrGet(stmthp, OCI_HTYPE_STMT, &type, NULL, OCI_ATTR_STMT_TYPE, errhp)) {
		SetError();
		return false;
	}

	if(OCI8().OCIStmtExecute(SvcCtx(), stmthp, errhp, type != OCI_STMT_SELECT, 0, NULL, NULL,
		              session->StdMode() && session->level == 0 ? OCI_COMMIT_ON_SUCCESS : OCI_DEFAULT)) {
		SetError();
		session->PostError();
		return false;
	}

	if(!dynamic_param.IsEmpty()) {
		dynamic_pos = -1;
		for(int i = 0; i < dynamic_param.GetCount(); i++)
			param[dynamic_param[i]].DynaFlush();
		dynamic_rows = param[dynamic_param[0]].dynamic.GetCount();
	}

	if(parse) {
		if(!GetColumnInfo())
			return false;
		for(int i = 0; i < param.GetCount(); i++)
			if(param[i].refcursor && !param[i].refcursor -> GetColumnInfo())
				return false;
	}
	fetched = 0;
	fetchtime = 0;
	if(Stream *s = session->GetTrace()) {
		*s << ToString() << '\n';
		if(session->IsTraceTime())
			*s << Format("----- exec %d ms\n", msecs(time));
	}
	return true;
}

bool OCI8Connection::GetColumnInfo() {
	info.Clear();
	column.Clear();
	ub4 argcount;
	if(OCI8().OCIAttrGet(stmthp, OCI_HTYPE_STMT, &argcount, 0, OCI_ATTR_PARAM_COUNT, errhp) != OCI_SUCCESS) {
		SetError();
		return false;
	}
	if(!dynamic_param.IsEmpty()) {
		for(int i = 0; i < dynamic_param.GetCount(); i++) {
			const Item& p = param[dynamic_param[i]];
			SqlColumnInfo& ci = info.Add();
			ci.name = Format("#%d", i + 1);
			ci.width = p.dyna_width;
			ci.prec = Null;
			ci.scale = Null;
		}
		parse = false;
		return true;
	}
	for(ub4 i = 1; i <= argcount; i++) {
		OCIParam *pd;
		if(OCI8().OCIParamGet(stmthp, OCI_HTYPE_STMT, errhp, (dvoid **)&pd, i) != OCI_SUCCESS) {
			SetError();
			return false;
		}
		char *name;
		ub4 name_len;
		ub2 type, width;
		ub2 prec;
		sb1 scale;
		OCI8().OCIAttrGet(pd, OCI_DTYPE_PARAM, &type, NULL,  OCI_ATTR_DATA_TYPE, errhp);
		OCI8().OCIAttrGet(pd, OCI_DTYPE_PARAM, &width, NULL, OCI_ATTR_DATA_SIZE, errhp);
		OCI8().OCIAttrGet(pd, OCI_DTYPE_PARAM, &name, &name_len, OCI_ATTR_NAME, errhp);
		OCI8().OCIAttrGet(pd, OCI_DTYPE_PARAM, &prec, NULL,  OCI_ATTR_PRECISION, errhp);
		OCI8().OCIAttrGet(pd, OCI_DTYPE_PARAM, &scale, NULL,  OCI_ATTR_SCALE, errhp);
		SqlColumnInfo& ii = info.Add();
		ii.decimals = Null;
		ii.width = width;
		ii.prec = prec;
		ii.scale = scale;
		ii.name = ToUpper(TrimRight(String(name, name_len)));
		bool blob = false;
		switch(type) {
		case SQLT_NUM:
			ii.type = DOUBLE_V;
			AddColumn(SQLT_FLT, sizeof(double));
			break;
		case SQLT_DAT:
			ii.type = TIME_V;
			AddColumn(SQLT_DAT, 7);
			break;
		case SQLT_BLOB:
			ii.type = ORA_BLOB_V;
			AddColumn(SQLT_BLOB, sizeof(OCILobLocator *));
			blob = true;
			break;
		case SQLT_CLOB:
			ii.type = ORA_CLOB_V;
			AddColumn(SQLT_CLOB, sizeof(OCILobLocator *));
			blob = true;
			break;
		case SQLT_RDD:
			ii.type = STRING_V;
			AddColumn(SQLT_STR, 64);
			break;
		default:
			ii.type = STRING_V;
			AddColumn(SQLT_STR, ii.width ? ii.width + 1 : longsize);
			break;
		}
		Item& c = column.Top();
		OCI8().OCIDefineByPos(stmthp, &c.define, errhp, i,
			blob ? (void *)&c.lob : (void *)c.Data(), blob ? -1 : c.len,
			c.type, &c.ind, NULL, NULL, OCI_DEFAULT);
	}
	parse = false;
	return true;
}

int OCI8Connection::GetRowsProcessed() const {
	if(!dynamic_param.IsEmpty())
		return dynamic_pos + 1;
	ub4 rp = 0;
	OCI8().OCIAttrGet(stmthp, OCI_HTYPE_STMT, &rp, NULL, OCI_ATTR_ROW_COUNT, errhp);
	return rp;
}

bool OCI8Connection::Fetch() {
	ASSERT(!parse);
	if(parse) return false;
	if(!dynamic_param.IsEmpty()) // dynamic pseudo-fetch
		return (dynamic_pos < dynamic_rows && ++dynamic_pos < dynamic_rows);
	bool tt = session->IsTraceTime();
	int fstart = tt ? msecs() : 0;
	sword status = OCI8().OCIStmtFetch(stmthp, errhp, 1, OCI_FETCH_NEXT, OCI_DEFAULT);
	bool ok = false;
	if(status == OCI_SUCCESS || status == OCI_SUCCESS_WITH_INFO) {
		fetchtime += msecs(fstart);
		++fetched;
		ok = true;
	}
	else if(status != OCI_NO_DATA)
		SetError();
	if(Stream *s = session->GetTrace()) {
		if(!ok || fetched % 100 == 0 && fetchtime)
			*s << NFormat("----- fetch(%d) in %d ms, %8n ms/rec, %2n rec/s\n",
				fetched, fetchtime,
				fetchtime / max<double>(fetched, 1),
				fetched * 1000.0 / max<double>(fetchtime, 1));
	}
	return ok;
}

void OCI8Connection::GetColumn(int i, String& s) const {
	if(!dynamic_param.IsEmpty()) {
		s = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Item& c = column[i];
	if(c.type == SQLT_BLOB || c.type == SQLT_CLOB) {
		int handle;
		GetColumn(i, handle);
		if(!IsNull(handle))
		{
			OracleBlob blob(*session, handle);
			s = LoadStream(blob);
		}
		else
			s = Null;
		return;
	}
	ASSERT(c.type == SQLT_STR);
	if(c.ind < 0)
		s = Null;
	else
		s = (char *) c.Data();
}

void OCI8Connection::GetColumn(int i, double& n) const {
	if(!dynamic_param.IsEmpty()) {
		n = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Item& c = column[i];
	ASSERT(c.type == SQLT_FLT || c.type == SQLT_BLOB || c.type == SQLT_CLOB);
	if(c.ind < 0)
		n = DOUBLE_NULL;
	else
		n = c.type == SQLT_BLOB || c.type == SQLT_CLOB ? (int)c.lob : *(double *) c.Data();
}

void OCI8Connection::GetColumn(int i, int& n) const {
	double d;
	GetColumn(i, d);
	n = IsNull(d) ? (int) Null : (int) d;
}

void OCI8Connection::GetColumn(int i, Date& d) const {
	if(!dynamic_param.IsEmpty()) {
		d = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Item& c = column[i];
	const byte *data = c.Data();
	ASSERT(c.type == SQLT_DAT);
	if(c.ind < 0)
		d = Null; // d.year = d.month = d.day = 0;
	else
		d = OciDecodeDate(data);
}

void OCI8Connection::GetColumn(int i, Time& t) const {
	if(!dynamic_param.IsEmpty()) {
		t = param[dynamic_param[i]].dynamic[dynamic_pos];
		return;
	}
	const Item& c = column[i];
	const byte *data = c.Data();
	ASSERT(c.type == SQLT_DAT);
	if(c.ind < 0)
		t = Null; // t.year = t.month = t.day = 0;
	else
		t = OciDecodeTime(data);
}

void  OCI8Connection::GetColumn(int i, Ref f) const {
	if(!dynamic_param.IsEmpty()) {
		f.SetValue(param[dynamic_param[i]].dynamic[dynamic_pos]);
		return;
	}
	switch(f.GetType()) {
	case STRING_V:
		GetColumn(i, RefString(f));
		break;
	case INT_V:
		GetColumn(i, RefInt(f));
		break;
	case DOUBLE_V:
		GetColumn(i, RefDouble(f));
		break;
	case DATE_V:
		GetColumn(i, RefDate(f));
		break;
	case TIME_V:
		GetColumn(i, RefTime(f));
		break;
	case VALUE_V:
		{
			double d;
			Time m;
			String s;
			switch(column[i].type) {
			case SQLT_STR:
//			case SQLT_RDD:
				GetColumn(i, s);
				f = Value(s);
				break;
			case SQLT_BLOB:
			case SQLT_CLOB:
			case SQLT_FLT:
				GetColumn(i, d);
				f = Value(d);
				break;
			case SQLT_DAT:
				GetColumn(i, m);
				f = Value(m);
				break;
			default:
				NEVER();
			}
		}
		break;
	default:
		NEVER();
	}
}

void OCI8Connection::Cancel() {
	OCI8().OCIBreak(SvcCtx(), errhp);
	parse = true;
}

void OCI8Connection::SetError() {
	if(session)
		session->SetError(OciError(errhp), statement);
	parse = true;
}

SqlSession& OCI8Connection::GetSession() const {
	ASSERT(session);
	return *session;
}

String OCI8Connection::GetUser() const {
	ASSERT(session);
	return session->user;
}

String OCI8Connection::ToString() const {
	String lg;
	bool quotes = false;
	int argn = 0;
	for(const char *q = statement; *q; q++) {
		if(*q== '\'' && q[1] != '\'')
			quotes = !quotes;
		if(!quotes && *q == '?') {
			if(argn < param.GetCount()) {
				const Item& m = param[argn++];
				if(m.IsNull())
					lg << "Null";
				else
				switch(m.type) {
				case SQLT_STR:
					lg.Cat('\'');
					lg += (const char *) m.Data();
					lg.Cat('\'');
					break;
				case SQLT_FLT:
					lg << *(const double *) m.Data();
					break;
				case SQLT_DAT:
					lg << OciDecodeTime(m.Data());
//					const byte *data = m.Data();
//					lg << (int)data[3] << '.' << (int)data[2] << '.' <<
//						  int(data[0] - 100) * 100 + data[1] - 100;
					break;
				}
			}
			else
				lg += t_("<not supplied>");
		}
		else
			lg += *q;
	}
	return lg;
}

OCI8Connection::OCI8Connection(Oracle8 *s) {
	refcursor = false;
	session = s;
	if(!session -> AllocOciHandle(&stmthp, OCI_HTYPE_STMT)
	|| !session -> AllocOciHandle(&errhp, OCI_HTYPE_ERROR))
		session -> SetError(t_("Error initializing connection"), t_("OCI8 connection"));
	LinkAfter(&s->clink);
}

void OCI8Connection::Clear() {
	if(session) {
		if(refcursor) {
			OCIStmt *aux = 0;
			if(!session -> AllocOciHandle(&aux, OCI_HTYPE_STMT))
				session -> SetError(OciError(errhp), t_("Closing reference cursor"));
			static char close[] = "begin close :1; end;";
			bool err = false;
			OCIBind *bind = 0;
			err = OCI8().OCIStmtPrepare(aux, errhp, (OraText *)close, strlen(close), OCI_NTV_SYNTAX, OCI_DEFAULT)
				|| OCI8().OCIBindByPos(aux, &bind, errhp, 1, &stmthp, 0, SQLT_RSET, 0, 0, 0, 0, 0, OCI_DEFAULT)
				|| OCI8().OCIStmtExecute(SvcCtx(), aux, errhp, 1, 0, 0, 0, OCI_DEFAULT);
			if(err)
				SetError();
			session -> FreeOciHandle(aux, OCI_HTYPE_STMT);
		}
		session -> FreeOciHandle(stmthp, OCI_HTYPE_STMT);
		session -> FreeOciHandle(errhp, OCI_HTYPE_ERROR);
		session = NULL;
	}
}

OCI8Connection::~OCI8Connection() {
	Clear();
}

SqlConnection *Oracle8::CreateConnection() {
	return new OCI8Connection(this);
}

bool   Oracle8::IsOpen() const {
	return svchp;
}

bool Oracle8::Open(const String& connect_string) {
	String name, pwd, server;
	const char *b = connect_string, *p = b;
	while(*p && *p != '/' && *p != '@')
		p++;
	name = String(b, p);
	if(*p == '/') {
		b = ++p;
		while(*p && *p != '@')
			p++;
		pwd = String(b, p);
	}
	if(*p == '@')
		server = ++p;
	return Login(name, pwd, server);
}

bool Oracle8::Login(const char *name, const char *pwd, const char *db) {
	LLOG("Oracle8::Login");
	level = 0;
	Logoff();
	ClearError();
	user = ToUpper(String(name));
//	puts("Loading library"); fflush(stdout);
	if(!OCI8().Load())
	{
		SetError(t_("Error running OCI8 Oracle connection dynamic library."), t_("Connecting to Oracle database."));
		return false;
	}
//	puts("OCI8 loaded"); fflush(stdout);
	if(OCI8().OCIInitialize(OCI_THREADED | OCI_OBJECT, NULL, NULL, NULL, NULL)
	|| OCI8().OCIEnvInit(&envhp, OCI_DEFAULT, 0, NULL)
	|| !AllocOciHandle(&errhp, OCI_HTYPE_ERROR)
	|| !AllocOciHandle(&svchp, OCI_HTYPE_SVCCTX)
	|| !AllocOciHandle(&srvhp, OCI_HTYPE_SERVER)
	|| !AllocOciHandle(&seshp, OCI_HTYPE_SESSION))
	{
		Logoff();
		SetError(t_("Error initializing OCI8 library."), t_("Connecting to Oracle database."));
		return false;
	}
//	puts("Attributes allocated"); fflush(stdout);
	if(OCI8().OCIServerAttach(srvhp, errhp, (byte *)db, strlen(db), 0))
	{
		SetError(OciError(errhp), NFormat(t_("Connecting to server '%s'"), db));
		Logoff();
		return false;
	}
//	puts("In server"); fflush(stdout);
	in_server = true;
	if(OCI8().OCIAttrSet(svchp, OCI_HTYPE_SVCCTX, srvhp, 0, OCI_ATTR_SERVER, errhp)
	|| OCI8().OCIAttrSet(seshp, OCI_HTYPE_SESSION, (byte *)name, strlen(name), OCI_ATTR_USERNAME, errhp)
	|| OCI8().OCIAttrSet(seshp, OCI_HTYPE_SESSION, (byte *)pwd, strlen(pwd), OCI_ATTR_PASSWORD, errhp)
	|| OCI8().OCIAttrSet(svchp, OCI_HTYPE_SVCCTX, seshp, 0, OCI_ATTR_SESSION, errhp)
	|| OCI8().OCISessionBegin(svchp, errhp, seshp, OCI_CRED_RDBMS, OCI_DEFAULT))
	{
		SetError(OciError(errhp), t_("Connecting to Oracle database."));
		Logoff();
		return false;
	}
//	puts("In session, user = " + GetUser()); fflush(stdout);
	in_session = true;
	return true;
}

void Oracle8::Logoff() {
	while(!clink.IsEmpty()) {
		clink.GetNext()->Clear();
		clink.GetNext()->Unlink();
	}
	if(in_session)
	{
		in_session = false;
		OCI8().OCISessionEnd(svchp, errhp, seshp, OCI_DEFAULT);
	}
	if(in_server)
	{
		in_server = false;
		OCI8().OCIServerDetach(srvhp, errhp, OCI_DEFAULT);
	}
	FreeOciHandle(seshp, OCI_HTYPE_SESSION);
	seshp = NULL;
	FreeOciHandle(srvhp, OCI_HTYPE_SERVER);
	srvhp = NULL;
	FreeOciHandle(svchp, OCI_HTYPE_SVCCTX);
	svchp = NULL;
	FreeOciHandle(errhp, OCI_HTYPE_ERROR);
	errhp = NULL;
	FreeOciHandle(envhp, OCI_HTYPE_ENV);
	envhp = NULL;
}

void   Oracle8::PostError() {
}

String Oracle8::Spn() {
	return Format("TRANSACTION_LEVEL_%d", level);
}

void   Oracle8::Begin() {
	if(Stream *s = GetTrace())
		*s << user << "(OCI8) -> StartTransaction(level " << level << ")\n";
	level++;
//	ClearError();
	if(level > 1)
		Sql(*this).Execute("savepoint " + Spn());
}

void   Oracle8::Commit() {
	int time = msecs();
	ASSERT(tmode == ORACLE || level > 0);
	if(level)
		level--;
//	else
//		ClearError();
	if(level == 0) {
		OCI8().OCITransCommit(svchp, errhp, OCI_DEFAULT);
//		if(Stream *s = GetTrace())
//			*s << "%commit;\n";
	}
	if(Stream *s = GetTrace())
		*s << NFormat("----- %s (OCI8) -> Commit(level %d) %d ms\n", user, level, msecs(time));
}

void   Oracle8::Rollback() {
	ASSERT(tmode == ORACLE || level > 0);
	if(level > 1)
		Sql(*this).Execute("rollback to savepoint " + Spn());
	else {
		OCI8().OCITransRollback(svchp, errhp, OCI_DEFAULT);
//		if(Stream *s = GetTrace())
//			*s << "%rollback;\n";
	}
	if(level)
		level--;
//	else
//		ClearError();
	if(Stream *s = GetTrace())
		*s << user << "(OCI8) -> Rollback(level " << level << ")\n";
}

String Oracle8::Savepoint() {
	static dword i;
	i = (i + 1) & 0x8fffffff;
	String s = Sprintf("SESSION_SAVEPOINT_%d", i);
	Sql(*this).Execute("savepoint " + s);
	return s;
}

void   Oracle8::RollbackTo(const String& savepoint) {
	Sql(*this).Execute("rollback to savepoint " + savepoint);
}

Oracle8::Oracle8() {
#ifdef PLATFORM_POSIX
	char *orahome = getenv("ORACLE_HOME");
	if(!orahome || !*orahome)
#ifdef CPU_SPARC
		orahome = "/ora_sw/app/oracle/product/9.0.1/";
#else
		orahome = "/opt/oracle/OraHome1";
#endif
#ifdef CPU_SPARC
	OCI8().LibName() = AppendFileName(orahome, "lib32/libclntsh.so");
#else
	OCI8().LibName() = AppendFileName(orahome, "lib/libclntsh.so");
#endif
//	puts("OCI = " + OCI8.LibName());
#endif
	level = 0;
	envhp = NULL;
	errhp = NULL;
	srvhp = NULL;
	seshp = NULL;
	svchp = NULL;
	tmode = NORMAL;
	in_session = in_server = false;
}

Oracle8::~Oracle8() {
	Logoff();
}

Vector<String> Oracle8::EnumUsers()
{
	Sql cursor(*this);
	return OracleSchemaUsers(cursor);
}

Vector<String> Oracle8::EnumDatabases()
{
	Sql cursor(*this);
	return OracleSchemaUsers(cursor);
}

Vector<String> Oracle8::EnumTables(String database)
{
	Sql cursor(*this);
	return OracleSchemaTables(cursor, database);
}

Vector<String> Oracle8::EnumViews(String database)
{
	Sql cursor(*this);
	return OracleSchemaViews(cursor, database);
}

Vector<String> Oracle8::EnumSequences(String database)
{
	Sql cursor(*this);
	return OracleSchemaSequences(cursor, database);
}

Vector<String> Oracle8::EnumPrimaryKey(String database, String table)
{
	Sql cursor(*this);
	return OracleSchemaPrimaryKey(cursor, database, table);
}

String Oracle8::EnumRowID(String database, String table)
{
	Sql cursor(*this);
	return OracleSchemaRowID(cursor, database, table);
}

Vector<String> Oracle8::EnumReservedWords()
{
	return OracleSchemaReservedWords();
}

void OracleBlob::SetStreamSize(int64 pos) {
	ASSERT(pos <= GetMediaSize());
	if(pos < GetMediaSize())
		OCI8().OCILobTrim(session->svchp, session->errhp, locp, pos);
}

dword OracleBlob::Read(int64 at, void *ptr, dword size) {
	ASSERT(IsOpen() && (style & STRM_READ) && session);
	ASSERT(at == (dword)at);
	ub4 n = size;
	if(OCI8().OCILobRead(session->svchp, session->errhp, locp, &n, (dword)at + 1, ptr, size,
	                     NULL, NULL, 0, SQLCS_IMPLICIT) != OCI_SUCCESS) return 0;
	return n;
}

void OracleBlob::Write(int64 at, const void *ptr, dword size) {
	ASSERT(IsOpen() && (style & STRM_WRITE) && session);
	ASSERT(at == (dword)at);
	ub4 n = size;
	if(OCI8().OCILobWrite(session->svchp, session->errhp, locp, &n, (dword)at + 1, (void *)ptr, size,
		OCI_ONE_PIECE, NULL, NULL, 0, SQLCS_IMPLICIT) != OCI_SUCCESS || n != size)
	{
		SetError();
	}
}

void OracleBlob::Assign(Oracle8& s, int blob) {
	session = &s;
	locp = (OCILobLocator *)blob;
	ub4 n;
	OpenInit(READWRITE, OCI8().OCILobGetLength(session->svchp, session->errhp, locp, &n) == OCI_SUCCESS ? n : 0);
}


void OracleBlob::Assign(const Sql& sql, int blob) {
	Oracle8 *session = dynamic_cast<Oracle8 *>(&sql.GetSession());
	ASSERT(session);
	Assign(*session, blob);
}

bool OracleBlob::IsOpen() const {
	return locp;
}

void OracleBlob::Close() {
	if(locp) Flush();
	locp = NULL;
}

OracleBlob::OracleBlob(const Sql& sql, int blob) {
	Assign(sql, blob);
}

OracleBlob::OracleBlob(Oracle8& session, int blob) {
	Assign(session, blob);
}

OracleBlob::OracleBlob() {
	locp = NULL;
	session = NULL;
}

OracleBlob::~OracleBlob() {
	Close();
}
