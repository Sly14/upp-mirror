#include "Core.h"

NAMESPACE_UPP

unsigned stou(const char *s, void *endptr, unsigned base)
{
	ASSERT(base >= 2 && base <= 36);
	unsigned digit = ctoi(*s);
	if(digit >= base)
	{ // error
		if(endptr)
			*(const char **)endptr = s;
		return ~0;
	}
	unsigned value = digit;
	while((digit = ctoi(*++s)) < base)
		value = value * base + digit;
	if(endptr)
		*(const char **)endptr = s;
	return value;
}

unsigned stou(const wchar *s, void *endptr, unsigned base)
{
	ASSERT(base >= 2 && base <= 36);
	unsigned digit = ctoi(*s);
	if(digit >= base)
	{ // error
		if(endptr)
			*(const wchar **)endptr = s;
		return ~0;
	}
	unsigned value = digit;
	while((digit = ctoi(*++s)) < base)
		value = value * base + digit;
	if(endptr)
		*(const wchar **)endptr = s;
	return value;
}


uint64 stou64(const char *s, void *endptr, unsigned base)
{
	ASSERT(base >= 2 && base <= 36);
	unsigned digit = ctoi(*s);
	if(digit >= base)
	{ // error
		if(endptr)
			*(const char **)endptr = s;
		return ~0;
	}
	uint64 value = digit;
	while((digit = ctoi(*++s)) < base)
		value = value * base + digit;
	if(endptr)
		*(const char **)endptr = s;
	return value;
}

int ScanInt(const char *ptr, const char **endptr, int base)
{
	const char *s = ptr;
	bool minus = false;
	while(*s && (byte)*s <= ' ')
		s++;
	if(*s == '+' || *s == '-')
		minus = (*s++ == '-');
	unsigned u = stou(s, endptr, base);
	if(~u)
		return (minus ? -(int)u : (int)u);
	else
		return Null;
}

int ScanInt(const wchar *ptr, const wchar **endptr, int base)
{
	const wchar *s = ptr;
	bool minus = false;
	while(*s && *s <= ' ')
		s++;
	if(*s == '+' || *s == '-')
		minus = (*s++ == '-');
	unsigned u = stou(s, endptr, base);
	if(~u)
		return (minus ? -(int)u : (int)u);
	else
		return Null;
}

int64 ScanInt64(const char *ptr, const char **endptr, int base)
{
	const char *s = ptr;
	bool minus = false;
	while(*s && *s <= ' ')
		s++;
	if(*s == '+' || *s == '-')
		minus = (*s++ == '-');
	uint64 u = stou64(s, endptr, base);
	if(~u)
		return (minus ? -(int64)u : (int64)u);
	else
		return Null;
}

double ScanDouble(const char *p, const char **endptr, bool accept_comma)
{
	const char *begin = p;
	while(*p && (byte)*p <= ' ')
		p++;
	bool neg = false;
	if(*p == '+' || *p == '-')
		neg = (*p++ == '-');
	if((byte)(*p - '0') >= 10 && !((*p == '.' || accept_comma && *p == ',') && (byte)(p[1] - '0') < 10)) {
		if(endptr) *endptr = begin;
		return Null;
	}
	double mantissa = 0;
	char c;
	int exp = 0;
	while((byte)(*p - '0') < 10)
		if((c = *p++) != '0') {
			if(exp) { mantissa *= ipow10(exp); exp = 0; }
			mantissa = 10 * mantissa + c - '0';
		}
		else
			exp++;
	int raise = exp;
	if(*p == '.' || accept_comma && *p == ',') // decimal part
		while((byte)((c = *++p) - '0') < 10)
			if(c != '0') {
				if(raise) { mantissa *= ipow10(raise); exp -= raise; raise = 0; }
				exp--;
				mantissa = 10 * mantissa + c - '0';
			}
			else
				raise++;
	if(*p == 'E' || *p == 'e') { // exponent
		int vexp = ScanInt(p + 1, endptr);
		if(!IsNull(vexp))
			exp += vexp;
	}
	else
		if(endptr) *endptr = p;
	if(exp) {
		double e = ipow10(tabs(exp));
		mantissa = (exp > 0 ? mantissa * e : mantissa / e);
	}
	return neg ? -mantissa : mantissa;
}

