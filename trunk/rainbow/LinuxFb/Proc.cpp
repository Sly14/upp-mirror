#include "LinuxFbLocal.h"

NAMESPACE_UPP

#define LLOG(x)       //LOG(x)

//FIXME get input events
bool GetShift()       { return false; }//!!(GetKeyState(VK_SHIFT) & 0x8000); }
bool GetCtrl()        { return false; }//!!(GetKeyState(VK_CONTROL) & 0x8000); }
bool GetAlt()         { return false; }//!!(GetKeyState(VK_MENU) & 0x8000); }
bool GetCapsLock()    { return false; }//!!(GetKeyState(VK_CAPITAL) & 1); }
bool GetMouseLeft()   { return mouseb & 0x4; }
bool GetMouseRight()  { return mouseb & 0x1; }
bool GetMouseMiddle() { return mouseb & 0x2; }

void purgefd(int fd)
{
	fd_set fdset;
	struct timeval tv;
	char temp[64];

	FD_ZERO(&fdset);
	FD_SET(fd, &fdset);
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	while(select(fd+1, &fdset, 0, 0, &tv) > 0)
		read(fd, temp, sizeof(temp));
}

int set_imps2(int fd, int b)
{
	uint8 set[] = {0xf3, 200, 0xf3, 100, 0xf3, 80};
	uint8 reset[] = {0xff};
	int retval = 0;

	if(b && write(fd, &set, sizeof(set)) != sizeof(set))
		return 0;
	//SDL says not to reset, some mice wont work with it
	if(!b && write(fd, &reset, sizeof (reset)) != sizeof(reset))
		return 0;

	purgefd(fd);
	return 1;
}

int has_imps2(int fd)
{
	uint8 query = 0xF2;
	purgefd(fd);
	if(write(fd, &query, sizeof (query)) != sizeof(query))
		return 0;

	uint8 ch = 0;
	fd_set fdset;
	struct timeval tv;
	while(1) {
		FD_ZERO(&fdset);
		FD_SET(fd, &fdset);
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		if(select(fd+1, &fdset, 0, 0, &tv) < 1) break;
		if(read(fd, &ch, sizeof(ch)) != sizeof(ch)) break;
		switch(ch) {
			case 0xFA:
			case 0xAA: continue;
			case 3:
			case 4: return 1;
			default: return 0;			
		}
	}
	return 0;
}

void handle_mouse()
{
	static int offs = 0;
	static unsigned char buf[BUFSIZ];
	static int rel = 1;

	int i, n;
	int b = 0;
	int dx = 0, dy = 0, dz = 0;
	int rdsize = (!mouse_imps2)?(3):(4);
	
	n = read(mouse_fd, &buf[offs], BUFSIZ-offs);
	if(n < 0) return;
	n += offs;

	for(i=0; i<(n-(rdsize-1)); i += rdsize ) {
		if((buf[i] & 0xC0) != 0) {
			i -= (rdsize-1);
			continue;
		}
		if(!mouse_imps2)
		{
			b = (buf[i] & 0x07); /*MRL*/
			dx = (buf[i] & 0x10) ? buf[i+1] - 256 : buf[i+1];
			dy = (buf[i] & 0x20) ? -(buf[i+2] - 256) : -buf[i+2];
		}
		else
		{
			b = (buf[i] & 0xC7); /*54...MRL*/
			dx = (buf[i] & 0x10) ? buf[i+1] - 256 : buf[i+1];
			dy = (buf[i] & 0x20) ? -(buf[i+2] - 256) : -buf[i+2]; 
			dz = (char)buf[i+3];
		}

		if(dx || dy)
		{
			mousep.x += dx;
			mousep.y += dy;
			mousep.x = minmax(mousep.x, 0, int(vinfo.xres));
			mousep.y = minmax(mousep.y, 0, int(vinfo.yres));
			Ctrl::DoMouseFB(Ctrl::MOUSEMOVE, mousep);
		}

		int bm = b ^ mouseb;
		mouseb = b; //for GetMouse*
		if(bm & 0x1) Ctrl::DoMouseFB((mouseb & 0x1)?Ctrl::LEFTDOWN:Ctrl::LEFTUP, mousep);
		if(bm & 0x2) Ctrl::DoMouseFB((mouseb & 0x2)?Ctrl::RIGHTDOWN:Ctrl::RIGHTUP, mousep);
		if(bm & 0x4) Ctrl::DoMouseFB((mouseb & 0x4)?Ctrl::MIDDLEDOWN:Ctrl::MIDDLEUP, mousep);
		if(dz) Ctrl::DoMouseFB(Ctrl::MOUSEWHEEL, mousep, dz*120);
	}
	if(i < n) {
		memcpy(buf, &buf[i], (n-i));
		offs = (n-i);
	} else {
		offs = 0;
	}
}

void handle_keyboard()
{
	
}

//returns 0 if timeout, 1 for mouse, 2 for keyboard
//common for waitforevents and sleep 
int readevents(int ms)
{
	fd_set fdset;
	int max_fd;
	static struct timeval to;
	to.tv_sec = ms / 1000;
	to.tv_usec = ms % 1000 * 1000;

	FD_ZERO(&fdset);
	max_fd = 0;
	if(keyboard_fd >= 0) {
		FD_SET(keyboard_fd, &fdset);
		if(max_fd < keyboard_fd) {
			max_fd = keyboard_fd;
		}
	}
	if(mouse_fd >= 0) {
		FD_SET(mouse_fd, &fdset);
		if(max_fd < mouse_fd) {
			max_fd = mouse_fd;
		}
	}
	if(select(max_fd+1, &fdset, NULL, NULL, &to) > 0 ) {
		if(keyboard_fd >= 0 ) {
			if(FD_ISSET(keyboard_fd, &fdset)) {
				return 2;
			}
		}
		if(mouse_fd >= 0) {
			if (FD_ISSET(mouse_fd, &fdset)) {
				return 1;
			}
		}
	}
	return 0;
}

END_UPP_NAMESPACE
