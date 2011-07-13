#include "SDLFbLocal.h"

NAMESPACE_UPP

#define LLOG(x)       //LOG(x)

bool fbEndSession = false;
SDL_Surface * screen = NULL;
int videoflags = 0;
int height = 0;
int width = 0;
int bpp = 0;

bool FBEndSession()
{
	return fbEndSession;
}

bool FBIsWaitingEvent()
{
	SDL_PumpEvents();
	SDL_Event events;
	int tc = SDL_PeepEvents(&events, 1, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_ALLEVENTS));
	return (tc>0);
}

bool FBProcessEvent(bool *quit)
{
	SDL_Event event;
	if(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT && quit)
			*quit = true;
		HandleSDLEvent(&event);
		return true;
	}   // End while
	return false;
}

void FBSleep(int ms)
{
	Sleep(ms); //sleep should be wakeable with input
}

void FBUpdate(const Rect& inv)
{
	//The invalidated areas accumulate in the update region until the region is processed when the next WM_PAINT message occurs
	SDL_LockSurface(screen);
	const ImageBuffer& framebuffer = Ctrl::GetFrameBuffer();

#if 1
	memcpy(screen->pixels, ~framebuffer, framebuffer.GetLength() * sizeof(RGBA));
#endif

#if 0
	ASSERT(Size(screen->w,screen->h) == framebuffer.GetSize());

	Size ssz = inv.GetSize();
	Size dsz = framebuffer.GetSize();

	ASSERT(Rect(dsz).Contains(inv));

	for(int i = inv.top; i < inv.bottom; i++)
	{
		uint32 o = i * dsz.cx + inv.left;
		memcpy(((RGBA*)screen->pixels) + o, (~framebuffer) + o, ssz.cx * sizeof(RGBA));
	}
#endif

	SDL_UnlockSurface(screen);
	SDL_Flip(screen);
}

void FBFlush()
{
}

void FBInit()
{
	GuiLock __;
	
	Ctrl::InitFB();

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		Cout() << Format("Couldn't initialize SDL: %s\n", SDL_GetError());
		return;
	}

	SDL_EnableUNICODE(1); //for unicode keycode availability
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL/2);

	const SDL_VideoInfo* vi = SDL_GetVideoInfo();
	//ASSERT(vi->hw_available);

	width = vi->current_w;
	height = vi->current_h;
	bpp = vi->vfmt->BitsPerPixel;
	ASSERT(bpp == 32);
	
	//FIXME adjustable
	videoflags = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE;// | SDL_NOFRAME | SDL_FULLSCREEN;

	screen = CreateScreen(width, height, bpp, videoflags);
	ASSERT(screen);
	Ctrl::SetFramebufferSize(Size(width, height));
}

void FBDeInit()
{
	Ctrl::ExitFB();
	SDL_FreeSurface(screen);
	SDL_Quit();
}

END_UPP_NAMESPACE
