#include <Core/Core.h>

using namespace Upp;

struct MyCustomValue : AssignValueTypeNo<MyCustomValue, 514> {
	int    x, y, z;

	bool      operator==(const MyCustomValue& o) const { return false; }
	void     Serialize(Stream& s)                     {}
	String   ToString() const                         { return "MyCustomValue"; }
	unsigned GetHashValue() const                     { return 0; }
	bool     IsNullInstance() const                   { return false; }

	operator Value() const                   { return RichValue<MyCustomValue>(*this); }
	MyCustomValue(const Value& v)            { *this = ValueTo<MyCustomValue>(v); }

	MyCustomValue()                          {}
};

void Xmlize(XmlIO xml, MyCustomValue& v)
{
	xml
		.Attr("x", v.x)
		.Attr("y", v.y)
		.Attr("z", v.z)
	;
}

INITBLOCK {
	RichValue<MyCustomValue>::Register();
}

REGISTER_VALUE_XMLIZE(MyCustomValue);

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	MyCustomValue a;
	a.x = 1;
	a.y = 2;
	a.z = 3;
	Value v = a;
	LOG(StoreAsXML(v, "Test"));
}
