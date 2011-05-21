#include "SystemDraw.h"
#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

#define LLOG(x) // LOG(x)
#define LTIMING(x) // RTIMING(x)

#if defined(PLATFORM_WIN32) && defined(flagOPENGL)

int64 Resources::currentSerialId = -1;
Index<int64> Resources::textures;
VectorMap<String, OpenGLFont> Resources::fonts;

float GetFps()
{
	static float fps = 0.0f;
	static dword updateInterval = 1000;
	static dword timeSinceLastUpdate = 0;
	static dword frameCount = 0;
	static dword currentTick;
	static dword lastTick;
	static bool  isFirst = true;

	if(isFirst)
	{
		currentTick = GetTickCount();
		lastTick = currentTick;
		isFirst = false;
	}

	frameCount++;
	currentTick = GetTickCount();

	dword elapsed = currentTick - lastTick;

	lastTick = currentTick;
	timeSinceLastUpdate += elapsed;

	if (timeSinceLastUpdate > updateInterval)
	{
		if (timeSinceLastUpdate)
		{
			fps = (frameCount / float(timeSinceLastUpdate)) * 1000.f;
			frameCount = 0;
			timeSinceLastUpdate -= updateInterval;
		}
	}

	return fps;
}


void Texture::Add(const Rect& r)
{
	Size rsz = r.GetSize();	
}

int64 Resources::Bind(const Image& img, bool linear)
{
	int64 serialId = img.GetSerialId();

	if(!Bind(serialId))
		return serialId;

	int textureNumber = textures.Find(serialId);

	if(textureNumber < 0)
	{
		textures.Add(serialId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.GetWidth(), img.GetHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linear ? GL_LINEAR : GL_NEAREST);
	}

	return serialId;
}

bool Resources::Bind(int64 serialId, bool force)
{
	if(!force && serialId == currentSerialId)
		return false;
	
	currentSerialId = serialId;
	glBindTexture(GL_TEXTURE_2D, (GLuint) serialId);
	return true;
}

OpenGLFont& Resources::GetFont(const char* fontName)
{
	int n = fonts.Find(fontName);
	if(n >= 0)
		return fonts[n];
	else
	{
		OpenGLFont& font = fonts.Add(fontName);
		font.Load(fontName);
		return font;
	}
}

OpenGLFont& Resources::StdFont(bool bold)
{
	return GetFont(bold ? "tahoma14b.fnt" : "tahoma14.fnt");
}

dword OpenGLDraw::GetInfo() const
{
	return 0;
}

Size OpenGLDraw::GetPageSize() const
{
	return Size(0, 0);
}

Size OpenGLDraw::GetNativeDpi() const
{
	return Size(96, 96);
}

void OpenGLDraw::BeginNative()
{
}

void OpenGLDraw::EndNative()
{
}

int OpenGLDraw::GetCloffLevel() const
{
	return ci;
}

void OpenGLDraw::InitClip(const Rect& clip)
{
	drawing_clip = clip;
}

void OpenGLDraw::Reset() {
	cloff.SetCount(20);
	ci = 0;
	drawing_offset = Point(0, 0);
	alpha = 255;
	angle = 0.f;
}

OpenGLDraw::OpenGLDraw() {
	Reset();
}

OpenGLDraw::OpenGLDraw(HDC hdc, Size sz) {
	Reset();
	drawing_clip = sz;
	drawing_size = sz;
	clip = sz;
	Init();
}

void OpenGLDraw::Init()
{
	glViewport(0, 0, (GLsizei) drawing_size.cx, (GLsizei) drawing_size.cy);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
#if CLIP_MODE == 0
	glEnable(GL_SCISSOR_TEST);
#elif CLIP_MODE == 2
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0, ~0);
	glStencilFunc(GL_KEEP, GL_KEEP, GL_KEEP);
	glClearStencil(0);
#endif
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClearDepth(1.0f);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4f(1.f, 1.f, 1.f, 1.f);
}

void OpenGLDraw::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLDraw::FlatView()
{
	float aspect = drawing_size.cx / (float) drawing_size.cy;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, drawing_size.cx, drawing_size.cy, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float dx = (float) drawing_size.cx / 2;
	float dy = (float) drawing_size.cy / 2;
	glTranslatef(dx, dy, 0.f);
	glRotatef(angle, 0, 0, 1);
	glTranslatef(-dx, -dy, 0.f);
}

OpenGLDraw::~OpenGLDraw() {
}


#endif
END_UPP_NAMESPACE
