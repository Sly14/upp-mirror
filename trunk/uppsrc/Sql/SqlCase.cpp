#include "Sql.h"

#define LTIMING(x)

NAMESPACE_UPP

template <class T>
String MakeSqlValue(int code, T& value)
{
	StringBuffer b(sizeof(T) + 1);
	b[0] = code;
	memcpy(~b + 1, &value, sizeof(T));
	return b;
}

template <class T>
T ReadSqlValue(T& x, const char *&s) {
	memcpy(&x, s, sizeof(T));
	s += sizeof(T);
	return x;
}

static bool sSqlIdQuoted;

void SqlId::UseQuotes(bool b)
{
	sSqlIdQuoted = b;
}

String SqlId::Quoted() const
{
	if(sSqlIdQuoted && !id.IsNull())
		return String().Cat() << '\t' << id << '\t';
	return id.ToString();
}

void SqlCompile(const char *&s, StringBuffer *r, byte dialect)
{
	char quote = dialect == MY_SQL ? '`' : '\"';
	for(;;) {
		int c = *s++;
		switch(c) {
		case SQLC_OF:
			if(r)
				*r << '.';
			break;
		case SQLC_AS:
			if(r) {
				if(dialect & (MSSQL | PGSQL))
					*r << " as ";
				else
					*r << ' ';
			}
			break;
		case SQLC_COMMA:
			if(r)
				*r << ", ";
			break;
		case SQLC_ID: {
				for(;;) {
					const char *b = s;
					bool do_quote = sSqlIdQuoted && *s != '*';
					while((byte)*s >= 32)
						s++;
					int c = *s;
					if(r) {
						if(do_quote)
							*r << quote;
						r->Cat(b, s);
						if(do_quote)
							*r << quote;
						if(c == SQLC_AS) {
							if(dialect & (MSSQL | PGSQL))
								*r << " as ";
							else
								*r << ' ';
						}
						else
						if(c == SQLC_OF)
							*r << '.';
						else
						if(c == SQLC_COMMA)
							*r << ", ";
					}
					s++;
					if(c == SQLC_ID)
						break;
					if(c == '\0')
						return;
				}
			}
			break;
		case SQLC_IF: {
			LTIMING("SqlCompile IF");
			StringBuffer *er = r;
			for(;;) {
				c = *s++;
				if(c & dialect) {
					SqlCompile(s, er, dialect);
					er = NULL;
				}
				else
					SqlCompile(s, NULL, dialect);
				if(*s == '\0')
					return;
				c = *s++;
				if(c == SQLC_ELSE) {
					SqlCompile(s, er, dialect);
					ASSERT(*s == SQLC_ENDIF);
					s++;
					break;
				}
				if(c == SQLC_ENDIF)
					break;
				ASSERT(c == SQLC_ELSEIF);
			}
		}
		break;
		case SQLC_DATE: {
			LTIMING("SqlCompile DATE");
			Date x;
			ReadSqlValue(x, s);
			if(!r) break;
			if(IsNull(x)) {
				*r << "NULL";
				break;
			}
			switch(dialect) {
			case MSSQL:
				if(x.year < 1753) x.year = 1753; // Date::Low()
				*r << Format("convert(datetime, '%d/%d/%d', 120)", x.year, x.month, x.day);
				break;
			case ORACLE:
				*r << Format("to_date('%d/%d/%d', 'SYYYY/MM/DD')", x.year, x.month, x.day);
				break;
			case PGSQL:
				if(x.year < 1) x.year = 1; // Date::Low()
				*r << "date ";
			default:
				*r << Format("\'%04d-%02d-%02d\'", x.year, x.month, x.day);
			}
			break;
		}
		case SQLC_TIME: {
			LTIMING("SqlCompile TIME");
			Time x;
			ReadSqlValue(x, s);
			if(!r) break;
			if(IsNull(x)) {
				*r << "NULL";
				break;
			}
			switch(dialect) {
			case MSSQL:
				if(x.year < 1753) x.year = 1753; // Date::Low()
				*r << Format(x.hour || x.minute || x.second
				             ? "convert(datetime, '%d/%d/%d %d:%d:%d', 120)"
				             : "convert(datetime, '%d/%d/%d', 120)",
				             x.year, x.month, x.day, x.hour, x.minute, x.second);
				break;
			case ORACLE:
				*r << Format("to_date('%d/%d/%d/%d', 'SYYYY/MM/DD/SSSSS')",
				             x.year, x.month, x.day, x.second + 60 * (x.minute + 60 * x.hour));
				break;
			case PGSQL:
				if(x.year < 1) x.year = 1; // Date::Low()
				*r << "timestamp ";
			default:
				*r << Format("\'%04d-%02d-%02d %02d:%02d:%02d\'",
				             x.year, x.month, x.day, x.hour, x.minute, x.second);
			}
			break;
		}
		case SQLC_BINARY: {
			int l;
			ReadSqlValue(l, s);
			if(r) {
				if(l == 0)
					*r << "NULL";
				else
					switch(dialect) {
					case PGSQL: {
						*r << "E\'";
						const char *e = s + l;
						while(s < e) {
							byte c = *s++;
							if(c < 32 || c > 126 || c == 39 || c == 92) {
								*r << '\\' << '\\';
								r->Cat(((c >> 6) & 3) + '0');
								r->Cat(((c >> 3) & 7) + '0');
								r->Cat((c & 7) + '0');
							}
							else
								r->Cat(c);
						}
						*r << "\'::bytea";
						break;
					}
					case MSSQL:
						*r << "0x" << HexString(s, l);
						s += l;
						break;
					case SQLITE3:
					case MY_SQL:
						*r << "X";
					default:
						*r << "\'" << HexString(s, l) << "\'";
						s += l;
						break;
					}
			}
			break;
		}
		case SQLC_STRING: {
			LTIMING("SqlCompile STRING");
			int l;
			ReadSqlValue(l, s);
			String x = String(s, l);
			s += l;
			if(!r) break;
			if(IsNull(x)) {
				*r << "NULL";
				break;
			}
			if(dialect == PGSQL)
				r->Cat('E');
			r->Cat('\'');
			for(const char *q = x; *q; q++) {
				int c = (byte)*q;
				if(c == '\'') {
					if(dialect == MY_SQL)
						r->Cat("\\\'");
					else if(dialect == PGSQL)
						r->Cat("\\'");
					else
					 	r->Cat("\'\'");
				}
				else {
					if((c == '\"' || c == '\\') && (dialect == MY_SQL || dialect == PGSQL))
						r->Cat('\\');
					if(dialect == PGSQL && c < 32) {
						if(c == '\n')
							r->Cat("\\n");
						else
						if(c == '\r')
							r->Cat("\\r");
						else
						if(c == '\t')
							r->Cat("\\t");
						else {
							char h[4];
							h[0] = '\\';
							h[1] = (3 & (c >> 6)) + '0';
							h[2] = (7 & (c >> 3)) + '0';
							h[3] = (7 & c) + '0';
							r->Cat(h, 4);
						}
					}
					else
						r->Cat(c);
				}
			}
			r->Cat('\'');
			break;
		}
		default:
			if(c >= 0 && c < 32) {
				s--;
				return;
			}
			else
				if(r) {
					const char *p = s - 1;
					while((byte)*s >= 32)
						s++;
					r->Cat(p, s);
				}
		}
	}
}

