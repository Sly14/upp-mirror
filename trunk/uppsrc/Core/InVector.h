template <class K, class T, class Less, class Data>
class SortedAMap;

template <class T> struct Slaved_InVector__;
template <class T> struct Slaved_InArray__;

struct InVectorSlave__ {
	virtual void Clear() = 0;
	virtual void Count(int n) = 0;
	virtual void Split(int blki, int nextsize) = 0;
	virtual void AddFirst() = 0;
	virtual void Insert(int blki, int pos) = 0;
	virtual void Join(int blki) = 0;
	virtual void Remove(int blki, int pos, int n) = 0;
	virtual void RemoveBlk(int blki, int n) = 0;
	virtual void Index(int blki, int n) = 0;
	virtual void Reindex() = 0;
};

template <class T>
class InVector : public MoveableAndDeepCopyOption< InVector<T> > {
public:
	class ConstIterator;
	class Iterator;

	template <class K, class TT, class Less, class Data> friend class SortedAMap;
	template <class TT> friend struct Slaved_InVector__;
	template <class TT> friend struct Slaved_InArray__;

private:
	Vector< Vector<T> > data;
	Vector< Vector<int> > index;

	int    count;
	int    hcount;
	int64  serial;
	int    blk_high;
	int    blk_low;

	InVectorSlave__ *slave;

	void SetCache(int blki, int offset) const;
	void ClearCache() const;
	int  FindBlock0(int& pos, int& off) const;
	int  FindBlock(int& pos, int& off) const;
	int  FindBlock(int& pos) const;
	void SetBlkPar();

	template <class L>
	int  FindUpperBound(const T& val, const L& less, int& off, int& pos) const;

	template <class L>
	int  FindLowerBound(const T& val, const L& less, int& off, int& pos) const;

	void Reindex();
	void Index(int q, int n);
	void Count(int n)                               { count += n; }
	void Join(int blki);

	bool JoinSmall(int blki);
	T   *Insert0(int ii, int blki, int pos, int off, const T *val);
	T   *Insert0(int ii, const T *val);
	
	void Reset();

	void SetIter(ConstIterator& it, int ii) const;
	void SetBegin(ConstIterator& it) const;
	void SetEnd(ConstIterator& it) const;

	void     Chk() const                            { ASSERT_(!IsPicked(), "Broken pick semantics"); }

#ifdef flagIVTEST
	void Check(int blki, int offset) const;
#endif

public:
	T&       Insert(int i)                          { return *Insert0(i, NULL); }
	T&       Insert(int i, const T& x)              { return *Insert0(i, &x); }
	void     InsertN(int i, int count);
	void     Remove(int i, int count = 1);

	const T& operator[](int i) const;
	T&       operator[](int i);

	T&       Add()                                  { return Insert(GetCount()); }
	T&       Add(const T& x)                        { return Insert(GetCount(), x); }
	void     AddN(int n)                            { InsertN(GetCount(), n); }
	
	int      GetCount() const                       { Chk(); return count; }
	bool     IsEmpty() const                        { return GetCount() == 0; }

	void     Trim(int n)                            { Remove(n, GetCount() - n); }
	void     SetCount(int n);
	void     Clear();

	T&       At(int i)                              { if(i >= GetCount()) SetCount(i + 1); return (*this)[i]; }

	void     Shrink();

	void     Set(int i, const T& x, int count);
	T&       Set(int i, const T& x)                 { Set(i, x, 1); return (*this)(i); }

	void     Swap(int i1, int i2)                   { UPP::Swap((*this)[i1], (*this)[i2]); }

	void     Drop(int n = 1)                        { ASSERT(n <= GetCount()); Trim(GetCount() - n); }
	T&       Top()                                  { ASSERT(GetCount()); return (*this)[GetCount() - 1]; }
	const T& Top() const                            { ASSERT(GetCount()); return (*this)[GetCount() - 1]; }
	T        Pop()                                  { T h = Top(); Drop(); return h; }

