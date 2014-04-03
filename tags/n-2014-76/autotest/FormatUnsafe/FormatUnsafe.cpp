#include <Core/Core.h>

using namespace Upp;

#ifdef flagAUTOTEST
#define N 1000
#else
#define N 100000000
#endif

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	String rndset;
	rndset << "[]<>;`%%%%";
	rndset << rndset;
	rndset << rndset;
	rndset << rndset;
	for(int i = 32; i < 128; i++)
		if(IsAlpha(i) || IsDigit(i))
			rndset.Cat(i);
	SeedRandom();
	for(int i = 0; i < N; i++) {
		String fmtstr;
		while(Random(20))
			fmtstr << (char)rndset[Random(rndset.GetCount())];
		Vector<Value> v;
		for(;;) {
			int q = Random(5);
			if(q == 0)
				break;
			switch(q) {
			case 1:
				v.Add((int)Random(100));
				break;
			case 2:
				v.Add(AsString(Random(100)));
				break;
			case 3:
				v.Add(GetSysDate() + Random(100));
				break;
			}
		}
		DUMP(fmtstr);
		DUMPC(v);
		DUMP(NFormat(fmtstr, v));
	}
	
	LOG("============ OK");
}
