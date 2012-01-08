#include "Core.h"

NAMESPACE_UPP

#ifndef flagSO
const Nuller Null;
#endif

#define LTIMING(x) // RTIMING(x)

unsigned Value::GetOtherHashValue() const {
	if(IsNull())
		return 0;
	byte st = data.GetSt();
	if(st == REF)
		return ptr()->GetHashValue();
	return svo[st]->GetHashValue(&data);
}

void Value::RefRelease()
{
	ptr()->Release();
}

void Value::RefRetain()
{
	ptr()->Retain();
}

Value& Value::operator=(const Value& v) {
	if(this == &v) return *this;
	Free();
	data = v.data;
	if(IsRef())
		ptr()->Retain();
	return *this;
}

dword Value::GetType() const
{
	if(data.IsString())
		return STRING_V;
	byte st = data.GetSt();
	return st == REF ? ptr()->GetType() : st == VOID ? VOID_V : st;
}

bool Value::IsNull() const
{
	if(IsString())
		return data.GetCount() == 0;
	int st = data.GetSt();
	if(st == VOID)
		return true;
	if(st == REF)
		return ptr()->IsNull();
	return svo[st]->IsNull(&data);
}

bool Value::IsPolyEqual(const Value& v) const
{
	int st = data.GetSpecial();
	if(st == REF)
		return ptr()->IsPolyEqual(v);
	if(svo[st] && svo[st]->IsPolyEqual(&data, v))
		return true;
	return IsNull() && v.IsNull();
}

bool Value::operator==(const Value& v) const {
	if(IsString() && v.IsString())
		return data == v.data;
	if(GetType() != v.GetType())
		return IsPolyEqual(v) || v.IsPolyEqual(*this);
	int st = data.GetSpecial();
	if(st == REF)
		return ptr()->IsEqual(v.ptr());
	if(st == VOID)
		return v.IsVoid();
	return svo[st]->IsEqual(&data, &v.data);
}

Value::Value(const WString& s) { InitRef(new RichValueRep<WString>(s)); }

Value::operator WString() const
{
	if(IsNull()) return Null;
	return GetType() == WSTRING_V ? RichValue<WString>::Extract(*this)
		                          : ((String)(*this)).ToWString();//!!!
}

Date Value::GetOtherDate() const
{
	if(IsNull()) return Null;
	return GetSmall<Time>();
}

Time Value::GetOtherTime() const
{
	if(IsNull()) return Null;
	return ToTime(GetSmall<Date>());	
}

String Value::GetOtherString() const
{
	if(IsNull()) return Null;
	return RichValue<WString>::Extract(*this).ToString();
}

int Value::GetOtherInt() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(BOOL_V) ? (int)GetSmall<bool>() :
	       data.IsSpecial(INT64_V) ? (int)GetSmall<int64>() :
	       (int)GetSmall<double>();
}

int64 Value::GetOtherInt64() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(BOOL_V) ? (int64)GetSmall<bool>() :
	       data.IsSpecial(INT_V) ? (int64)GetSmall<int>() :
	       (int64)GetSmall<double>();
}

double Value::GetOtherDouble() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(BOOL_V) ? (double)GetSmall<bool>() :
	       data.IsSpecial(INT_V) ? (double)GetSmall<int>() :
	       (double)GetSmall<int64>();
}

bool Value::GetOtherBool() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(DOUBLE_V) ? (bool)GetSmall<double>() :
	       data.IsSpecial(INT_V) ? (bool)GetSmall<int>() :
	       (bool)GetSmall<int64>();
}

VectorMap<dword, Value::Void *(*)(Stream& s)>& Value::Typemap()
{
	static VectorMap<dword, Value::Void *(*)(Stream& s)> x;
	return x;
}

SVO_FN(s_String, String);
SVO_FN(s_int, int);
SVO_FN(s_double, double);
SVO_FN(s_int64, int64);
SVO_FN(s_bool, bool);
SVO_FN(s_date, Date);
SVO_FN(s_time, Time);

struct SvoVoidFn {
	static bool       IsNull(const void *p)                      { return true; }
	static void       Serialize(void *p, Stream& s)              {}
	static unsigned   GetHashValue(const void *p)                { return 0; }
	static bool       IsEqual(const void *p1, const void *p2)    { return true; }
	static bool       IsPolyEqual(const void *p, const Value& v) { return false; }
	static String     AsString(const void *p)                    { return String(); }
};