	template <class L>
	int FindUpperBound(const T& val, const L& less) const { int off, pos; FindUpperBound(val, less, off, pos); return off + pos; }
	int FindUpperBound(const T& val) const                { return FindUpperBound(val, StdLess<T>()); }

	template <class L>
	int FindLowerBound(const T& val, const L& less) const { int off, pos; FindLowerBound(val, less, off, pos); return off + pos; }
	int FindLowerBound(const T& val) const                { return FindLowerBound(val, StdLess<T>()); }

	template <class L>
	int InsertUpperBound(const T& val, const L& less);
	int InsertUpperBound(const T& val)                    { return InsertUpperBound(val, StdLess<T>()); }
	
	template <class L>
	int Find(const T& val, const L& less) const;
	int Find(const T& val) const                          { return Find(val, StdLess<T>()); }

	typedef T        ValueType;

	ConstIterator    Begin() const                  { ConstIterator it; SetBegin(it); return it; }
	ConstIterator    End() const                    { ConstIterator it; SetEnd(it); return it; }
	ConstIterator    GetIter(int pos) const         { ConstIterator it; SetIter(it, pos); return it; }

	Iterator         Begin()                        { Iterator it; SetBegin(it); return it; }
	Iterator         End()                          { Iterator it; SetEnd(it); return it; }
	Iterator         GetIter(int pos)               { Iterator it; SetIter(it, pos); return it; }

	InVector();
	bool IsPicked() const                           { return data.IsPicked(); }

	InVector(const InVector& v, int);

	void Swap(InVector& b);

	friend void Swap(InVector& a, InVector& b)      { a.Swap(b); }
	
	STL_VECTOR_COMPATIBILITY(InVector<T>)

#ifdef _DEBUG
	void DumpIndex();
#endif
};

template <class T>
class InVector<T>::ConstIterator {
	const T        *ptr;
	const T        *begin;
	const T        *end;
	const InVector *v;
	int             offset;
	int             blki;

	friend class InVector<T>;
	friend class InVector<T>::Iterator;

	void NextBlk();
	void PrevBlk();		

public:
	force_inline int GetIndex() const              { return ptr - begin + offset; }

	force_inline ConstIterator& operator++()       { ASSERT(ptr); if(++ptr == end) NextBlk(); return *this; }
	force_inline ConstIterator& operator--()       { ASSERT(ptr); if(ptr == begin) PrevBlk(); --ptr; return *this; }
	force_inline ConstIterator  operator++(int)    { ConstIterator t = *this; ++*this; return t; }
	force_inline ConstIterator  operator--(int)    { ConstIterator t = *this; --*this; return t; }

	force_inline ConstIterator& operator+=(int d);
	ConstIterator& operator-=(int d)               { return operator+=(-d); }

	ConstIterator operator+(int d) const           { ConstIterator t = *this; t += d; return t; }
	ConstIterator operator-(int d) const           { return operator+(-d); }

	int  operator-(const ConstIterator& x) const   { return GetIndex() - x.GetIndex(); }

	bool operator==(const ConstIterator& b) const  { return ptr == b.ptr; }
	bool operator!=(const ConstIterator& b) const  { return ptr != b.ptr; }
	bool operator<(const ConstIterator& b) const   { return blki == b.blki ? ptr < b.ptr : blki < b.blki; }
	bool operator>(const ConstIterator& b) const   { return blki == b.blki ? ptr > b.ptr : blki > b.blki; }
	bool operator<=(const ConstIterator& b) const  { return blki == b.blki ? ptr <= b.ptr : blki <= b.blki; }
	bool operator>=(const ConstIterator& b) const  { return blki == b.blki ? ptr >= b.ptr : blki >= b.blki; }

	operator bool() const                          { return ptr; }

	const T& operator*() const                     { return *ptr; }
	const T *operator->() const                    { return ptr; }
	const T& operator[](int i) const               { ConstIterator h = *this; h += i; return *h; }
};

