#ifndef _SDLFb_SDLFbLocal_h
#define _SDLFb_SDLFbLocal_h

#include <CtrlCore/CtrlCore.h>

//has to go second
#include "SDLWrap.h"

NAMESPACE_UPP

extern bool fbEndSession;
extern SDL_Surface * screen;
extern int videoflags;
extern int height;
extern int width;
extern int bpp;

SDL_Surface *CreateScreen(int w, int h, int bpp, int flags);
dword fbKEYtoK(dword chr);
void HandleSDLEvent(SDL_Event* event);
void HandleUserEvent(SDL_Event* event);

END_UPP_NAMESPACE

#endif

