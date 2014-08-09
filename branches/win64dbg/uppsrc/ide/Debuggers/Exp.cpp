#include "Debuggers.h"

#ifdef COMPILER_MSC

#define LLOG(x)  // DLOG(x)

#ifdef _DEBUG
String Pdb::Val::ToString() const
{
	String r;
	r << "Pdb::Val type: " << type << ", ref: " << ref << ", array: " << array
	  << ", rvalue: " << rvalue;
	return r;
}
#endif

void Pdb::ThrowError(const char *s)
{
	throw CParser::Error(s);
}

int Pdb::SizeOfType(int ti)
{
	if(ti >= 0)
		return GetType(ti).size;
	switch(ti) {
	case BOOL1:
	case SINT1:
	case UINT1:
		return 1;
	case SINT2:
	case UINT2:
		return 2;
	case SINT4:
	case UINT4:
	case FLT:
		return 4;
	case SINT8:
	case UINT8:
	case DBL:
		return 8;
	}
	return 0;
}

#define READINT(q, type) \
case q: { \
	type x; \
	if(!Copy(v.address, &x, sizeof(x))) \
		ThrowError("??"); \
	v.ival = x; \
	break; \
} \

#define READFLT(q, type) \
case q: { \
	type x; \
	if(!Copy(v.address, &x, sizeof(x))) \
		ThrowError("??"); \
	v.fval = x; \
	break; \
} \

Pdb::Val Pdb::GetRVal(Pdb::Val v)
{
	DDUMP(Hex(v.address));
	if(v.rvalue)
		return v;
	v.rvalue = true;
	if(v.array) {
		v.array = false;
		v.ref++;
	}
	else
	if(v.ref) {
		if(!Copy(v.address, &v.address, 4))
			ThrowError("??");
	}
	else
	if(v.bitcnt) {
		dword w;
		if(!Copy(v.address, &w, 4))
			ThrowError("??");
		w = (w >> v.bitpos) & (0xffffffff >> (32 - v.bitcnt));
		if(v.type == SINT1 || v.type == SINT2 || v.type == SINT4 || v.type == SINT8) {
			if(w & (1 << (v.bitcnt - 1)))
				w |= (0xffffffff << v.bitcnt);
			v.ival = (int32)w;
		}
		else
			v.ival = w;
	}
	else
		switch(v.type) {
		READINT(BOOL1, bool)
		READINT(UINT1, byte);
		READINT(SINT1, int8);
		READINT(UINT2, uint16);
		READINT(SINT2, int16);
		READINT(UINT4, uint32);
		READINT(SINT4, int32);
		READINT(UINT8, uint64);
		READINT(SINT8, int64);
		READFLT(FLT, float);
		READFLT(DBL, double);
		READINT(PFUNC, uint32);
		case UNKNOWN:
			break;
		default:
			ThrowError("Invalid operand");
		}
	return v;
}

#define GETINT(q, type) case q: return (type)v.ival;

int64 Pdb::GetInt(Pdb::Val v)
{
	v = GetRVal(v);
	switch(v.type) {
	GETINT(BOOL1, bool)
	GETINT(UINT1, byte);
	GETINT(SINT1, int8);
	GETINT(UINT2, uint16);
	GETINT(SINT2, int16);
	GETINT(UINT4, uint32);
	GETINT(SINT4, int32);
	GETINT(UINT8, uint64);
	GETINT(SINT8, int64);
	case FLT:
	case DBL:
		if(v.fval >= _I64_MIN && v.fval <= _I64_MAX)
			return (int64)v.fval;
	}
	ThrowError("Invalid operand");
	return 0;
}

#define GETINT(q, type) case q: return (type)v.ival;

double Pdb::GetFlt(Pdb::Val v)
{
	v = GetRVal(v);
	if(v.type == DBL || v.type == FLT)
		return v.fval;
	return (double)GetInt(v);
}