template <class T>
class InVector<T>::Iterator : public InVector<T>::ConstIterator {
	typedef ConstIterator B;
public:
	Iterator& operator++()                         { ConstIterator::operator++(); return *this; }
	Iterator& operator--()                         { ConstIterator::operator--(); return *this; }
	Iterator  operator++(int)                      { Iterator t = *this; ++*this; return t; }
	Iterator  operator--(int)                      { Iterator t = *this; --*this; return t; }

	Iterator& operator+=(int d)                    { ConstIterator::operator+=(d); return *this; }
	Iterator& operator-=(int d)                    { return operator+=(-d); }

	Iterator operator+(int d) const                { Iterator t = *this; t += d; return t; }
	Iterator operator-(int d) const                { return operator+(-d); }

	int  operator-(const Iterator& x) const        { return B::GetIndex() - x.GetIndex(); }

	T& operator*()                                 { return *(T*)B::ptr; }
	T *operator->()                                { return (T*)B::ptr; }
	T& operator[](int i)                           { Iterator h = *this; h += i; return *h; }

	const T& operator*() const                     { return *B::ptr; }
	const T *operator->() const                    { return B::ptr; }
	const T& operator[](int i) const               { ConstIterator h = *this; h += i; return *h; }
};

template <class T>
class InArray : public MoveableAndDeepCopyOption< InVector<T> > {
	template <class K, class TT, class Less, class Data> friend class SortedAMap;
	template <class TT> friend struct Slaved_InArray__;

public:
	class ConstIterator;
	class Iterator;

private:
#ifdef _DEBUG
	typedef T *PointerType;
#else
	typedef void *PointerType;
#endif
	typedef InVector<PointerType> IV;
	typedef typename InVector<PointerType>::Iterator IVIter;
	typedef typename InVector<PointerType>::Iterator IVConstIter;

	IV iv;

	T&       Get(int i) const       { return *(T *)iv[i]; }
	void     Delete(IVIter i, int count);
	void     Delete(int i, int count);
	void     Init(int i, int count);
	void     Free()                 { if(!IsPicked()) Delete(iv.Begin(), GetCount()); }

	void     SetIter(ConstIterator& it, int ii);
	void     SetBegin(ConstIterator& it);
	void     SetEnd(ConstIterator& it);

	template <class L>
	struct ALess {
		const L& less;
		
		bool operator()(const PointerType& a, const PointerType& b) const {
			return less(*(T*)a, *(T*)b);
		}
		
		ALess(const L& less) : less(less) {}
	};

	template <class K, class V, class L> friend class SortedVectorMap;

public:
	T&       Insert(int i, T *newt)                 { iv.Insert(i, newt); return *newt; }
	T&       Insert(int i)                          { return Insert(i, new T); }
	T&       Insert(int i, const T& x)              { return Insert(i, new T(x)); }
	template<class TT> TT& InsertCreate(int i)      { TT *q = new TT; Insert(i, q); return *q; }

	void     InsertN(int i, int count);
	void     Remove(int i, int count = 1);

	const T& operator[](int i) const                { return Get(i); }
	T&       operator[](int i)                      { return Get(i); }

	T&       Add()                                  { return Insert(GetCount()); }
	T&       Add(const T& x)                        { return Insert(GetCount(), x); }
	void     AddN(int n)                            { InsertN(GetCount(), n); }
	T&       Add(T *newt)                           { Insert(GetCount(), newt); }
	template<class TT> TT& Create()                 { TT *q = new TT; Add(q); return *q; }
	
	int      GetCount() const                       { return iv.GetCount(); }
	bool     IsEmpty() const                        { return GetCount() == 0; }

	void     Trim(int n)                            { Delete(n, GetCount() - n); }

	void     SetCount(int n);
	void     Clear();

	T&       At(int i)                              { if(i >= GetCount()) SetCount(i + 1); return (*this)[i]; }