double ScanDouble(const wchar *p, const wchar **endptr, bool accept_comma)
{
	const wchar *begin = p;
	while(*p && *p <= ' ')
		p++;
	bool neg = false;
	if(*p == '+' || *p == '-')
		neg = (*p++ == '-');
	if((unsigned)(*p - '0') >= 10) {
		if(endptr) *endptr = begin;
		return Null;
	}
	double mantissa = 0;
	wchar c;
	int exp = 0;
	while((unsigned)(*p - '0') < 10)
		if((c = *p++) != '0') {
			if(exp) { mantissa *= ipow10(exp); exp = 0; }
			mantissa = 10 * mantissa + c - '0';
		}
		else
			exp++;
	int raise = exp;
	if(*p == '.' || accept_comma && *p == ',') // decimal part
		while((unsigned)((c = *++p) - '0') < 10)
			if(c != '0') {
				if(raise) { mantissa *= ipow10(raise); exp -= raise; raise = 0; }
				exp--;
				mantissa = 10 * mantissa + c - '0';
			}
			else
				raise++;
	if(*p == 'E' || *p == 'e') { // exponent
		int vexp = ScanInt(p + 1, endptr);
		if(!IsNull(vexp))
			exp += vexp;
	}
	else
		if(endptr) *endptr = p;
	if(exp) {
		double e = ipow10(tabs(exp));
		mantissa = (exp > 0 ? mantissa * e : mantissa / e);
	}
	return neg ? -mantissa : mantissa;
}

Value StrIntValue(const char *s)
{
	if(s && *s) {
		int64 q = ScanInt64(s);
		return IsNull(q) ? ErrorValue(t_("Invalid number !")) : Value(q);
	}
	return Null;
}

Value StrDblValue(const char *s)
{
	if(s && *s) {
		double q = ScanDouble(s);
		return IsNull(q) ? ErrorValue(t_("Invalid number !")) : Value(q);
	}
	return Null;
}

Value Scan(dword qtype, const String& text) {
	Date date;
	const char *s;
	switch(qtype) {
	case INT64_V:
	case INT_V:
	case BOOL_V:
		return StrIntValue(text);
	case DATE_V:
		if(text.IsEmpty()) return (Date) Null;
		s = StrToDate(date, text);
		if(s)
			for(;;) {
				if(IsDigit(*s))
					break;
				if(*s == '\0')
					return date;
				s++;
			}
		return ErrorValue(t_("Invalid date !"));
	case TIME_V: {
		if(text.IsEmpty()) return (Time) Null;
		s = StrToDate(date, text);
		if(s)
			try {
				CParser p(s);
				Time tm = ToTime(date);
				if(p.IsEof())
					return tm;
				int q = p.ReadInt();
				if(q < 0 || q > 23)
					throw CParser::Error("");
				tm.hour = q;
				if(p.IsEof())
					return tm;
				p.PassChar(':');
				q = p.ReadInt();
				if(q < 0 || q > 59)
					throw CParser::Error("");
				tm.minute = q;
				if(p.IsEof())
					return tm;
				p.PassChar(':');
				q = p.ReadInt();
				if(q < 0 || q > 59)
					throw CParser::Error("");
				tm.second = q;
				if(p.IsEof())
					return tm;
			}
			catch(CParser::Error) {}
		return ErrorValue(t_("Invalid time !"));
	}
	case STRING_V:
	case WSTRING_V:
		return text;
	case DOUBLE_V:
		return StrDblValue(text);
	default:
		ASSERT(0);
		break;
	}
	return Null;
}

#ifdef flagSO
Convert::Convert() {}
Convert::~Convert() {}
#endif

