#include <Core/Core.h>

#include <algorithm>
#include <vector>

using namespace Upp;

#define LLOG(x)

struct Item : Comparable<Item> {
	String v;
	int    i;

	int Compare(const Item& b) const { return SgnCompare(v, b.v); }
};

void Benchmark();

void Check(int N, int M)
{
	Cout() << "LESS\n";
	for(int pass = 0; pass < 2; pass++) {
		int i;
		Vector<String> k;
		Vector<int> v;
		StableSort(k);
		Cout() << "Testing " << N << ' ' << M << EOL;
		for(i = 0; i < N; i++) {
			k.Add(AsString(rand() % M));
			v.Add(i);
		}
		Cout() << " GetStableSortOrder..." << EOL;
		Vector<int> o = pass ? GetStableSortOrderCmp(k) : GetStableSortOrder(k);
		i = 0;
		while(i < o.GetCount()) {
			LLOG(k[o[i]].v);
			if(i > 0)
				ASSERT(k[o[i]] >= k[o[i - 1]]);
			int j = i;
			while(++j < o.GetCount() && k[o[j]] == k[o[i]])
				ASSERT(o[j] > o[j - 1]);
			i = j;
		}
		Cout() << " StableIndexSort..." << EOL;
		if(pass)
			StableIndexSortCmp(k, v);
		else
			StableIndexSort(k, v);
		Cout() << " Testing..." << EOL;
		for(i = 0; i < k.GetCount(); i++)
			LLOG(k[i] << ' ' << v[i]);
		LLOG("----------");
		i = 0;
		while(i < k.GetCount()) {
			LLOG(k[i] << ' ' << v[i]);
			if(i > 0)
				ASSERT(k[i] >= k[i - 1]);
			int j = i;
			while(++j < k.GetCount() && k[j] == k[i])
				ASSERT(v[j] > v[j - 1]);
			i = j;
		}
		if(pass == 0)
			Cout() << "CMP\n";
	}
}

CONSOLE_APP_MAIN
{
	Check(1000, 100);
	Check(1000000, 100);
	Check(1000000, 10000);
	Check(1000000, 1);
	for(int i = 1; i < 30000; i += i)
		Check(2000000, i);
}

void CompileCheck()
{
	Vector<String> x;
	IndexSort(x, x);
	IndexSort2(x, x, x);
	IndexSort3(x, x, x, x);
	StableIndexSort(x, x);
	StableIndexSort2(x, x, x);
	StableIndexSort3(x, x, x, x);
	StableIndexSortCmp(x, x);
	StableIndexSort2Cmp(x, x, x);
	StableIndexSort3Cmp(x, x, x, x);
}
