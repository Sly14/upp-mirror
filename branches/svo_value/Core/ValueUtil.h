Value Scan(dword stdtype, const String& text, const Value& def = Null);

inline const String&  Nvl(const String& a, const String& b)    { return IsNull(a) ? b : a; }
inline int            Nvl(int a, int b)                        { return IsNull(a) ? b : a; }
inline int64          Nvl(int64 a, int64 b)                    { return IsNull(a) ? b : a; }
inline double         Nvl(double a, double b)                  { return IsNull(a) ? b : a; }
inline Date           Nvl(Date a, Date b)                      { return IsNull(a) ? b : a; }
inline Time           Nvl(Time a, Time b)                      { return IsNull(a) ? b : a; }

inline int            Nvl(int a)                               { return Nvl(a, 0); }
inline int64          Nvl(int64 a)                             { return Nvl(a, (int64)0); }
inline double         Nvl(double a)                            { return Nvl(a, 0.0); }

int StdValueCompare(const Value& a, const Value& b, int language);
int StdValueCompare(const Value& a, const Value& b);

int StdValueCompareDesc(const Value& a, const Value& b, int language);
int StdValueCompareDesc(const Value& a, const Value& b);

struct ValueOrder {
	virtual bool operator()(const Value& a, const Value& b) const = 0;
	virtual ~ValueOrder() {}
};

struct StdValueOrder : ValueOrder {
	int language;

	virtual bool operator()(const Value& a, const Value& b) const;

	StdValueOrder(int l = -1);
};

struct FnValueOrder : ValueOrder {
	int (*fn)(const Value& a, const Value& b);

	virtual bool operator()(const Value& a, const Value& b) const;

	FnValueOrder(int (*fn)(const Value& a, const Value& b)) : fn(fn) {}
};

struct ValuePairOrder {
	virtual bool operator()(const Value& keya, const Value& valuea, const Value& keyb, const Value& valueb) const = 0;
	virtual ~ValuePairOrder() {}
};

struct StdValuePairOrder : ValuePairOrder {
	int language;

	virtual bool operator()(const Value& keya, const Value& valuea, const Value& keyb, const Value& valueb) const;

	StdValuePairOrder(int l = -1);
};

struct FnValuePairOrder : ValuePairOrder {
	int (*fn)(const Value& k1, const Value& v1, const Value& k2, const Value& v2);

	virtual bool operator()(const Value& keya, const Value& valuea, const Value& keyb, const Value& valueb) const;

	FnValuePairOrder(int (*fn)(const Value& k1, const Value& v1, const Value& k2, const Value& v2)) : fn(fn) {}
};

class Id : Moveable<Id> {
	String id;

public:
	const String&  ToString() const              { return id; }
	dword          GetHashValue() const          { return UPP::GetHashValue(id); }
	bool           IsNull() const                { return UPP::IsNull(id); }

	operator const String&() const               { return ToString(); }
	const String&  operator~() const             { return ToString(); }
	bool           operator==(const Id& b) const { return id == b.id; }
	bool           operator!=(const Id& b) const { return id != b.id; }

	operator bool() const                        { return id.GetCount(); }

	Id()                                         {}
	Id(const String& s)                          { id = s; }
	Id(const char *s)                            { id = s; }
};

struct RefManager {
	virtual int   GetType() = 0;
	virtual Value GetValue(const void *)            { return Null; }
	virtual bool  IsNull(const void *)              { return false; }
	virtual void  SetValue(void *, const Value& v)  { NEVER(); }
	virtual void  SetNull(void *)                   { NEVER(); }
	virtual ~RefManager() {}
};


template <class T>
struct RawRef : public RefManager {
	virtual void  SetValue(void *p, const Value& v) { *(T *) p = RawValue<T>::Extract(v); }
	virtual Value GetValue(const void *p)           { return RawValue<T>(*(const T *) p); }
	virtual int   GetType()                         { return GetValueTypeNo<T>(); }
	virtual ~RawRef() {}
};

template <class T>
struct RichRef : public RawRef<T> {
	virtual Value GetValue(const void *p)           { return RichValue<T>(*(T *) p); }
	virtual bool  IsNull(const void *p)             { return UPP::IsNull(*(T *) p); }
	virtual void  SetValue(void *p, const Value& v) { *(T *) p = T(v); }
	virtual void  SetNull(void *p)                  { UPP::SetNull(*(T *)p); }
};