static Value::Sval s_void[] = { \
	SvoVoidFn::IsNull, SvoVoidFn::Serialize, SvoVoidFn::GetHashValue, SvoVoidFn::IsEqual,
	SvoVoidFn::IsPolyEqual, SvoVoidFn::AsString
};

Value::Sval *Value::svo[256] = {
	s_String, // STRING_V
	s_int, // INT_V

	s_double, //DOUBLE_V  = 2;
	s_void, //VOID_V  = 3;
	s_date, //DATE_V    = 4;
	s_time, //TIME_V    = 5;

	NULL, //ERROR_V   = 6;

	NULL, //VALUE_V   = 7;

	NULL, //WSTRING_V = 8;

	NULL, //VALUEARRAY_V = 9;

	s_int64, //INT64_V  = 10;
	s_bool, //BOOL_V   = 11;

	NULL, //VALUEMAP_V   = 12;
};

Value::Void *ValueArrayDataCreate(Stream& s)
{
	ValueArray::Data *a = new ValueArray::Data;
	a->Serialize(s);
	return a;
}

Value::Void *ValueMapDataCreate(Stream& s)
{
	ValueMap::Data *a = new ValueMap::Data;
	a->Serialize(s);
	return a;
}

static void sRegisterStd()
{
	ONCELOCK {
//		RichValue<int>::Register();
//		RichValue<int64>::Register();
//		RichValue<bool>::Register();
//		RichValue<double>::Register();
//		RichValue<String>::Register();
		RichValue<WString>::Register();
//		RichValue<Date>::Register();
//		RichValue<Time>::Register();
		RichValue<Complex>::Register();
		Value::Register(VALUEARRAY_V, ValueArrayDataCreate);
		Value::Register(VALUEMAP_V, ValueMapDataCreate);
	};
}

INITBLOCK {
	sRegisterStd();
}


void Value::Serialize(Stream& s) {
	sRegisterStd();
	dword type;
	if(s.IsLoading()) {
		s / type;
		Free();
		int st = type == VOID_V ? VOID : type == STRING_V ? STRING : type;
		if(st == STRING)
			s % data;
		else
		if(st < 255 && svo[st]) {
			data.SetSpecial(type);
			svo[st]->Serialize(&data, s);
		}
		else {
			typedef Void* (*vp)(Stream& s);
			vp *cr = Typemap().FindPtr(type);
			if(cr)
				InitRef((**cr)(s));
			else {
				Free();
				data.SetSpecial(3);
				if(type != VOID_V && type != ERROR_V)
					s.LoadError();
			}
		}
	}
	else {
		type = GetType();
		s / type;
		int st = data.GetSpecial();
		ASSERT_(!type || type == ERROR_V || type == UNKNOWN_V || st == STRING ||
		        (IsRef() ? Typemap().Find(type) >= 0 : st < 255 && svo[st]),
		        AsString(type) + " is not registred for serialization");
		if(st == VOID)
			return;
		if(st == STRING)
			s % data;
		else
		if(IsRef())
			ptr()->Serialize(s);
		else
			svo[st]->Serialize(&data, s);
	}
}

void Value::Register(dword w, Void* (*c)(Stream& s)) init_ {
#ifdef flagCHECKINIT
	RLOG("Register valuetype " << w);
#endif
	AssertST();
	ASSERT(w != UNKNOWN_V);
	ASSERT(w < 0x8000000);
	CHECK(Typemap().GetAdd(w, c) == c);
}

String  Value::ToString() const {
	if(IsNull())
		return Null;
	if(IsString())
		return data;
	if(IsRef())
		return ptr()->AsString();
	int st = data.GetSpecial();
	return svo[st]->AsString(&data);
}

class ValueErrorCls : public RichValueRep<String> {
public:
	virtual dword      GetType() const             { return ERROR_V; }
	virtual bool       IsNull() const              { return true; }
	virtual void       Serialize(Stream& s)        {}
	virtual String     AsString() const            { return "<error: \'" + v + "\'>"; }

	ValueErrorCls(const String& s) : RichValueRep<String>(s)  {}
};

Value ErrorValue(const String& s) {
	return Value(new ValueErrorCls(s));
}

Value ErrorValue(const char *s) {
	return ErrorValue(String(s));
}

const Value& ErrorValue() {
	static Value *p;
	ONCELOCK {
		static Value v = ErrorValue(String());
		p = &v;
	}
	return *p;
}

String GetErrorText(const Value& v) {
	ASSERT(IsError(v));
	return ((RichValueRep<String> *)v.GetVoidPtr())->Get();
}

END_UPP_NAMESPACE