Value  Convert::Format(const Value& q) const {
	if(IsVoid(q) || q.IsNull()) return String();
	switch(q.GetType()) {
	case INT64_V:
		return IntStr64((int64)q);
	case INT_V:
	case BOOL_V:
		return IntStr((int)q);
	case DOUBLE_V:
		return DblStr((double)q);
	case DATE_V:
		return UPP::Format(Date(q));
	case TIME_V:
		return UPP::Format(Time(q));
	case STRING_V:
	case WSTRING_V:
		return q;
	}
	return q.ToString();
}

Value  Convert::Scan(const Value& text) const {
	return text;
};

int    Convert::Filter(int chr) const {
	return chr;
}

GLOBAL_VAR_INIT(const Convert, StdConvert);

String StdFormat(const Value& q) {
	return StdConvert().Format(q);
}

Value NotNullError() {
	return ErrorValue(t_("Null value not allowed."));
}

#ifdef flagSO
ConvertInt::ConvertInt(int minval, int maxval, bool notnull)
: minval(minval), maxval(maxval), notnull(notnull) {}
ConvertInt::~ConvertInt() {}

ConvertInt64::ConvertInt64(int64 minval, int64 maxval, bool notnull) {
	MinMax(minval, maxval); NotNull(notnull);
}
ConvertInt64::~ConvertInt64() {}
#endif

Value ConvertInt::Scan(const Value& text) const {
	Value v = UPP::Scan(INT_V, text);
	if(IsError(v)) return v;
	if(IsNull(v)) return notnull ? NotNullError() : v;
	int64 m = v;
	if(m >= minval && m <= maxval) return v;
	return ErrorValue(UPP::Format(t_("Number must be between %d and %d."), minval, maxval));
}

int   ConvertInt::Filter(int chr) const {
	return minval >= 0 ? CharFilterDigit(chr) : CharFilterInt(chr);
}

Value ConvertDouble::Format(const Value& q) const
{
	if(IsNull(q))
		return Null;
	return UPP::NFormat(pattern, (double)q);
}

Value ConvertDouble::Scan(const Value& text) const {
	Value v = UPP::Scan(DOUBLE_V, text);
	if(IsError(v)) return v;
	if(IsNull(v)) return notnull ? NotNullError() : v;
	double m = v;
	if(m >= minval && m <= maxval) return v;
	return ErrorValue(UPP::Format(t_("Number must be between %g and %g."), minval, maxval));
}

int   ConvertDouble::Filter(int chr) const {
	return CharFilterDouble(chr);
}

ConvertDouble::ConvertDouble(double minval, double maxval, bool notnull)
  : minval(minval), maxval(maxval), notnull(notnull)
{
	pattern = "%.10g";
}

#ifdef flagSO
ConvertDouble::~ConvertDouble() {}
#endif

ConvertDate::ConvertDate(Date minval, Date maxval, bool notnull)
: minval(minval), maxval(maxval), notnull(notnull) {}

ConvertDate::~ConvertDate()
{
}

Value ConvertDate::Scan(const Value& text) const {
	Value v = UPP::Scan(DATE_V, text);
	if(IsError(v)) return v;
	if(IsNull(v)) return notnull ? NotNullError() : v;
	Date m = v;
	if(m >= minval && m <= maxval) return v;
	return ErrorValue(t_("Date must be between ") + UPP::Format(minval) + t_("range\v and ") + UPP::Format(maxval) + ".");
}

int   ConvertDate::Filter(int chr) const {
	return CharFilterDate(chr);
}

ConvertTime::ConvertTime(Time minval, Time maxval, bool notnull)
: minval(minval), maxval(maxval), notnull(notnull), seconds(true) {}

ConvertTime::~ConvertTime()
{
}

Value ConvertTime::Scan(const Value& text) const
{
	Value v = UPP::Scan(TIME_V, text);
	if(IsError(v)) return v;
	if(IsNull(v)) return notnull ? NotNullError() : v;
	Time m = v;
	if(m >= minval && m <= maxval) return v;
	return ErrorValue(t_("Time must be between ") + UPP::Format(minval) + t_("range\v and ") + UPP::Format(maxval) + ".");
}