Pdb::Val Pdb::Compute(Pdb::Val v1, Pdb::Val v2, int oper)
{
	LLOG("Compute " << char(oper));
	LLOG("v1: " << v1);
	LLOG("v2: " << v2);
	if(v1.ref || v1.array) {
		int q = (int)GetInt(v2) * (v1.ref > 1 ? 4 : SizeOfType(v1.type));
		v1 = GetRVal(v1);
		switch(oper) {
		case '+': v1.address += q; break;
		case '-': v1.address -= q; break;
		default: ThrowError("Invalid pointer arithmetics");
		}
		return v1;
	}
	if(v2.ref || v2.array) {
		int q = (int)GetInt(v1) * (v2.ref ? 4 : SizeOfType(v2.type));
		v2 = GetRVal(v2);
		if(oper == '+')
			v2.address += q;
		else
			ThrowError("Invalid pointer arithmetics");
		return v2;
	}
	Pdb::Val res;
	res.type = max(v1.type, v2.type);
	res.rvalue = true;
	if(res.type == DBL || res.type == FLT) {
		double a = GetFlt(v1);
		double b = GetFlt(v2);
		switch(oper) {
		case '+': res.fval = a + b; break;
		case '-': res.fval = a - b; break;
		case '*': res.fval = a * b; break;
		case '/': res.fval = a / b; break;
		default: ThrowError("Invalid operands for operation");
		}
	}
	else {
		int64 a = GetInt(v1);
		int64 b = GetInt(v2);
		switch(oper) {
		case '+': res.ival = a + b; break;
		case '-': res.ival = a - b; break;
		case '*': res.ival = a * b; break;
		case '/': res.ival = a / b; break;
		case '%': res.ival = a % b; break;
		default: ThrowError("Invalid operands for operation");
		}
	}
	return res;
}

Pdb::Val Pdb::RValue(int i)
{
	Val v;
	v.rvalue = true;
	v.ival = i;
	v.type = i >= -128 && i <= 127 ? SINT1 :
	         i >= -32768 && i <= 32767 ? SINT2 :
	         SINT4;
	return v;
}

Pdb::Val Pdb::DeRef(Pdb::Val v)
{
	if(v.ref <= 0)
		ThrowError("Only pointer can be dereferenced");
	v = GetRVal(v);
	v.ref--;
	v.rvalue = false;
	return v;
}

Pdb::Val Pdb::Ref(Pdb::Val v)
{
	if(v.rvalue)
		ThrowError("R-value cannot be referenced");
	v.rvalue = true;
	v.ref++;
	return v;
}

Pdb::Val Pdb::Field0(Pdb::Val v, const String& field)
{
	Val v1;
	v1.type = Null;
	if(v.rvalue || v.ref || v.array || v.type < 0)
		return v1;
	const Type& t = GetType(v.type);
	int q = t.member.Find(field);
	if(q >= 0) {
		Val r = t.member[q];
		r.address += v.address;
		return r;
	}
	q = t.static_member.Find(field);
	if(q >= 0)
		return t.static_member[q];
	for(int i = 0; i < t.base.GetCount(); i++) {
		v1 = t.base[i];
		v1.address += v.address;
		if(v1.type >= 0) {
			if(GetType(v1.type).name == field)
				return v1;
			v1 = Field0(v1, field);
			if(!IsNull(v1.type))
				return v1;
		}
	}
	return v1;
}

Pdb::Val Pdb::Field(Pdb::Val v, const String& field)
{
	if(v.type < 0)
		ThrowError("Must be struct, class or union");
	v = Field0(v, field);
	if(IsNull(v.type))
		ThrowError("Unknown member \'" + field + "\'");
	return v;
}

