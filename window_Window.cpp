#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include "window.h"
using namespace std;
//-----------mywindow
Desktop* MyWindow::desktop = Desktop::GetDesktop ();
ProcessEvent* MyWindow::processEvent = ProcessEvent::GetProcessEvent ();
//-----------
MyWindow::MyWindow (int x, int y, unsigned int w, unsigned int h, Window client) {
	this->x = this->oldx = x;
	this->y = this->oldy = y;
	this->width = this->oldWidth = w;
	this->height = this->oldHeight = h;
	this->topbarHeight = 20;
	this->ismodel = false;
	this->isicon = false;
	this->ismax = false;
	XSetWindowAttributes attr;
	unsigned long valuemask = 0;
	//---------------root
	attr.background_pixel = XWhitePixel (desktop->display, desktop->screenNum);
	//attr.override_redirect = true;
	valuemask = CWBackPixel;
	this->root = XCreateWindow (desktop->display, desktop->Get_desktop (),
			x, y, width, height, 0,
			XDefaultDepth (desktop->display, desktop->screenNum), InputOutput,
			XDefaultVisual (desktop->display, desktop->screenNum),
			valuemask, &attr);
	//---------------topbar
	attr.background_pixel = 10593190;
	attr.event_mask = ButtonPressMask | PointerMotionMask | ExposureMask;
	valuemask = CWBackPixel | CWEventMask;
	this->topbar = XCreateWindow (desktop->display, root,
			0, 0, width, topbarHeight, 0,
			CopyFromParent, CopyFromParent, CopyFromParent,
			valuemask, &attr);
	//--------------content
	attr.background_pixel = XWhitePixel (desktop->display, desktop->screenNum);
	//attr.override_redirect = true;
	attr.event_mask = EnterWindowMask;
	valuemask = CWBackPixel | CWEventMask;
	this->content = XCreateWindow (desktop->display, root,
			0, topbarHeight, width, height - topbarHeight, 0,
			CopyFromParent, CopyFromParent, CopyFromParent,
			valuemask, &attr);
	//---------------client
	this->client = client;
	XSetWindowBorderWidth (desktop->display, client, 0);
	XSelectInput (desktop->display, this->client, StructureNotifyMask);
	XResizeWindow (desktop->display, this->client, width, height - topbarHeight);
	XReparentWindow (desktop->display, this->client, this->content, 0, 0);
	//---------------btn
	attr.background_pixel = 16707309;
	attr.event_mask = ButtonPressMask | ButtonReleaseMask | ExposureMask;
	valuemask = CWBackPixel | CWEventMask;
	this->btn1 = XCreateWindow (desktop->display, topbar,
			3, 3, 13, 13, 0,
			CopyFromParent, CopyFromParent, CopyFromParent,
			valuemask, &attr);
	this->btn2 = XCreateWindow (desktop->display, topbar,
			20, 3, 13, 13, 0,
			CopyFromParent, CopyFromParent, CopyFromParent,
			valuemask, &attr);
	this->btn3 = XCreateWindow (desktop->display, topbar,
			37, 3, 13, 13, 0,
			CopyFromParent, CopyFromParent, CopyFromParent,
			valuemask, &attr);
	attr.event_mask = ButtonPressMask | PointerMotionMask | ExposureMask;
	valuemask = CWBackPixel | CWEventMask;
	this->btn5 = XCreateWindow (desktop->display, topbar,
			width - topbarHeight, 0, topbarHeight, topbarHeight, 0,
			CopyFromParent, CopyFromParent, CopyFromParent,
			valuemask, &attr);
	//----------icon
	attr.background_pixel = 13748151;
	attr.event_mask = ButtonPressMask | ButtonReleaseMask | EnterWindowMask | ExposureMask;
	valuemask = CWBackPixel | CWEventMask;
	this->icon = XCreateWindow (desktop->display, desktop->Get_taskbar(), 0, 0, 
			desktop->Get_topbarHeight (), topbarHeight, 0,
			CopyFromParent, CopyFromParent, CopyFromParent,
			valuemask, &attr);
	XSync (desktop->display, false);
}
void MyWindow::MapWindow () { //use x, y, width, height
	if (this->y < 0)
		this->y = 0;
	XMapWindow (desktop->display, btn1);
	XMapWindow (desktop->display, btn2);
	XMapWindow (desktop->display, btn3);
	if (ismax || ismodel)
		XUnmapWindow (desktop->display, btn5);
	else {
		XMoveWindow (desktop->display, btn5, width - topbarHeight, 0);
		XMapWindow (desktop->display, btn5);
	}
	XResizeWindow (desktop->display, topbar, width, topbarHeight);
	XMapWindow (desktop->display, topbar);
	//XResizeWindow (desktop->display, client, width, height - topbarHeight);
	XMapWindow (desktop->display, client);
	XResizeWindow (desktop->display, content, width, height - topbarHeight);
	XMapWindow (desktop->display, content);
	XMoveResizeWindow (desktop->display, root, x, y, width, height);
	XMapWindow (desktop->display, root);
	XSync (desktop->display, false);
	return;
}
void MyWindow::MapWindowBack () { //use oldx, oldy, oldwidth, oldheight
	if (this->oldy < 0)
		this->oldy = 0;
	XMapWindow (desktop->display, btn1);
	XMapWindow (desktop->display, btn2);
	XMapWindow (desktop->display, btn3);
	XMoveWindow (desktop->display, btn5, oldWidth - topbarHeight, 0);
	XMapWindow (desktop->display, btn5);
	XResizeWindow (desktop->display, topbar, oldWidth, topbarHeight);
	XMapWindow (desktop->display, topbar);
	//XResizeWindow (desktop->display, client, oldWidth, oldHeight - topbarHeight);
	XMapWindow (desktop->display, client);
	XResizeWindow (desktop->display, content, oldWidth, oldHeight - topbarHeight);
	XMapWindow (desktop->display, content);
	XMoveResizeWindow (desktop->display, root, oldx, oldy, oldWidth, oldHeight);
	XMapWindow (desktop->display, root);
	XSync (desktop->display, false);
	return;
}
void MyWindow::ShowWindow () {
	if (this->ismodel) {
		this->width = desktop->Get_width () / 2;
		this->x = this->isx;
	}
	else if (this->ismax) {
		this->width = desktop->Get_width ();
		this->x = this->isx;
	}
	else {
		this->x = this->oldx;
		this->y = this->oldy;
		this->width = this->oldWidth;
		this->height = this->oldHeight;
	}
	MapWindow ();
	this->isicon = false;
	return;
}
void MyWindow::HideWindow () {
	XUnmapWindow (desktop->display, this->root);
	XSync (desktop->display, false);
	this->isicon = true;
	return;
}
void MyWindow::CloseWindow () {
	XGrabServer (desktop->display);
	XSetCloseDownMode (desktop->display, DestroyAll);
	XKillClient (desktop->display, this->client);
	XSync (desktop->display, false);
	XUngrabServer (desktop->display);
	return;
}
void MyWindow::MaxWindow () {
	if (!ismax) {
		this->x =  -desktop->Get_x ();
		this->y = 0;
		this->width = desktop->Get_width ();
		this->height = desktop->Get_height () - desktop->Get_topbarHeight ();
		MapWindow ();
		XResizeWindow (desktop->display, client, width, height - topbarHeight);
		this->isx = -desktop->Get_x ();
		this->ismax = true;
		this->ismodel = false;
		this->isicon = false;
		return;
	}
	else {
		MapWindowBack ();
		this->ismax = false;
		return;
	}
}