	void     Shrink()                               { iv.Shrink(); }

	void     Set(int i, const T& x, int count);
	T&       Set(int i, const T& x)                 { Set(i, x, 1); return Get(i); }

	void     Swap(int i1, int i2)                   { UPP::Swap((*this)[i1], (*this)[i2]); }

	void     Drop(int n = 1)                        { Trim(GetCount() - n); }
	T&       Top()                                  { return (*this)[GetCount() - 1]; }
	const T& Top() const                            { return (*this)[GetCount() - 1]; }
	T        Pop()                                  { T h = Top(); Drop(); return h; }

	template <class L>
	int FindUpperBound(const T& val, const L& less) const  { return iv.FindUpperBound((T*)&val, ALess<L>(less)); }
	int FindUpperBound(const T& val) const                 { return FindUpperBound(val, StdLess<T>()); }

	template <class L>
	int FindLowerBound(const T& val, const L& less) const  { return iv.FindLowerBound((T*)&val, ALess<L>(less)); }
	int FindLowerBound(const T& val) const                 { return FindLowerBound(val, StdLess<T>()); }

	template <class L>
	int InsertUpperBound(const T& val, const L& lss)       { return iv.InsertUpperBound(new T(val), ALess<L>(lss)); }
	int InsertUpperBound(const T& val)                     { return InsertUpperBound(val, StdLess<T>()); }
	
	template <class L>
	int Find(const T& val, const L& less) const            { return iv.Find((T*)&val, ALess<L>(less)); }
	int Find(const T& val) const                           { return Find(val, StdLess<T>()); }

	typedef T        ValueType;

	ConstIterator    Begin() const                  { ConstIterator it; SetBegin(it); return it; }
	ConstIterator    End() const                    { ConstIterator it; SetEnd(it); return it; }
	ConstIterator    GetIter(int pos) const         { ConstIterator it; SetIter(it, pos); return it; }

	Iterator         Begin()                        { Iterator it; SetBegin(it); return it; }
	Iterator         End()                          { Iterator it; SetEnd(it); return it; }
	Iterator         GetIter(int pos)               { Iterator it; SetIter(it, pos); return it; }

	bool IsPicked() const                           { return iv.IsPicked(); }

	InArray() {}
	InArray(const InArray& v, int);
	~InArray()                                      { Free(); }
	
	void Swap(InArray& b)                           { iv.Swap(b.iv); }
	
	friend void Swap(InArray& a, InArray& b)        { a.Swap(b); }
	
	STL_VECTOR_COMPATIBILITY(InArray<T>)

#ifdef _DEBUG
	void DumpIndex();
#endif
};

template <class T>
class InArray<T>::ConstIterator {
	IVConstIter it;

	friend class InArray<T>;
	friend class InArray<T>::Iterator;

public:
	force_inline int GetIndex() const              { return it.GetIndex(); }

	force_inline ConstIterator& operator++()       { ++it; return *this; }
	force_inline ConstIterator& operator--()       { --it; return *this; }
	force_inline ConstIterator  operator++(int)    { ConstIterator t = *this; ++*this; return t; }
	force_inline ConstIterator  operator--(int)    { ConstIterator t = *this; --*this; return t; }

	force_inline ConstIterator& operator+=(int d)  { it += d; return *this; }
	ConstIterator& operator-=(int d)               { return operator+=(-d); }

	ConstIterator operator+(int d) const           { ConstIterator t = *this; t += d; return t; }
	ConstIterator operator-(int d) const           { return operator+(-d); }

	int  operator-(const ConstIterator& x) const   { return GetIndex() - x.GetIndex(); }

	bool operator==(const ConstIterator& b) const  { return it == b.it; }
	bool operator!=(const ConstIterator& b) const  { return it != b.it; }
	bool operator<(const ConstIterator& b) const   { return it < b.it; }
	bool operator>(const ConstIterator& b) const   { return it > b.it; }
	bool operator<=(const ConstIterator& b) const  { return it <= b.it; }
	bool operator>=(const ConstIterator& b) const  { return it >= b.it; }