Pdb::Val Pdb::Term(CParser& p)
{
	if(p.Char2('0', 'x') || p.Char2('0', 'X'))
		return RValue(p.ReadNumber(16));
	if(p.IsChar2('0', '.')) {
		Val v;
		v.type = DBL;
		v.rvalue = true;
		v.fval = p.ReadDouble();
		return v;
	}
	if(p.Char('0'))
		return RValue(p.IsNumber() ? p.ReadNumber(8) : 0);
	if(p.IsNumber()) {
		double d = p.ReadDouble();
		if(d >= INT_MIN && d <= INT_MAX && (int)d == d) {
			return RValue((int)d);
		}
		else {
			Val v;
			v.type = DBL;
			v.rvalue = true;
			v.fval = d;
			return v;
		}
	}
	if(p.IsChar('\'')) {
		String s = p.ReadString('\'');
		if(s.GetLength() != 1)
			ThrowError("Invalid character literal");
		return RValue(s[0]);
	}
	if(p.Char('(')) {
		Val v = Exp0(p);
		p.PassChar(')');
		return v;
	}
	if(!p.IsId())
		ThrowError("Missing id");
	String id = p.ReadId();
	while(p.Char2(':', ':') && p.IsId())
		id << "::" << p.ReadId();
	int q = ~framelist;
	if(q >= 0 && q < frame.GetCount()) {
		Frame& f = frame[q];
		q = f.local.Find(id);
		if(q >= 0)
			return f.local[q];
		q = f.param.Find(id);
		if(q >= 0)
			return f.param[q];
		q = f.local.Find("this");
		if(q >= 0)
			return Field(DeRef(f.local[q]), id);
		Val v = GetGlobal(f.fn.name, id);
		if(v.address)
			return v;
	}
	ThrowError("\'" + id + "\' undefined");
	return Val();
}

String ReadType(CParser& p)
{
	String t = p.ReadId();
	if(p.Char('<')) {
		t << '<';
		int l = 1;
		while(l > 0) {
			if(p.Char('<')) {
				t << '<';
				l++;
			}
			else
			if(p.Char('>')) {
				t << '>';
				l--;
			}
			else
			if(p.IsNumber())
				t << p.ReadInt();
			else
			if(p.Char(','))
				t << ',';
			else
				t << p.ReadId();
		}
	}
	return t;
}

Pdb::Val Pdb::Post(CParser& p)
{
	Val v = Term(p);
	LLOG("Post: " << v);
	for(;;) {
		if(p.Char(':'))
			v = Field(v.ref ? DeRef(v) : v, ReadType(p));
		else
		if(p.Char('.') || p.Char2(':', ':') || p.Char2('-', '>'))
			v = Field(v.ref ? DeRef(v) : v, p.ReadId());
		else
		if(p.Char('[')) {
			int i = (int)GetInt(Exp0(p));
			p.Char(']');
			v = DeRef(Compute(v, RValue(i), '+'));
		}
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Unary(CParser& p)
{
	if(p.Char('-'))
		return Compute(RValue(0), Unary(p), '-');
	if(p.Char('+'))
		return GetRVal(Unary(p));
	if(p.Char('*'))
		return DeRef(Unary(p));
	if(p.Char('&'))
		return Ref(Unary(p));
	return Post(p);
}

Pdb::Val Pdb::Multiplicative(CParser& p)
{
	Val v = Unary(p);
	for(;;) {
		if(p.Char('*'))
			v = Compute(v, Unary(p), '*');
		else
		if(p.Char('/'))
			v = Compute(v, Unary(p), '/');
		else
		if(p.Char('%'))
			v = Compute(v, Unary(p), '%');
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Additive(CParser& p)
{
	Val v = Multiplicative(p);
	for(;;) {
		if(p.Char('+'))
			v = Compute(v, Multiplicative(p), '+');
		else
		if(p.Char('-'))
			v = Compute(v, Multiplicative(p), '-');
		else
			break;
	}
	return v;
}

Pdb::Val Pdb::Exp0(CParser& p)
{
	LLOG("Evaluating Expression: " << p.GetPtr());
	return Additive(p);
}

Pdb::Val Pdb::Exp(CParser& p)
{
	Pdb::Val v = Exp0(p);
	if(!p.IsEof())
		ThrowError("Extra tokens");
	return v;
}

#endif
