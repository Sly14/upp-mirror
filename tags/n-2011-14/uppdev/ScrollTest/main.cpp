#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct App : public TopWindow {
	Point pos;
	Button ok, tl;

	virtual void Paint(Draw &w)
	{
		Size sz = GetSize();
		for(int i = pos.x / 50 - 50; (i * 50 - pos.y) < sz.cy; i++)
			for(int j = pos.y / 50 - 50; (j * 50 - pos.x) < sz.cx; j++)
				w.DrawRect(j * 50 - pos.x, i * 50 - pos.y, 50, 50,
				           Color(33 * j % 255, 11 * i % 255, 100));
	}

	virtual bool Key(dword key, int count)
	{
		if(key == VK_SPACE) {
			Size sz = GetSize();
			Size delta(rand() % 50 - 25, rand() % 50 - 25);
			delta = Size(0, -10);
			pos += delta;
			ScrollView(-delta);
		}
		return true;
	}

	App()
	{
		pos = Point(0, 0);
		Add(ok.HCenterPos(20).VCenterPos(20));
		Add(tl.TopPos(0, 20).LeftPos(0, 20));
	}
};

GUI_APP_MAIN
{
	Ctrl::ShowRepaint(30);
	App().Run();
}
