#include <Core/Core.h>

using namespace Upp;

void Test(Convert& cv, const char *s, Value res)
{
	DDUMP(cv.Scan(s));
	Value v = cv.Scan(s);
	if(IsError(res))
		ASSERT(IsError(v));
	else
		ASSERT(v == res);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	SetLanguage(LNG_('C','S','C','Z'));

	ConvertTime cv;

	DUMP(cv.Format(Date(2013, 1, 2)));
	ASSERT(cv.Format(Date(2013, 1, 2)) == "02.01.2013");

	DUMP(cv.Format(Time(2013, 1, 2)));
	ASSERT(cv.Format(Time(2013, 1, 2)) == "02.01.2013");

	DUMP(cv.Format(Time(2013, 1, 2, 11, 12, 13)));
	ASSERT(cv.Format(Time(2013, 1, 2, 11, 12, 13)) == "02.01.2013 11:12:13");
	
	cv.TimeAlways();

	DUMP(cv.Format(Date(2013, 1, 2)));
	ASSERT(cv.Format(Date(2013, 1, 2)) == "02.01.2013 00:00:00");

	DUMP(cv.Format(Time(2013, 1, 2, 11, 12, 13)));
	ASSERT(cv.Format(Time(2013, 1, 2, 11, 12, 13)) == "02.01.2013 11:12:13");
	
	DUMP(cv.Scan("15.6.2010"));
	ASSERT(cv.Scan("15.6.2010") == Time(2010, 6, 15, 0, 0, 0));

	cv.DayEnd();

	DUMP(cv.Scan("15.6.2010"));
	ASSERT(cv.Scan("15.6.2010") == Time(2010, 6, 15, 23, 59, 59));
	
	ConvertDouble d;

	DUMP(d.Scan("12-12"));
	ASSERT(IsError(d.Scan("12-12")));

	Test(d, "12-12", ErrorValue());
	Test(d, "12+12", ErrorValue());
	Test(d, "12.-12", ErrorValue());
	Test(d, "12.+12", ErrorValue());
	Test(d, "a", ErrorValue());
	Test(d, " ", ErrorValue());

	Test(d, "123 ", 123);

	Test(d, "0", 0);
	Test(d, "123", 123);
	Test(d, "+123", 123);
	Test(d, "-123", -123);
	Test(d, "1.1", 1.1);
	Test(d, "+1.1", 1.1);
	Test(d, "-1.1", -1.1);
	Test(d, "1.1e10", 1.1e10);
	Test(d, "+1.1e10", 1.1e10);
	Test(d, "-1.1e10", -1.1e10);
	Test(d, "1.1e+10", 1.1e10);
	Test(d, "+1.1e+10", 1.1e10);
	Test(d, "-1.1e+10", -1.1e10);
	Test(d, "1.1e-10", 1.1e-10);
	Test(d, "+1.1e-10", 1.1e-10);
	Test(d, "-1.1e-10", -1.1e-10);
	Test(d, "1.1E10", 1.1E10);
	Test(d, "+1.1E10", 1.1E10);
	Test(d, "-1.1E10", -1.1E10);
	Test(d, "1.1E+10", 1.1E10);
	Test(d, "+1.1E+10", 1.1E10);
	Test(d, "-1.1E+10", -1.1E10);
	Test(d, "1.1E-10", 1.1E-10);
	Test(d, "+1.1E-10", 1.1E-10);
	Test(d, "-1.1E-10", -1.1E-10);
}