	operator bool() const                          { return it; }

	const T& operator*() const                     { return *(T *)*it; }
	const T *operator->() const                    { return (T *)*it; }
	const T& operator[](int i) const               { ConstIterator h = *this; h += i; return *h; }
};

template <class T>
class InArray<T>::Iterator : public InArray<T>::ConstIterator {
	typedef ConstIterator B;

public:
	Iterator& operator++()                         { ConstIterator::operator++(); return *this; }
	Iterator& operator--()                         { ConstIterator::operator--(); return *this; }
	Iterator  operator++(int)                      { Iterator t = *this; ++*this; return t; }
	Iterator  operator--(int)                      { Iterator t = *this; --*this; return t; }

	Iterator& operator+=(int d)                    { ConstIterator::operator+=(d); return *this; }
	Iterator& operator-=(int d)                    { return operator+=(-d); }

	Iterator operator+(int d) const                { Iterator t = *this; t += d; return t; }
	Iterator operator-(int d) const                { return operator+(-d); }

	int  operator-(const Iterator& x) const        { return B::GetIndex() - x.GetIndex(); }

	T& operator*()                                 { return *(T *)*B::it; }
	T *operator->()                                { return (T *)*B::it; }
	T& operator[](int i)                           { Iterator h = *this; h += i; return *h; }

	const T& operator*() const                     { return *(T *)*B::it; }
	const T *operator->() const                    { return (T *)*B::it; }
	const T& operator[](int i) const               { Iterator h = *this; h += i; return *h; }
};

template <class T, class Less = StdLess<T> >
class SortedIndex : MoveableAndDeepCopyOption< SortedIndex<T, Less> > {
	InVector<T> iv;
	
	template <class K, class TT, class Lss, class Data> friend class SortedAMap;

public:
	int           Add(const T& x)                  { return iv.InsertUpperBound(x, Less()); }
	int           FindAdd(const T& key);
	SortedIndex&  operator<<(const T& x)           { Add(x); return *this; }
	
	int           FindLowerBound(const T& x) const { return iv.FindLowerBound(x, Less()); }
	int           FindUpperBound(const T& x) const { return iv.FindUpperBound(x, Less()); }

	int           Find(const T& x) const           { return iv.Find(x, Less()); }
	int           FindNext(int i) const;
	int           FindLast(const T& x) const;
	int           FindPrev(int i) const;

	void          Remove(int i)                    { iv.Remove(i); }
	void          Remove(int i, int count)         { iv.Remove(i, count); }
	int           RemoveKey(const T& x);

	const T&      operator[](int i) const          { return iv[i]; }
	int           GetCount() const                 { return iv.GetCount(); }
	bool          IsEmpty() const                  { return iv.IsEmpty(); }
	void          Clear()                          { iv.Clear(); }

	void          Trim(int n)                      { return iv.Trim(n); }
	void          Drop(int n = 1)                  { iv.Drop(n); }
	const T&      Top() const                      { return iv.Top(); }

	void          Shrink()                         { iv.Shrink(); }
	
	typedef typename InVector<T>::ConstIterator ConstIterator;
	
	typedef T        ValueType;

	ConstIterator    Begin() const                  { return iv.Begin(); }
	ConstIterator    End() const                    { return iv.End(); }
	ConstIterator    GetIter(int pos) const         { return iv.GetIter(pos); }

	SortedIndex()                                        {}
	SortedIndex(const SortedIndex& s, int) : iv(s.iv, 1) {}

	void Swap(SortedIndex& a)                       { iv.Swap(a.iv); }

	friend void Swap(SortedIndex& a, SortedIndex& b){ a.Swap(b); }

	STL_SINDEX_COMPATIBILITY(SortedIndex<T _cm_ Less>)
};

#define LLOG(x)   // DLOG(x)
#include "InVector.hpp"
#undef  LLOG
