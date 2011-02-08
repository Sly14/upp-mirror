enum _CNULL { CNULL };

template <class D, class S>
inline D brutal_cast(const S& source) { return *(D *)&source; }

#include "Cbgen.h"

template <class OBJECT_, class METHOD_, class T>
struct CallbackMethodActionArgPte : public CallbackAction {
	Ptr<OBJECT_>  object;
	METHOD_       method;
	T             arg;

	void    Execute() { if(object) (object->*method)(arg); }

	CallbackMethodActionArgPte(OBJECT_ *object, METHOD_ method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class Object, class M, class P, class T>
Callback pteback1(Object *object, void (M::*method)(P), T arg) {
	return Callback(new CallbackMethodActionArgPte<Object, void (M::*)(P), T>(object, method, arg));
}

template <class OBJECT_, class METHOD_, class T>
struct CallbackMethodActionArg : public CallbackAction {
	OBJECT_  *object;
	METHOD_   method;
	T         arg;

	void    Execute() { (object->*method)(arg); }

	CallbackMethodActionArg(OBJECT_ *object, METHOD_ method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class Object, class M, class P, class T>
Callback callback1(Object *object, void (M::*method)(P), T arg) {
	return Callback(new CallbackMethodActionArg<Object, void (M::*)(P), T>
	                    (object, method, arg));
}

template <class Object, class M, class P, class T>
Callback callback1(const Object *object, void (M::*method)(P) const, T arg) {
	return Callback(new CallbackMethodActionArg<const Object, void (M::*)(P) const, T>
	                    (object, method, arg));
}

template <class F, class T, class HC = F>
struct CallbackActionCallArg : public CallbackAction {
	F         fn;
	T         arg;
	void    Execute() { fn(arg); }

	CallbackActionCallArg(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class T, class P>
Callback callback1(void (*fn)(P), T arg)
{
	return Callback(new CallbackActionCallArg<void (*)(P), T, uintptr_t>(fn, arg));
}

template <class OBJECT_, class METHOD_, class P1, class T>
struct CallbackMethodActionArg1Pte : public Callback1Action<P1> {
	Ptr<OBJECT_>  object;
	METHOD_       method;
	T             arg;
	void    Execute(P1 p1) { if(object) (object->*method)(p1, arg); }

	CallbackMethodActionArg1Pte(OBJECT_ *object, METHOD_ method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class Object, class M, class P1, class P, class T>
Callback1<P1> pteback1(Object *object, void (M::*method)(P1, P), T arg) {
	return Callback1<P1>(new CallbackMethodActionArg1Pte<Object, void (M::*)(P1, P), P1, T> (object, method, arg));
}

template <class OBJECT_, class METHOD_, class P1, class T>
struct CallbackMethodActionArg1 : public Callback1Action<P1> {
	OBJECT_  *object;
	METHOD_   method;
	T         arg;
	void    Execute(P1 p1) { (object->*method)(p1, arg); }

	CallbackMethodActionArg1(OBJECT_ *object, METHOD_ method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class Object, class M, class P1, class P, class T>
Callback1<P1> callback1(Object *object, void (M::*method)(P1, P), T arg) {
	return Callback1<P1>(new CallbackMethodActionArg1<Object, void (M::*)(P1, P), P1, T>
	                         (object, method, arg));
}

template <class Object, class M, class P1, class P, class T>
Callback1<P1> callback1(const Object *object, void (M::*method)(P1, P) const, T arg) {
	return Callback1<P1>(new CallbackMethodActionArg1<const Object, void (M::*)(P1, P) const, P1, T>
	                         (object, method, arg));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class T1>
struct CallbackMethodActionArg1_2 : public Callback2Action<P1, P2> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	void    Execute(P1 p1, P2 p2) { (object->*method)(p1, p2, arg1);}

	CallbackMethodActionArg1_2(OBJECT_ *object, METHOD_ method, T1 arg1)
	: object(object), method(method), arg1(arg1) {}
};

template <class Object, class M, class P1, class P2, class T1>
Callback2<P1, P2> callback1(Object *object, void (M::*method)(P1, P2, T1), T1 arg1) {
	return Callback2<P1, P2>(new CallbackMethodActionArg1_2<Object, void (M::*)(P1, P2, T1), P1, P2, T1>
	                         (object, method, arg1));
}

template <class Object, class M, class P1, class P2, class T1>
Callback2<P1, P2> callback1(const Object *object, void (M::*method)(P1, P2, T1) const, T1 arg1) {
	return Callback2<P1, P2>(new CallbackMethodActionArg1_2<const Object, void (M::*)(P1, P2, T1) const, P1, P2, T1>
	                         (object, method, arg1));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class P3, class T1>
struct CallbackMethodActionArg1_3 : public Callback3Action<P1, P2, P3> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	void    Execute(P1 p1, P2 p2, P3 p3) { (object->*method)(p1, p2, p3, arg1);}

	CallbackMethodActionArg1_3(OBJECT_ *object, METHOD_ method, T1 arg1)
	: object(object), method(method), arg1(arg1) {}
};

template <class Object, class M, class P1, class P2, class P3, class T1>
Callback3<P1, P2, P3> callback1(Object *object, void (M::*method)(P1, P2, P3, T1), T1 arg1) {
	return Callback3<P1, P2, P3>(new CallbackMethodActionArg1_3<Object, void (M::*)(P1, P2, P3, T1), P1, P2, P3, T1>
	                         (object, method, arg1));
}

template <class Object, class M, class P1, class P2, class P3, class T1>
Callback3<P1, P2, P3> callback1(const Object *object, void (M::*method)(P1, P2, P3, T1) const, T1 arg1) {
	return Callback3<P1, P2, P3>(new CallbackMethodActionArg1_3<const Object, void (M::*)(P1, P2, P3, T1) const, P1, P2, P3, T1>
	                         (object, method, arg1));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class P3, class P4, class T1>
struct CallbackMethodActionArg1_4 : public Callback4Action<P1, P2, P3, P4> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	void    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { (object->*method)(p1, p2, p3, p4, arg1);}

	CallbackMethodActionArg1_4(OBJECT_ *object, METHOD_ method, T1 arg1)
	: object(object), method(method), arg1(arg1) {}
};

template <class Object, class M, class P1, class P2, class P3, class P4, class T1>
Callback4<P1, P2, P3, P4> callback1(Object *object, void (M::*method)(P1, P2, P3, P4, T1), T1 arg1) {
	return Callback4<P1, P2, P3, P4>(new CallbackMethodActionArg1_4<Object, void (M::*)(P1, P2, P3, P4, T1), P1, P2, P3, P4, T1>
	                         (object, method, arg1));
}

template <class Object, class M, class P1, class P2, class P3, class P4, class T1>
Callback4<P1, P2, P3, P4> callback1(const Object *object, void (M::*method)(P1, P2, P3, P4, T1) const, T1 arg1) {
	return Callback4<P1, P2, P3, P4>(new CallbackMethodActionArg1_4<const Object, void (M::*)(P1, P2, P3, P4, T1) const, P1, P2, P3, P4, T1>
	                         (object, method, arg1));
}

template <class OBJECT_, class METHOD_, class P1, class T>
struct GateMethodActionArg1 : public Gate1Action<P1> {
	OBJECT_  *object;
	METHOD_   method;
	T         arg;
	bool      Execute(P1 p1) { return (object->*method)(p1, arg); }

	GateMethodActionArg1(OBJECT_ *object, METHOD_ method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class Object, class M, class P1, class P, class T>
Gate1<P1> callback1(Object *object, bool (M::*method)(P1, P), T arg) {
	return Gate1<P1>(new GateMethodActionArg1<Object, bool (M::*)(P1, P), P1, T>
	                         (object, method, arg));
}

template <class F, class P1, class T, class HC = F>
struct CallbackActionCallArg1 : Callback1Action<P1> {
	F         fn;
	T         arg;
	void    Execute(P1 p1) { fn(p1, arg); }

	CallbackActionCallArg1(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class T, class P1, class P>
Callback1<P1> callback1(void (*fn)(P1, P), T arg)
{
	return Callback1<P1>(new CallbackActionCallArg1<void (*)(P1, P), P1, T, uintptr_t>(fn, arg));
}

template <class T, class P>
Callback callback1(Callback1<P> cb, T arg)
{
	return Callback(new CallbackActionCallArg<Callback1<P>, T>(cb, arg));
}

template <class T, class P1, class P>
Callback1<P1> callback1(Callback2<P1, P> cb, T arg) {
	return Callback1<P1>(new CallbackActionCallArg1<Callback2<P1, P>, P1, T>(cb, arg));
}

template <class P1>
bool Gate1<P1>::Execute(P1 p1) const {
	return (void *)action == (void *)1 ? true : action ? action->Execute(p1) : false;
}

template <class P1, class P2>
bool Gate2<P1, P2>::Execute(P1 p1, P2 p2) const {
	return (void *)action == (void *)1 ? true : action ? action->Execute(p1, p2) : false;
}

template <class P1, class P2, class P3>
bool Gate3<P1, P2, P3>::Execute(P1 p1, P2 p2, P3 p3) const {
	return (void *)action == (void *)1 ? true : action ? action->Execute(p1, p2, p3) : false;
}

template <class P1, class P2, class P3, class P4>
bool Gate4<P1, P2, P3, P4>::Execute(P1 p1, P2 p2, P3 p3, P4 p4) const {
	return (void *)action == (void *)1 ? true : action ? action->Execute(p1, p2, p3, p4) : false;
}

template <class OBJECT_, class METHOD_, class T1, class T2>
struct CallbackMethodActionArg2Pte : public CallbackAction {
	Ptr<OBJECT_>  object;
	METHOD_       method;
	T1            arg1;
	T2            arg2;
	void    Execute() { if(object) (object->*method)(arg1, arg2); }

	CallbackMethodActionArg2Pte(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class Object, class R, class O, class A, class B, class T1, class T2>
Callback pteback2(Object *object, R (O::*method)(A, B), T1 arg1, T2 arg2) {
	return Callback(new CallbackMethodActionArg2Pte<Object, R (O::*)(A, B), T1, T2>
	                    (object, method, arg1, arg2));
}

template <class OBJECT_, class METHOD_, class T1, class T2>
struct CallbackMethodActionArg2 : public CallbackAction {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	void    Execute() { (object->*method)(arg1, arg2); }

	CallbackMethodActionArg2(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class Object, class R, class O, class A, class B, class T1, class T2>
Callback callback2(Object *object, R (O::*method)(A, B), T1 arg1, T2 arg2) {
	return Callback(new CallbackMethodActionArg2<Object, R (O::*)(A, B), T1, T2>
	                    (object, method, arg1, arg2));
}

template <class Object, class R, class O, class A, class B, class T1, class T2>
Callback callback2(const Object *object, R (O::*method)(A, B) const, T1 arg1, T2 arg2) {
	return Callback(new CallbackMethodActionArg2<const Object, R (O::*)(A, B) const, T1, T2>
	                    (object, method, arg1, arg2));
}

template <class OBJECT_, class METHOD_, class P1, class T1, class T2>
struct CallbackMethodActionArg2_1 : public Callback1Action<P1> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1) { (object->*method)(p1, arg1, arg2); }

	CallbackMethodActionArg2_1(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class Object, class M, class P1, class T1, class T2>
Callback1<P1> callback2(Object *object, void (M::*method)(P1, T1, T2), T1 arg1, T2 arg2) {
	return Callback1<P1>(new CallbackMethodActionArg2_1<Object, void (M::*)(P1, T1, T2), P1, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class Object, class M, class P1, class T1, class T2>
Callback1<P1> callback2(const Object *object, void (M::*method)(P1, T1, T2) const, T1 arg1, T2 arg2) {
	return Callback1<P1>(new CallbackMethodActionArg2_1<const Object, void (M::*)(P1, T1, T2) const, P1, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class T1, class T2>
struct CallbackMethodActionArg2_2 : public Callback2Action<P1, P2> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1, P2 p2) { (object->*method)(p1, p2, arg1, arg2);}

	CallbackMethodActionArg2_2(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class Object, class M, class P1, class P2, class T1, class T2>
Callback2<P1, P2> callback2(Object *object, void (M::*method)(P1, P2, T1, T2), T1 arg1, T2 arg2) {
	return Callback2<P1, P2>(new CallbackMethodActionArg2_2<Object, void (M::*)(P1, P2, T1, T2), P1, P2, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class Object, class M, class P1, class P2, class T1, class T2>
Callback2<P1, P2> callback2(const Object *object, void (M::*method)(P1, P2, T1, T2) const, T1 arg1, T2 arg2) {
	return Callback2<P1, P2>(new CallbackMethodActionArg2_2<const Object, void (M::*)(P1, P2, T1, T2) const, P1, P2, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class P3, class T1, class T2>
struct CallbackMethodActionArg2_3 : public Callback3Action<P1, P2, P3> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1, P2 p2, P3 p3) { (object->*method)(p1, p2, p3, arg1, arg2);}

	CallbackMethodActionArg2_3(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class Object, class M, class P1, class P2, class P3, class T1, class T2>
Callback3<P1, P2, P3> callback2(Object *object, void (M::*method)(P1, P2, P3, T1, T2), T1 arg1, T2 arg2) {
	return Callback3<P1, P2, P3>(new CallbackMethodActionArg2_3<Object, void (M::*)(P1, P2, P3, T1, T2), P1, P2, P3, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class Object, class M, class P1, class P2, class P3, class T1, class T2>
Callback3<P1, P2, P3> callback2(const Object *object, void (M::*method)(P1, P2, P3, T1, T2) const, T1 arg1, T2 arg2) {
	return Callback3<P1, P2, P3>(new CallbackMethodActionArg2_3<const Object, void (M::*)(P1, P2, P3, T1, T2) const, P1, P2, P3, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class P3, class P4, class T1, class T2>
struct CallbackMethodActionArg2_4 : public Callback4Action<P1, P2, P3, P4> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { (object->*method)(p1, p2, p3, p4, arg1, arg2);}

	CallbackMethodActionArg2_4(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class Object, class M, class P1, class P2, class P3, class P4, class T1, class T2>
Callback4<P1, P2, P3, P4> callback2(Object *object, void (M::*method)(P1, P2, P3, P4, T1, T2), T1 arg1, T2 arg2) {
	return Callback4<P1, P2, P3, P4>(new CallbackMethodActionArg2_4<Object, void (M::*)(P1, P2, P3, P4, T1, T2), P1, P2, P3, P4, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class Object, class M, class P1, class P2, class P3, class P4, class T1, class T2>
Callback4<P1, P2, P3, P4> callback2(const Object *object, void (M::*method)(P1, P2, P3, P4, T1, T2) const, T1 arg1, T2 arg2) {
	return Callback4<P1, P2, P3, P4>(new CallbackMethodActionArg2_4<const Object, void (M::*)(P1, P2, P3, P4, T1, T2) const, P1, P2, P3, P4, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class X, class T1, class T2, class HC = X>
struct CallbackActionCallArg2 : public CallbackAction {
	X         x;
	T1        arg1;
	T2        arg2;
	void    Execute() { x(arg1, arg2); }

	CallbackActionCallArg2(X x, T1 arg1, T2 arg2) : x(x), arg1(arg1), arg2(arg2) {}
};

template <class R, class A, class B, class T1, class T2>
Callback callback2(R (*fn)(A, B), T1 arg1, T2 arg2) {
	return Callback(new CallbackActionCallArg2<R (*)(A, B), T1, T2, uintptr_t>(fn, arg1, arg2));
}

template <class A, class B, class T1, class T2>
Callback callback2(Callback2<A, B> cb, T1 arg1, T2 arg2) {
	return Callback(new CallbackActionCallArg2<Callback2<A, B>, T1, T2>(cb, arg1, arg2));
}

template <class OBJECT_, class METHOD_, class T1, class T2, class T3>
struct CallbackMethodActionArg3Pte : public CallbackAction {
	Ptr<OBJECT_>  object;
	METHOD_       method;
	T1            arg1;
	T2            arg2;
	T3            arg3;
	void    Execute() { if(object) (object->*method)(arg1, arg2, arg3); }

	CallbackMethodActionArg3Pte(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3) {}
};

template <class Object, class R, class O, class A, class B,class C, class T1, class T2, class T3>
Callback pteback3(Object *object, R (O::*method)(A, B, C), T1 arg1, T2 arg2, T3 arg3) {
	return Callback(new CallbackMethodActionArg3Pte<Object, R (O::*)(A,B,C), T1, T2, T3>
	                    (object, method, arg1, arg2, arg3));
}

template <class OBJECT_, class METHOD_, class T1, class T2, class T3>
struct CallbackMethodActionArg3 : public CallbackAction 
{
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	
	void    Execute() { (object->*method)(arg1, arg2, arg3); }

	CallbackMethodActionArg3(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3) {}
};

template <class Object, class R, class O, class A, class B, class C, class T1, class T2, class T3> 
Callback callback3(Object *object, R (O::*method)(A, B, C), T1 arg1, T2 arg2, T3 arg3) 
{
	return Callback(
		new CallbackMethodActionArg3<Object, R (O::*)(A, B, C), T1, T2, T3>(object, method, arg1, arg2, arg3));
}

template <class Object, class R, class O, class A, class B, class C, class T1, class T2, class T3> 
Callback callback3(const Object *object, R (O::*method)(A, B, C) const, T1 arg1, T2 arg2, T3 arg3) {
	return Callback(new CallbackMethodActionArg3<const Object, R (O::*)(A, B, C) const, T1, T2, T3>
	                    (object, method, arg1, arg2, arg3));
}

template <class OBJECT_, class METHOD_, class P1, class T1, class T2, class T3>
struct CallbackMethodActionArg3_1 : public Callback1Action<P1> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	void    Execute(P1 p1) { (object->*method)(p1, arg1, arg2, arg3); }

	CallbackMethodActionArg3_1(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3) {}
};

template <class Object, class M, class P1, class T1, class T2, class T3>
Callback1<P1> callback3(Object *object, void (M::*method)(P1, T1, T2, T3), T1 arg1, T2 arg2, T3 arg3) {
	return Callback1<P1>(new CallbackMethodActionArg3_1<Object, void (M::*)(P1, T1, T2, T3), P1, T1, T2, T3>
	                         (object, method, arg1, arg2, arg3));
}

template <class Object, class M, class P1, class T1, class T2, class T3>
Callback1<P1> callback3(const Object *object, void (M::*method)(P1, T1, T2, T3) const, T1 arg1, T2 arg2, T3 arg3) {
	return Callback1<P1>(new CallbackMethodActionArg3_1<const Object, void (M::*)(P1, T1, T2, T3) const, P1, T1, T2, T3>
	                         (object, method, arg1, arg2, arg3));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class T1, class T2, class T3>
struct CallbackMethodActionArg3_2 : public Callback2Action<P1, P2> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	void    Execute(P1 p1, P2 p2) { (object->*method)(p1, p2, arg1, arg2, arg3);}

	CallbackMethodActionArg3_2(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3) {}
};

template <class Object, class M, class P1, class P2, class T1, class T2, class T3>
Callback2<P1, P2> callback3(Object *object, void (M::*method)(P1, P2, T1, T2, T3), T1 arg1, T2 arg2, T3 arg3) {
	return Callback2<P1, P2>(new CallbackMethodActionArg3_2<Object, void (M::*)(P1, P2, T1, T2, T3), P1, P2, T1, T2, T3>
	                         (object, method, arg1, arg2, arg3));
}

template <class Object, class M, class P1, class P2, class T1, class T2, class T3>
Callback2<P1, P2> callback3(const Object *object, void (M::*method)(P1, P2, T1, T2, T3) const, T1 arg1, T2 arg2, T3 arg3) {
	return Callback2<P1, P2>(new CallbackMethodActionArg3_2<const Object, void (M::*)(P1, P2, T1, T2, T3) const, P1, P2, T1, T2, T3>
	                         (object, method, arg1, arg2, arg3));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class P3, class T1, class T2, class T3>
struct CallbackMethodActionArg3_3 : public Callback3Action<P1, P2, P3> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	void    Execute(P1 p1, P2 p2, P3 p3) { (object->*method)(p1, p2, p3, arg1, arg2, arg3);}

	CallbackMethodActionArg3_3(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3) {}
};

template <class Object, class M, class P1, class P2, class P3, class T1, class T2, class T3>
Callback3<P1, P2, P3> callback3(Object *object, void (M::*method)(P1, P2, P3, T1, T2, T3), T1 arg1, T2 arg2, T3 arg3) {
	return Callback3<P1, P2, P3>(new CallbackMethodActionArg3_3<Object, void (M::*)(P1, P2, P3, T1, T2, T3), P1, P2, P3, T1, T2, T3>
	                         (object, method, arg1, arg2, arg3));
}

template <class Object, class M, class P1, class P2, class P3, class T1, class T2, class T3>
Callback3<P1, P2, P3> callback3(const Object *object, void (M::*method)(P1, P2, P3, T1, T2, T3) const, T1 arg1, T2 arg2, T3 arg3) {
	return Callback3<P1, P2, P3>(new CallbackMethodActionArg3_3<const Object, void (M::*)(P1, P2, P3, T1, T2, T3) const, P1, P2, P3, T1, T2, T3>
	                         (object, method, arg1, arg2, arg3));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class P3, class P4, class T1, class T2, class T3>
struct CallbackMethodActionArg3_4 : public Callback4Action<P1, P2, P3, P4> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	void    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { (object->*method)(p1, p2, p3, p4, arg1, arg2, arg3);}

	CallbackMethodActionArg3_4(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3) {}
};

template <class Object, class M, class P1, class P2, class P3, class P4, class T1, class T2, class T3>
Callback4<P1, P2, P3, P4> callback3(Object *object, void (M::*method)(P1, P2, P3, P4, T1, T2, T3), T1 arg1, T2 arg2, T3 arg3) {
	return Callback4<P1, P2, P3, P4>(new CallbackMethodActionArg3_4<Object, void (M::*)(P1, P2, P3, P4, T1, T2, T3), P1, P2, P3, P4, T1, T2, T3>
	                         (object, method, arg1, arg2, arg3));
}

template <class Object, class M, class P1, class P2, class P3, class P4, class T1, class T2, class T3>
Callback4<P1, P2, P3, P4> callback3(const Object *object, void (M::*method)(P1, P2, P3, P4, T1, T2, T3) const, T1 arg1, T2 arg2, T3 arg3) {
	return Callback4<P1, P2, P3, P4>(new CallbackMethodActionArg3_4<const Object, void (M::*)(P1, P2, P3, P4, T1, T2, T3) const, P1, P2, P3, P4, T1, T2, T3>
	                         (object, method, arg1, arg2, arg3));
}

template <class X, class T1, class T2, class T3, class HC = X>
struct CallbackActionCallArg3 : public CallbackAction {
	X         x;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	void    Execute() { x(arg1, arg2, arg3); }

	CallbackActionCallArg3(X x, T1 arg1, T2 arg2, T3 arg3) 
		: x(x), arg1(arg1), arg2(arg2), arg3(arg3) {}
};

template <class R, class A, class B, class C, class T1, class T2, class T3>
Callback callback3(R (*fn)(A, B, C), T1 arg1, T2 arg2, T3 arg3) {
	return Callback(
		new CallbackActionCallArg3<R (*)(A, B, C), T1, T2, T3, uintptr_t>(fn, arg1, arg2, arg3));
}

template <class A, class B, class C, class T1, class T2, class T3>
Callback callback3(Callback3<A, B, C> cb, T1 arg1, T2 arg2, T3 arg3) {
	return Callback(
		new CallbackActionCallArg3<Callback3<A, B, C>, T1, T2, T3>(cb, arg1, arg2, arg3));
}

template <class OBJECT_, class METHOD_, class T1, class T2, class T3, class T4>
struct CallbackMethodActionArg4Pte : public CallbackAction {
	Ptr<OBJECT_>  object;
	METHOD_       method;
	T1            arg1;
	T2            arg2;
	T3            arg3;
	T4            arg4;
	void    Execute() { if(object) (object->*method)(arg1, arg2, arg3, arg4); }

	CallbackMethodActionArg4Pte(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4) {}
};

template <class Object, class R, class O, class A, class B,class C, class D, class T1, class T2, class T3, class T4>
Callback pteback4(Object *object, R (O::*method)(A, B,C,D), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback(new CallbackMethodActionArg4Pte<Object, R (O::*)(A, B,C,D), T1, T2, T3, T4>
	                    (object, method, arg1, arg2, arg3, arg4));
}

template <class OBJECT_, class METHOD_, class T1, class T2, class T3, class T4>
struct CallbackMethodActionArg4 : public CallbackAction 
{
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	
	void    Execute() { (object->*method)(arg1, arg2, arg3, arg4); }

	CallbackMethodActionArg4(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4) {}
};

template <class Object, class R, class O, class A, class B, class C, class D, class T1, class T2, class T3, class T4> 
Callback callback4(Object *object, R (O::*method)(A, B, C, D), T1 arg1, T2 arg2, T3 arg3, T4 arg4) 
{
	return Callback(
		new CallbackMethodActionArg4<Object, R (O::*)(A, B, C, D), T1, T2, T3, T4>(object, method, arg1, arg2, arg3, arg4));
}

template <class Object, class R, class O, class A, class B, class C, class D, class T1, class T2, class T3, class T4> 
Callback callback4(const Object *object, R (O::*method)(A, B, C, D) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback(new CallbackMethodActionArg4<const Object, R (O::*)(A, B,C,D) const, T1, T2, T3, T4>
	                    (object, method, arg1, arg2, arg3, arg4));
}

template <class OBJECT_, class METHOD_, class P1, class T1, class T2, class T3, class T4>
struct CallbackMethodActionArg4_1 : public Callback1Action<P1> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	void    Execute(P1 p1) { (object->*method)(p1, arg1, arg2, arg3, arg4); }

	CallbackMethodActionArg4_1(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4) {}
};

template <class Object, class M, class P1, class T1, class T2, class T3, class T4>
Callback1<P1> callback4(Object *object, void (M::*method)(P1, T1, T2, T3, T4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback1<P1>(new CallbackMethodActionArg4_1<Object, void (M::*)(P1, T1, T2, T3, T4), P1, T1, T2, T3, T4>
	                         (object, method, arg1, arg2, arg3, arg4));
}

template <class Object, class M, class P1, class T1, class T2, class T3, class T4>
Callback1<P1> callback4(const Object *object, void (M::*method)(P1, T1, T2, T3, T4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback1<P1>(new CallbackMethodActionArg4_1<const Object, void (M::*)(P1, T1, T2, T3, T4) const, P1, T1, T2, T3, T4>
	                         (object, method, arg1, arg2, arg3, arg4));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class T1, class T2, class T3, class T4>
struct CallbackMethodActionArg4_2 : public Callback2Action<P1, P2> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	void    Execute(P1 p1, P2 p2) { (object->*method)(p1, p2, arg1, arg2, arg3, arg4);}

	CallbackMethodActionArg4_2(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4) {}
};

template <class Object, class M, class P1, class P2, class T1, class T2, class T3, class T4>
Callback2<P1, P2> callback4(Object *object, void (M::*method)(P1, P2, T1, T2, T3, T4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback2<P1, P2>(new CallbackMethodActionArg4_2<Object, void (M::*)(P1, P2, T1, T2, T3, T4), P1, P2, T1, T2, T3, T4>
	                         (object, method, arg1, arg2, arg3, arg4));
}

template <class Object, class M, class P1, class P2, class T1, class T2, class T3, class T4>
Callback2<P1, P2> callback4(const Object *object, void (M::*method)(P1, P2, T1, T2, T3, T4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback2<P1, P2>(new CallbackMethodActionArg4_2<const Object, void (M::*)(P1, P2, T1, T2, T3, T4) const, P1, P2, T1, T2, T3, T4>
	                         (object, method, arg1, arg2, arg3, arg4));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class P3, class T1, class T2, class T3, class T4>
struct CallbackMethodActionArg4_3 : public Callback3Action<P1, P2, P3> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T3        arg4;
	void    Execute(P1 p1, P2 p2, P3 p3) { (object->*method)(p1, p2, p3, arg1, arg2, arg3, arg4);}

	CallbackMethodActionArg4_3(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4) {}
};

template <class Object, class M, class P1, class P2, class P3, class T1, class T2, class T3, class T4>
Callback3<P1, P2, P3> callback4(Object *object, void (M::*method)(P1, P2, P3, T1, T2, T3, T4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback3<P1, P2, P3>(new CallbackMethodActionArg4_3<Object, void (M::*)(P1, P2, P3, T1, T2, T3, T4), P1, P2, P3, T1, T2, T3, T4>
	                         (object, method, arg1, arg2, arg3, arg4));
}

template <class Object, class M, class P1, class P2, class P3, class T1, class T2, class T3, class T4>
Callback3<P1, P2, P3> callback4(const Object *object, void (M::*method)(P1, P2, P3, T1, T2, T3, T4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback3<P1, P2, P3>(new CallbackMethodActionArg4_3<const Object, void (M::*)(P1, P2, P3, T1, T2, T3, T4) const, P1, P2, P3, T1, T2, T3, T4>
	                         (object, method, arg1, arg2, arg3, arg4));
}

template <class OBJECT_, class METHOD_, class P1, class P2, class P3, class P4, class T1, class T2, class T3, class T4>
struct CallbackMethodActionArg4_4 : public Callback4Action<P1, P2, P3, P4> {
	OBJECT_  *object;
	METHOD_   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	void    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { (object->*method)(p1, p2, p3, p4, arg1, arg2, arg3, arg4);}

	CallbackMethodActionArg4_4(OBJECT_ *object, METHOD_ method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4) {}
};

template <class Object, class M, class P1, class P2, class P3, class P4, class T1, class T2, class T3, class T4>
Callback4<P1, P2, P3, P4> callback4(Object *object, void (M::*method)(P1, P2, P3, P4, T1, T2, T3, T4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback4<P1, P2, P3, P4>(new CallbackMethodActionArg4_4<Object, void (M::*)(P1, P2, P3, P4, T1, T2, T3, T4), P1, P2, P3, P4, T1, T2, T3, T4>
	                         (object, method, arg1, arg2, arg3, arg4));
}

template <class Object, class M, class P1, class P2, class P3, class P4, class T1, class T2, class T3, class T4>
Callback4<P1, P2, P3, P4> callback4(const Object *object, void (M::*method)(P1, P2, P3, P4, T1, T2, T3, T4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback4<P1, P2, P3, P4>(new CallbackMethodActionArg4_4<const Object, void (M::*)(P1, P2, P3, P4, T1, T2, T3, T4) const, P1, P2, P3, P4, T1, T2, T3, T4>
	                         (object, method, arg1, arg2, arg3, arg4));
}

template <class X, class T1, class T2, class T3, class T4, class HC = X>
struct CallbackActionCallArg4 : public CallbackAction {
	X         x;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	void    Execute() { x(arg1, arg2, arg3, arg4); }

	CallbackActionCallArg4(X x, T1 arg1, T2 arg2, T3 arg3, T4 arg4) 
		: x(x), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4) {}
};

template <class R, class A, class B,class C, class D, class T1, class T2, class T3, class T4>
Callback callback4(R (*fn)(A, B, C, D), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback(
		new CallbackActionCallArg4<R (*)(A, B,C,D), T1, T2, T3, T4, uintptr_t>(fn, arg1, arg2, arg3, arg4));
}

template <class A, class B, class C, class D, class T1, class T2, class T3, class T4>
Callback callback4(Callback4<A, B, C, D> cb, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback(
		new CallbackActionCallArg4<Callback4<A, B,C,D>, T1, T2, T3, T4>(cb, arg1, arg2, arg3, arg4));
}

#define THISBACK(x)               callback(this, &CLASSNAME::x)
#define THISBACK1(x, arg)         callback1(this, &CLASSNAME::x, arg)
#define THISBACK2(m, a, b)        callback2(this, &CLASSNAME::m, a, b)
#define THISBACK3(m, a, b, c)     callback3(this, &CLASSNAME::m, a, b, c)
#define THISBACK4(m, a, b, c, d)  callback4(this, &CLASSNAME::m, a, b, c, d)

#define PTEBACK(x)                pteback(this, &CLASSNAME::x)
#define PTEBACK1(x, arg)          pteback1(this, &CLASSNAME::x, arg)
#define PTEBACK2(m, a, b)         pteback2(this, &CLASSNAME::m, a, b)
#define PTEBACK3(m, a, b, c)      pteback3(this, &CLASSNAME::m, a, b, c)
#define PTEBACK4(m, a, b, c, d)   pteback4(this, &CLASSNAME::m, a, b, c, d)

template <class T>
class CallbackArgTarget
{
	T result;

	void SetResult(const T& value)      { result = value; }
	void Set(T value)                   { result = value; }

public:
	typedef CallbackArgTarget CLASSNAME;

	operator const T&() const           { return result; }
	bool IsNullInstance() const         { return IsNull(result); }

	Callback operator[](const T& value) { return THISBACK1(SetResult, value); }
	operator Callback1<const T&>()      { return THISBACK(SetResult); }
	operator Callback1<T>()             { return THISBACK(Set); }

	CallbackArgTarget()                 { result = Null; }
};