class Ref : Moveable<Ref> {
protected:
	RefManager *m;
	void       *ptr;
	struct      ValueRef;

public:
	dword    GetType() const           { return m ? m->GetType() : VOID_V; }
	bool     IsNull() const            { return m ? m->IsNull(ptr) : true; }

	void    *GetVoidPtr() const        { return ptr; }

	void     SetNull()                 { if(m) m->SetNull(ptr); }
	Value    GetValue() const          { return m ? m->GetValue(ptr) : Value(); }
	void     SetValue(const Value& v)  { ASSERT(m); m->SetValue(ptr, v); }

	operator Value() const             { return GetValue(); }
	Value    operator~() const         { return GetValue(); }
	Ref&     operator=(const Value& v) { SetValue(v); return *this; }

	Ref(String& s);
	Ref(WString& s);
	Ref(int& i);
	Ref(int64& i);
	Ref(double& d);
	Ref(bool& b);
	Ref(Date& d);
	Ref(Time& t);
	Ref(Value& v);
	Ref(void *_ptr, RefManager *_m)    { ptr = _ptr, m = _m; }
	Ref()                              { ptr = m = NULL; }
};

template <class T>
T& GetRef(Ref r, T *x = NULL) {
	ASSERT(GetValueTypeNo<T>() == r.GetType());
	return *(T *) r.GetVoidPtr();
}

inline String&  RefString(Ref f)  { return GetRef<String>(f); }
inline WString& RefWString(Ref f) { return GetRef<WString>(f); }
inline int&     RefInt(Ref f)     { return GetRef<int>(f); }
inline int64&   RefInt64(Ref f)   { return GetRef<int64>(f); }
inline double&  RefDouble(Ref f)  { return GetRef<double>(f); }
inline bool&    RefBool(Ref f)    { return GetRef<bool>(f); }
inline Date&    RefDate(Ref f)    { return GetRef<Date>(f); }
inline Time&    RefTime(Ref f)    { return GetRef<Time>(f); }
inline Value&   RefValue(Ref f)   { ASSERT(f.GetType() == VALUE_V);
                                    return *(Value *)f.GetVoidPtr(); }

template <class T>
Ref RawAsRef(T& x) {
	return Ref(&x, &Single< RawRef<T> >());
}

template <class T>
Ref RichAsRef(T& x) {
	return Ref(&x, &Single< RichRef<T> >());
}

#define E__Value(I)   Value p##I
#define E__Ref(I)     Ref p##I

// ---------------------- Value Array

class ValueArray : AssignValueTypeNo<ValueArray, VALUEARRAY_V, Moveable<ValueArray> > {
	struct Data : Value::Void {
		virtual dword      GetType() const             { return VALUEARRAY_V; }
		virtual bool       IsNull() const;
		virtual void       Serialize(Stream& s);
		virtual unsigned   GetHashValue() const;
		virtual bool       IsEqual(const Value::Void *p);
		virtual String     AsString() const;

		int GetRefCount() const     { return AtomicRead(refcount); }

		Vector<Value> data;
	};
	struct NullData : Data {};
	Data *data;

	Vector<Value>& Create();
	Vector<Value>& Clone();
	void  Init0();
	
	friend Value::Void *ValueArrayDataCreate(Stream& s);
	friend class Value;

public:
	ValueArray()                              { Init0(); }
	ValueArray(const ValueArray& v);
	explicit ValueArray(pick_ Vector<Value>& values);
	explicit ValueArray(const Vector<Value>& values, int deep);
	~ValueArray();

	ValueArray& operator=(const ValueArray& v);

	operator Value() const;
	ValueArray(const Value& src);

	ValueArray(const Nuller&)                 { Init0(); }
	bool IsNull() const                       { return data->IsNull(); }

	void Clear();
	void SetCount(int n);
	void SetCount(int n, const Value& v);
	int  GetCount() const                     { return data->data.GetCount(); }
	bool IsEmpty() const                      { return data->data.IsEmpty(); }

	void Add(const Value& v);
	ValueArray& operator<<(const Value& v)    { Add(v); return *this; }
	void Set(int i, const Value& v);
	const Value& Get(int i) const;
	Value GetAndClear(int i);
	const Vector<Value>& Get() const          { return data->data; }

