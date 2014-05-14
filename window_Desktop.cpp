#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "window.h"
using namespace std;
//-----------desktop
ProcessEvent* Desktop::processEvent = ProcessEvent::GetProcessEvent ();
//------------
Desktop* Desktop::GetDesktop () {
	static Desktop desktop;
	return &desktop;
}
Desktop::Desktop () {
	if ((this->display = XOpenDisplay (NULL)) == NULL) {
		cerr << "ERROR: opendisplay error!" << endl;
		//exit (1);
	}
	this->screenNum = XDefaultScreen (display);
	XSelectInput (display, XRootWindow (display, screenNum), SubstructureRedirectMask | PointerMotionMask | ButtonReleaseMask | KeyPressMask);
	XSync (display, false);
	XSync (display, false);
	this->width = XDisplayWidth (display, screenNum);
	this->height = XDisplayHeight (display, screenNum) - topbarHeight;
	this->topbarHeight = 20;
	this->timebarWidth = 300;
	this->systembarWidth = this->taskbarWidth = (width - timebarWidth) / 2;
	this->buttonWidth = 100;
	this->x = 0;
	//-----------GC
	XGCValues values;
	values.foreground = XBlackPixel (display, screenNum);
	values.background = XWhitePixel (display, screenNum);
	values.line_width = 2;
	values.join_style = JoinMiter;
	unsigned long mask = GCForeground | GCBackground | GCLineWidth | GCJoinStyle;
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
	//-----------
	XSetWindowAttributes attr;
	unsigned long valuemask = 0;
	//---------desktop
	attr.background_pixel = 12506946;
	attr.event_mask = ButtonPressMask | ButtonReleaseMask
		| KeyPressMask | KeyReleaseMask | PointerMotionMask;
	valuemask = CWBackPixel | CWEventMask;
	this->desktop = XCreateWindow (display, XRootWindow (display, screenNum),
			0, topbarHeight, width * 2, height, 0, 
			XDefaultDepth (display, screenNum),InputOutput, 
			XDefaultVisual (display, screenNum),
			valuemask, &attr);
	//---------topbar
	attr.background_pixel = 12040811;
	attr.event_mask = ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
	valuemask = CWBackPixel | CWEventMask;
	this->topbar = XCreateWindow (display, XRootWindow (display, screenNum),
			0, 0, width, topbarHeight, 0, 
			XDefaultDepth (display, screenNum), InputOutput, 
			XDefaultVisual (display, screenNum),
			valuemask, &attr);
	//---------taskbar
	attr.background_pixel = 13493959;
	valuemask = CWBackPixel;
	this->taskbar = XCreateWindow (display, topbar, 0, 0, 
			taskbarWidth, topbarHeight, 0,
			XDefaultDepth (display, screenNum), InputOutput, 
			XDefaultVisual (display, screenNum),
			valuemask, &attr);
	//--------timebar
	attr.background_pixel = 14590764;
	valuemask = CWBackPixel;
	this->timebar = XCreateWindow (display, topbar, taskbarWidth, 0,
			timebarWidth, topbarHeight, 0,
			XDefaultDepth (display, screenNum), InputOutput,
			XDefaultVisual (display, screenNum),
			valuemask, &attr);
	//----------systembar
	attr.background_pixel = 12689779;
	valuemask = CWBackPixel;
	this->systembar = XCreateWindow (display, topbar, taskbarWidth + timebarWidth, 0,
			width - taskbarWidth - timebarWidth, topbarHeight, 0,
			XDefaultDepth (display, screenNum), InputOutput,
			XDefaultVisual (display, screenNum),
			valuemask, &attr);
	//----------cursor
	this->cursor = XCreateFontCursor (display, XC_left_ptr);
	XDefineCursor (display, XDefaultRootWindow (display), cursor);
	XSync (display, false);
	XUngrabKey (display, AnyKey, AnyModifier, XRootWindow (display, screenNum));
	KeyCode code;
	code = XKeysymToKeycode (display, XK_Return);
	XGrabKey (display, code, Mod1Mask | ShiftMask, desktop, true, GrabModeAsync, GrabModeAsync);
	code = XKeysymToKeycode (display, XK_q);
	XGrabKey (display, code, Mod1Mask | ShiftMask, desktop, true, GrabModeAsync, GrabModeAsync);
	XSync (display, false);
	//-------------systembutton
	attr.background_pixel = 11526116;
	attr.event_mask = ExposureMask | ButtonPressMask
		| ButtonReleaseMask;
	valuemask = CWBackPixel | CWEventMask;
	this->launchbutton = XCreateWindow (display, systembar, 
			systembarWidth - buttonWidth, 0, buttonWidth, topbarHeight, 0,
			CopyFromParent, CopyFromParent, CopyFromParent,
			valuemask, &attr);
	XMapWindow (display, launchbutton);
	this->xtermbutton = XCreateWindow (display, systembar, 
			systembarWidth - buttonWidth * 2 - 3, 0, buttonWidth, topbarHeight, 0,
			CopyFromParent, CopyFromParent, CopyFromParent,
			valuemask, &attr);
	XMapWindow (display, xtermbutton);
	//==========
	processEvent->spawnP ("./clock", "clock");
}
//----------mapDesktop
void Desktop::MapDesktop () {
	XMapWindow (display, taskbar);
	XMapWindow (display, timebar);
	XMapWindow (display, systembar);
	XMapWindow (display, topbar);
	XMapWindow (display, desktop);
	XSync (display, false);
}
void Desktop::Configure (XEvent *event) {
	XConfigureRequestEvent *ev = &(event->xconfigurerequest);
	XWindowChanges wc;
	wc.x = ev->x;
	wc.y = ev->y;
	wc.width = ev->width;
	wc.height = ev->height;
	wc.border_width = ev->border_width;
	wc.sibling = ev->above;
	wc.stack_mode = ev->detail;
	XConfigureWindow (display, ev->window, ev->value_mask, &wc);
}
void Desktop::Map (XEvent *event) {
	XWindowAttributes attr;
	XGetWindowAttributes (display, event->xmaprequest.window, &attr);
	int x = attr.x, y = attr.y;
	unsigned width = attr.width, height = attr.height;
	if (width < 100 || width > width)
		width = 600;
	if (height < 50 || height > height)
		height = 400;
	x = (this->width - width) / 2 - this->x;
	y = (this->height - height - topbarHeight) / 2;
	//-----------get name
	char **list = NULL;
	int n;
	XTextProperty name;
	char _name[256];
	_name[0] = '\0';
	XGetTextProperty (display, event->xmaprequest.window, &name, XA_WM_NAME);
	if (name.encoding == XA_STRING)
		strncpy (_name, (char *)name.value, sizeof _name - 1);
	else {
		if (XmbTextPropertyToTextList (display, &name, &list, &n) >= Success
				&& n > 0 && *list) {
			strncpy (_name, *list, sizeof _name - 1);
			XFreeStringList (list);
		}
	}
	_name[sizeof _name - 1] = '\0';
	XFree (name.value);
	if (_name[0] == '\0')
		strcpy (_name, "UNKNOW");
	if ((strcmp (_name, "_my_clock") == 0)) {
		x = (this->timebarWidth - attr.width) / 2;
		y = (this->topbarHeight - attr.height) / 2;
		x = x < 0 ? 0 : x;
		y = y < 0 ? 0 : y;
		XReparentWindow (display, event->xmaprequest.window, this->timebar, x, y);
		XMapWindow (display, event->xmaprequest.window);
		return;
	}
	//-----------------new client
	MyWindow *client = new MyWindow (x, y, width, height, 
			event->xmaprequest.window);
	client->Set_name (_name);
	//-------------------
	processEvent->clients.push_back (client);
	processEvent->stacks.push_back (client->Get_root ());
	client->MapWindow ();
	XSetInputFocus (display, client->Get_client (), RevertToPointerRoot, CurrentTime);
	cerr << "SUCCESS: create client" << endl;
}
void Desktop::ManageIcon () {
	for (unsigned int i = 0; i < processEvent->clients.size(); ++i) {
		XMoveResizeWindow (display, processEvent->clients[i]->Get_icon (), 
				i * 100 + i * 3, 0, 100, topbarHeight);
		XMapWindow (display, processEvent->clients[i]->Get_icon ());
		XSync (display, false);
	}
}
void Desktop::MoveDesktop (XEvent *event) {
	if (event->xbutton.button == Button5)
		x -= 100;
	else if (event->xbutton.button == Button4)
		x += 100;
	else
		this->x = event->xmotion.x - motionx;
	if (x >= 0)
		x = 0;
	else if (x <= -width)
		x = -width;
	XMoveWindow (display,desktop, x, topbarHeight);
	XSync (display, false);
	return;
}
