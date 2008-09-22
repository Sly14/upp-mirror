#include "Core.h"

NAMESPACE_UPP

//#BLITZ_APPROVE

void Uuid::Serialize(Stream& s) {
	int version = 0;
	s / version % a % b %c % d;
}

Uuid Uuid::Create() {
	Uuid ud;
	ud.a = Random();
	ud.b = Random();
	ud.c = Random();
	ud.d = Random();
	return ud;
}

String Format(const Uuid& id) {
	return Sprintf("%08X%08X%08X%08X", id.a, id.b, id.c, id.d);
}

dword scanX(const char *s)
{
	dword r = 0;
	for(int i = 0; i < 8; i++) {
		r = (r << 4) | (*s >= '0' && *s <= '9' ? *s - '0' :
		                *s >= 'A' && *s <= 'F' ? *s - 'A' :
		                *s >= 'a' && *s <= 'f' ? *s - 'a' : 0);
		s++;
	}
	return r;
}

Uuid ScanUuid(const char *s)
{
	Uuid id;
	String xu;
	while(*s) {
		if(IsXDigit(*s))
			xu.Cat(*s);
		s++;
	}
	if(xu.GetCount() < 32)
		return Null;
	id.a = scanX(~xu);
	id.b = scanX(~xu + 8);
	id.c = scanX(~xu + 16);
	id.d = scanX(~xu + 24);
	return id;
}

String Uuid::ToString() const
{
	return Format(*this);
}

String Dump(const Uuid& id) {
	return "UUID: " + Format(id);
}

struct UuidValueGenClass : ValueGen
{
	virtual Value Get() {
		return Format(Uuid::Create());
	}
};

ValueGen& UuidValueGen()
{
	return Single<UuidValueGenClass>();
}

END_UPP_NAMESPACE
