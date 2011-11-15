//$ class Ctrl {
	static Ptr<Ctrl>      desktop;
	static Vector<Ctrl *> topctrl;
	static InfoPanel      infoPanel;
	
	static float angle;
	static float scale;
	static float alpha;

	static Point glCursorPos;
	static Image glCursorImage;
	static Rect  glCaretRect;
	static int   glCaretTm;

	static int64 glEventLoop;
	static int64 glEndSessionLoop;

	int FindTopCtrl() const;
	static void SyncTopWindows();
	void DestroyWnd();
	void NewTop()                       { top = new Top; top->owner_window = NULL; }
	void PutForeground();
	static void MouseEventGl(Ptr<Ctrl> t, int event, Point p, int zdelta);

	static void DrawLine(const Vector<Rect>& clip, int x, int y, int cx, int cy, bool horz,
	                     const byte *pattern, int animation);
	static void DragRectDraw0(const Vector<Rect>& clip, const Rect& rect, int n,
	                          const byte *pattern, int animation);


	friend class TopWindowFrame;
	friend class SystemDraw;
	friend struct DnDLoop;

	void  SetOpen(bool b) { isopen = b; }

protected:
	static int PaintLock;

public:
	static Console console;
	static Rect screenRect;

	static void InitGl();
	static void ExitGl();
	static void EndSession();
	static bool DoMouseGl(Ctrl* q, int event, Point p, int zdelta);
	static void DoMouseGl(int event, Point p, int zdelta = 0);
	static bool DoKeyGl(dword key, int cnt);
	static void CursorSync(Draw& w);
	static void MouseSync(Draw& w);
	
	static void  SetDesktop(Ctrl& q);
	static Ctrl *GetDesktop() { return desktop; }
	static void  SetWindowSize(Size sz);
	
	static void  DrawScreen();
	
	virtual void ApplyTransform(TransformState state) {}
	void DragRectDraw(const Rect& rect1, const Rect& rect2, const Rect& clip, int n,
	                  Color color, int type, int animation);

	static Ctrl *FindMouseTopCtrl();

	static bool FullWindowDrag;
	
	enum { DRAWDRAGRECT_SCREEN = 0x8000 };

//$ };