int ConvertTime::Filter(int chr) const
{
	if(IsDigit(chr) || chr == ' ' || chr == '.' || chr == ':')
		return chr;
	if(chr == ',')
		return '.';
	return 0;
}

Value ConvertTime::Format(const Value& q) const
{
	if(IsVoid(q) || q.IsNull())
		return String();
	else if(q.GetType() == TIME_V)
		return UPP::Format(Time(q), seconds);
	else
		return Convert::Format(q);
}

#ifdef flagSO
ConvertString::ConvertString(int maxlen, bool notnull) : maxlen(maxlen), notnull(notnull) {}
ConvertString::~ConvertString() {}
#endif

Value ConvertString::Scan(const Value& text) const {
	if(IsError(text)) return text;
	if(IsNull(text)) return notnull ? NotNullError() : Value(text);
	if(text.GetType() == STRING_V && String(text).GetLength() <= maxlen ||
	   text.GetType() == WSTRING_V && WString(text).GetLength() <= maxlen) return text;
	return ErrorValue(UPP::Format(t_("Please enter no more than %d characters."), maxlen));
}

GLOBAL_VAR_INIT(const ConvertInt, StdConvertInt)
GLOBAL_VARP_INIT(const ConvertInt, StdConvertIntNotNull, (-INT_MAX, INT_MAX, true))
GLOBAL_VAR_INIT(const ConvertDouble, StdConvertDouble)
GLOBAL_VARP_INIT(const ConvertDouble, StdConvertDoubleNotNull,
            (-DOUBLE_NULL_LIM, DOUBLE_NULL_LIM, true))
GLOBAL_VAR_INIT(const ConvertDate, StdConvertDate)
GLOBAL_VARP_INIT(const ConvertDate, StdConvertDateNotNull, (Date(0, 0, 0), Date(3000, 12, 31), true))
GLOBAL_VAR_INIT(const ConvertTime, StdConvertTime)
GLOBAL_VARP_INIT(const ConvertTime, StdConvertTimeNotNull, (Null, Null, true))
GLOBAL_VAR_INIT(const ConvertString, StdConvertString);
GLOBAL_VARP_INIT(const ConvertString, StdConvertStringNotNull, (INT_MAX, true))

Value  MapConvert::Format(const Value& q) const {
	return map.Get(q, Null);
}

Value NoConvertClass::Format(const Value& q) const {
	return q;
}

GLOBAL_VAR_INIT(const NoConvertClass, NoConvert)

Value JoinConvert::Format(const Value& v) const {
	String r;
	ValueArray a = v;
	for(int i = 0; i < item.GetCount(); i++) {
		const Item& m = item[i];
		if(m.pos < 0)
			r << m.text;
		else
			r << (String) StdConvert().Format(m.convert->Format(a[m.pos]));
	}
	return r;
}

int JoinConvert::NextPos() const {
	for(int i = item.GetCount() - 1; i >= 0; i--)
		if(item[i].pos >= 0) return item[i].pos + 1;
	return 0;
}

JoinConvert& JoinConvert::Add(const char *text) {
	Item& m = item.Add();
	m.pos = -1;
	m.text = text;
	return *this;
}

JoinConvert& JoinConvert::Add(int pos, const Convert& cv) {
	ASSERT(pos >= 0);
	Item& m = item.Add();
	m.pos = pos;
	m.convert = &cv;
	return *this;
}

JoinConvert& JoinConvert::Add(int pos) {
	Add(pos, StdConvert());
	return *this;
}

JoinConvert& JoinConvert::Add(const Convert& cv) {
	Add(NextPos(), cv);
	return *this;
}

JoinConvert& JoinConvert::Add() {
	Add(NextPos());
	return *this;
}

Value FormatConvert::Format(const Value& v) const
{
	ValueArray va;
	if(IsValueArray(v))
		va = v;
	else
		va.Add(v);
	return UPP::Format(format, va.Get());
}

END_UPP_NAMESPACE