	void Remove(int i, int count = 1);
	void Insert(int i, const ValueArray& va);
	void Append(const ValueArray& va)         { Insert(GetCount(), va); }

	const Value& operator[](int i) const      { return Get(i); }

	unsigned GetHashValue() const             { return data->GetHashValue(); }
	void  Serialize(Stream& s);

	bool operator==(const ValueArray& v) const;
	bool operator!=(const ValueArray& v) const  { return !operator==(v); }
};

template<> inline unsigned GetHashValue(const ValueArray& v) { return v.GetHashValue(); }
template<> inline bool     IsNull(const ValueArray& v)       { return v.IsNull(); }

template<>
String AsString(const ValueArray& v);

class ValueMap : AssignValueTypeNo<ValueMap, VALUEMAP_V, Moveable<ValueMap> >{
	struct Data : Value::Void {
		virtual dword      GetType() const             { return VALUEMAP_V; }
		virtual bool       IsNull() const;
		virtual void       Serialize(Stream& s);
		virtual unsigned   GetHashValue() const;
		virtual bool       IsEqual(const Value::Void *p);
		virtual String     AsString() const;

		int GetRefCount() const     { return AtomicRead(refcount); }

		Index<Value> key;
		ValueArray   value;
	};

	struct NullData : Data {};
	Data *data;

	Data& Create();
	Data& Clone();
	void  Init0();

	friend Value::Void *ValueMapDataCreate(Stream& s);
	friend class Value;

public:
	ValueMap()                               { Init0(); }
	ValueMap(const ValueMap& v);
	ValueMap(pick_ Index<Value>& k, pick_ Vector<Value>& v);
	ValueMap(const Index<Value>& k, const Vector<Value>& v, int deep);
	~ValueMap();

	ValueMap& operator=(const ValueMap& v);

	operator Value() const;
	ValueMap(const Value& src);

	ValueMap(const Nuller&)                  { Init0(); }
	bool IsNullInstance() const              { return data->IsNull(); }

	void Clear();
	int  GetCount() const                     { return data->value.GetCount(); }
	bool IsEmpty() const                      { return data->value.IsEmpty(); }

	void Add(const Value& key, const Value& value);
	void Add(const String& s, const Value& value) { Add(Value(s), value); }
	void Add(const char *s, const Value& value)   { Add(Value(s), value); }
	void Add(Id id, const Value& value)           { Add(Value(id.ToString()), value); }

	void Set(const Value& key, const Value& value);
	void Set(const String& s, const Value& value) { Set(Value(s), value); }
	void Set(const char *s, const Value& value)   { Set(Value(s), value); }
	void Set(Id id, const Value& value)           { Set(Value(id.ToString()), value); }

	void SetAt(int i, const Value& v);
	void SetKey(int i, const Value& k);
	void SetKey(int i, const String& s)           { SetKey(i, Value(s)); }
	void SetKey(int i, const char* s)             { SetKey(i, Value(s)); }
	void SetKey(int i, Id id)                     { SetKey(i, Value(id.ToString())); }
	void Remove(int i);

	const Index<Value>& GetKeys() const           { return data->key; }
	ValueArray GetValues() const                  { return data->value; }

	operator ValueArray() const                   { return GetValues(); }

	const Value& operator[](const Value& k) const;
	const Value& operator[](const String& s) const{ return operator[](Value(s)); }
	const Value& operator[](const char *s) const  { return operator[](Value(s)); }
	const Value& operator[](const Id& k) const    { return operator[](Value(k.ToString())); }
	
	Value GetAndClear(const Value& key);

	unsigned GetHashValue() const                 { return data->GetHashValue(); }
	void     Serialize(Stream& s);
	String   ToString() const                     { return data->AsString(); }

	bool operator==(const ValueMap& v) const;
	bool operator!=(const ValueMap& v) const      { return !operator==(v); }
};

inline bool IsValueArray(const Value& v) { return v.GetType() == VALUEARRAY_V || v.GetType() == VALUEMAP_V; }
inline bool IsValueMap(const Value& v)   { return IsValueArray(v); }

class ValueGen {
public:
	virtual Value  Get() = 0;
	Value operator++()                           { return Get(); }
	virtual ~ValueGen() {}
};


#include "Value.hpp"