String SqlCompile(byte dialect, const String& s)
{
	StringBuffer b;
	b.Reserve(s.GetLength() + 100);
	const char *q = s;
	SqlCompile(q, &b, dialect);
	return b;
}

String SqlFormat(int x)
{
	if(IsNull(x)) return "NULL";
	return Format("%d", x);
}

String SqlFormat(double x)
{
	if(IsNull(x)) return "NULL";
	return FormatDouble(x, 20);
}

String SqlFormat(int64 x)
{
	if(IsNull(x)) return "NULL";
	return FormatInt64(x);
}

String SqlFormat0(const char *s, int l, int code)
{
	StringBuffer b(1 + sizeof(int) + l);
	b[0] = code;
	memcpy(~b + 1, &l, sizeof(int));
	memcpy(~b + 1 + sizeof(int), s, l);
	return b;
}

String SqlFormat(const char *s, int l)
{
	return SqlFormat0(s, l, SQLC_STRING);
}

String SqlFormat(const char *s)
{
	return SqlFormat(s, (int)strlen(s));
}

String SqlFormat(const String& x)
{
	return SqlFormat(x, x.GetLength());
}

String SqlFormatBinary(const char *s, int l)
{
	return SqlFormat0(s, l, SQLC_BINARY);
}

String SqlFormatBinary(const String& x)
{
	return SqlFormatBinary(x, x.GetLength());
}

String SqlFormat(Date x)
{
	return MakeSqlValue(SQLC_DATE, x);
}

String SqlFormat(Time x)
{
	return MakeSqlValue(SQLC_TIME, x);
}

String SqlFormat(const Value& x)
{
	if(x.IsNull()) return "NULL";
	switch(x.GetType()) {
	case BOOL_V:
	case INT_V:
		return SqlFormat((int) x);
	case INT64_V:
		return SqlFormat((int64) x);
	case DOUBLE_V:
		return SqlFormat((double) x);
	case STRING_V:
	case WSTRING_V:
		return SqlFormat(String(x));
	case DATE_V:
		return SqlFormat(Date(x));
	case TIME_V:
		return SqlFormat(Time(x));
	}
	NEVER();
	return "NULL";
}

String SqlCase::operator()(const String& text) {
	return s << (char)SQLC_ELSE << text << (char)SQLC_ENDIF;
}

String SqlCase::operator()() {
	return s << (char)SQLC_ENDIF;
}

SqlCase SqlCase::operator()(byte cond, const String& text) {
	s << (char)SQLC_ELSEIF << (char)cond << text;
	return *this;
}

SqlCase::SqlCase(byte cond, const String& text) {
	s << (char)SQLC_IF << (char)cond << text;
}

// Put to different file to force non-inline (size opt)
void SqlId::PutOf0(String& s, const SqlId& b) const
{
	s << ToString() << (char)SQLC_OF << ~b;
}

void SqlId::PutOf(String& s, const SqlId& b) const
{
	s << (char)SQLC_COMMA;
	PutOf0(s, b);
}

END_UPP_NAMESPACE
