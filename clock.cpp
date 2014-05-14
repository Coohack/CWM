#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include "window.h"
using namespace std;

int main (int argc, char **argv) {
	Display *display;
	int screenNum;
	Window window;
	if ((display = XOpenDisplay (NULL)) == NULL) {
		cerr << "ERROR: -----clock cannot openg display" << endl;
		exit (1);
	}
	screenNum = XDefaultScreen (display);
	//-----------GC
	GC gc;
	XGCValues values;
	values.foreground = XBlackPixel (display, screenNum);
	values.background = XWhitePixel (display, screenNum);
	unsigned long mask = GCForeground | GCBackground;
	if (!(gc = XCreateGC (display, XRootWindow (display, screenNum), mask, &values))) {
		cerr << "ERROR: create gc error!" << endl;
		//exit (1);
	}
	XFontStruct *font_info;
	if (!(font_info = XLoadQueryFont (display, "*-misc-*-20-*"))) {
		cerr << "ERROR: load font error" << endl;
		//exit (1);
	}
	XSetFont (display, gc, font_info->fid);
	//------------
	XSetWindowAttributes attr;
	unsigned long valuemask = 0;
	attr.background_pixel = 14590764;
	valuemask = CWBackPixel;
	window = XCreateWindow (display, XRootWindow (display, screenNum),
			0, 0, 200, 20, 0,
			CopyFromParent, CopyFromParent, CopyFromParent,
			valuemask, &attr);
	XStoreName (display, window, "_my_clock");
	XMapWindow (display, window);

	time_t t;
	while (1) {
		t = time (&t);
		char* str = ctime (&t);
		XClearWindow (display, window);
		XDrawString (display, window, gc, 2, 17, str, strlen (str));
		XSync (display, false);
		sleep (1);
	}
}
