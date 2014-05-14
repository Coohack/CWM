#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include "window.h"
//--------------------ProcessWindow
ProcessEvent* ProcessWindow::processEvent = ProcessEvent::GetProcessEvent ();
Desktop* ProcessWindow::desktop = Desktop::GetDesktop ();
//---------------======================-------------
ProcessWindow::ProcessWindow () {
	//-----------expose
	handlerWindow[Expose][enum_window_icon] = &ProcessWindow::expose_window_icon;
	handlerWindow[Expose][enum_window_topbar] = &ProcessWindow::expose_window_topbar;
	handlerWindow[Expose][enum_window_btn1] = &ProcessWindow::expose_window_btn1;
	handlerWindow[Expose][enum_window_btn2] = &ProcessWindow::expose_window_btn2;
	handlerWindow[Expose][enum_window_btn3] = &ProcessWindow::expose_window_btn3;
	handlerWindow[Expose][enum_window_btn5] = &ProcessWindow::expose_window_btn5;
	handlerWindow[Expose][enum_desktop_launchbutton] = &ProcessWindow::expose_desktop_launchbutton;
	handlerWindow[Expose][enum_desktop_xtermbutton] = &ProcessWindow::expose_desktop_xtermbutton;
	//-----------motionNotify
	handlerWindow[MotionNotify][enum_desktop_desktop] = &ProcessWindow::motionNotify_desktop_desktop;
	handlerWindow[MotionNotify][enum_desktop_root] = &ProcessWindow::motionNotify_desktop_root;
	//---------------buttonPress buttonRelease
	handlerWindow[ButtonPress][enum_window_topbar] = &ProcessWindow::buttonPress_window_topbar;
	handlerWindow[ButtonPress][enum_window_btn5] = &ProcessWindow::buttonPress_window_btn5;
	handlerWindow[ButtonPress][enum_window_icon] = &ProcessWindow::buttonPress_window_icon;
	handlerWindow[ButtonRelease][enum_window_icon] = &ProcessWindow::buttonRelease_window_icon;
	handlerWindow[ButtonPress][enum_window_btn1] = &ProcessWindow::buttonPress_window_btn1;
	handlerWindow[ButtonRelease][enum_window_btn1] = &ProcessWindow::buttonRelease_window_btn1;
	handlerWindow[ButtonPress][enum_window_btn2] = &ProcessWindow::buttonPress_window_btn2;
	handlerWindow[ButtonRelease][enum_window_btn2] = &ProcessWindow::buttonRelease_window_btn2;
	handlerWindow[ButtonPress][enum_window_btn3] = &ProcessWindow::buttonPress_window_btn3;
	handlerWindow[ButtonRelease][enum_window_btn3] = &ProcessWindow::buttonRelease_window_btn3;
	handlerWindow[ButtonPress][enum_desktop_desktop] = &ProcessWindow::buttonPress_desktop_desktop;
	handlerWindow[ButtonRelease][enum_desktop_desktop] = &ProcessWindow::buttonRelease_desktop_desktop;
	handlerWindow[ButtonPress][enum_desktop_topbar] = &ProcessWindow::buttonPress_desktop_topbar;
	handlerWindow[ButtonRelease][enum_desktop_root] = &ProcessWindow::buttonRelease_desktop_root;
	handlerWindow[ButtonPress][enum_desktop_xtermbutton] = &ProcessWindow::buttonPress_desktop_xtermbutton;
	handlerWindow[ButtonPress][enum_desktop_launchbutton] = &ProcessWindow::buttonPress_desktop_launchbutton;
}
ProcessWindow* ProcessWindow::GetProcessWindow () {
	static ProcessWindow processWindow;
	return &processWindow;
}
//--------------expose motionNotify
void ProcessWindow::expose_window_topbar (XEvent *event) {
	XClearWindow (desktop->display, processEvent->processClient.myWindow->Get_topbar ());
	XDrawString (desktop->display, processEvent->processClient.myWindow->Get_topbar (), 
			desktop->Get_gc (), 100, 18, processEvent->processClient.myWindow->Get_name (), 
			strlen (processEvent->processClient.myWindow->Get_name ()));
	XSync (desktop->display, false);
}
void ProcessWindow::expose_window_icon (XEvent *event) {
	XClearWindow (desktop->display, processEvent->processClient.myWindow->Get_icon ());
	XDrawString (desktop->display, processEvent->processClient.myWindow->Get_icon (),
			desktop->Get_gc (), 2, 18, processEvent->processClient.myWindow->Get_name (),
			strlen (processEvent->processClient.myWindow->Get_name ()));
	XSync (desktop->display, false);
}
void ProcessWindow::expose_window_btn1 (XEvent *event) {
	XClearWindow (desktop->display, processEvent->processClient.myWindow->Get_btn1 ());
	XDrawLine (desktop->display, processEvent->processClient.myWindow->Get_btn1 (),
			desktop->Get_gc (), 1, 1, 11, 11);
	XDrawLine (desktop->display, processEvent->processClient.myWindow->Get_btn1 (),
			desktop->Get_gc (), 11, 1, 1, 11);
	XSync (desktop->display, false);
}
void ProcessWindow::expose_window_btn2 (XEvent *event) {
	XClearWindow (desktop->display, processEvent->processClient.myWindow->Get_btn2 ());
	XDrawLine (desktop->display, processEvent->processClient.myWindow->Get_btn2 (),
			desktop->Get_gc (), 1, 2, 11, 2);
	XSync (desktop->display, false);
}
void ProcessWindow::expose_window_btn3 (XEvent *event) {
	XClearWindow (desktop->display, processEvent->processClient.myWindow->Get_btn3 ());
	XDrawRectangle (desktop->display, processEvent->processClient.myWindow->Get_btn3 (),
			desktop->Get_gc (), 2, 2, 9, 9);
	XSync (desktop->display, false);
}
void ProcessWindow::expose_window_btn5 (XEvent *event) {
	XClearWindow (desktop->display, processEvent->processClient.myWindow->Get_btn5 ());
	/*
	XDrawLine (desktop->display, processEvent->processClient.myWindow->Get_btn5 (),
			desktop->Get_gc (), 16, 4, 4, 16);
			*/
	XDrawLine (desktop->display, processEvent->processClient.myWindow->Get_btn5 (),
			desktop->Get_gc (), 16, 4, 6, 4);
	XDrawLine (desktop->display, processEvent->processClient.myWindow->Get_btn5 (),
			desktop->Get_gc (), 16, 4, 16, 14);
	XDrawLine (desktop->display, processEvent->processClient.myWindow->Get_btn5 (),
			desktop->Get_gc (), 4, 16, 4, 10);
	XDrawLine (desktop->display, processEvent->processClient.myWindow->Get_btn5 (),
			desktop->Get_gc (), 4, 16, 10, 16);
	XSync (desktop->display, false);
}
void ProcessWindow::expose_desktop_launchbutton (XEvent *event) {
	XClearWindow (desktop->display, desktop->Get_launchbutton ());
	XDrawString (desktop->display, desktop->Get_launchbutton (),
			desktop->Get_gc (), 2, 18, "launch", 6);
	XSync (desktop->display, false);
}
void ProcessWindow::expose_desktop_xtermbutton (XEvent *event) {
	XClearWindow (desktop->display, desktop->Get_xtermbutton ());
	XDrawString (desktop->display, desktop->Get_xtermbutton (),
			desktop->Get_gc (), 2, 18, "xterm", 5);
	XSync (desktop->display, false);
}
void ProcessWindow::motionNotify_desktop_desktop (XEvent *event) {
	XPointerMovedEvent *ev = &(event->xmotion);
	MyWindow *temp = processEvent->currentClient.myWindow;
	if (processEvent->currentClient.type == enum_window_topbar) {
		if (ev->y <= 2) {
			if (ev->x <= -desktop->Get_x () + desktop->Get_taskbarWidth ()) {
				temp->x = -desktop->Get_x ();
				temp->y = 0;
				temp->width = desktop->Get_width () / 2;
				temp->height = desktop->Get_height () - desktop->Get_topbarHeight ();
				temp->MapWindow ();
				temp->ismodel = true;
				temp->isicon = false;
				temp->ismax = false;
				temp->isx = -desktop->Get_x ();
				return;
			}
			else if (ev->x > -desktop->Get_x () + desktop->Get_taskbarWidth () + desktop->Get_timebarWidth ()) {
				temp->x = -desktop->Get_x () + desktop->Get_width () / 2;
				temp->y = 0;
				temp->width = desktop->Get_width () / 2;
				temp->height = desktop->Get_height () - desktop->Get_topbarHeight ();
				temp->MapWindow ();
				temp->ismodel = true;
				temp->isicon = false;
				temp->ismax = false;
				temp->isx = -desktop->Get_x () + desktop->Get_width () / 2;
				return;
			}
			else {
				temp->x = -desktop->Get_x ();
				temp->y = 0;
				temp->width = desktop->Get_width ();
				temp->height = desktop->Get_height () - desktop->Get_topbarHeight ();
				temp->MapWindow ();
				temp->ismax = true;
				temp->isicon = false;
				temp->ismodel = false;
				temp->isx = -desktop->Get_x ();
				return;
			}
		}
		else {
			temp->x = ev->x - temp->motionx;
			temp->y = ev->y - temp->motiony;
			temp->width = temp->oldWidth;
			temp->height = temp->oldHeight;
			temp->MapWindow ();
			temp->ismodel = false;
			temp->ismax = false;
			temp->isicon = false;
			return;
		}
	}
	else if (processEvent->currentClient.type == enum_window_btn5) {
		temp->x = temp->oldx;
		temp->y = temp->oldy + (ev->y - temp->motiony);
		temp->width = temp->oldWidth + ev->x - temp->motionx;
		temp->height = temp->oldHeight - (ev->y - temp->motiony);
		if (temp->width < 30 || temp->width > 5000)
			temp->width = 30;
		if (temp->height < 30 || temp->height > 2000)
			temp->height = 30;
		temp->MapWindow ();
		return;
	}
}
void ProcessWindow::motionNotify_desktop_root (XEvent *event) {
	desktop->MoveDesktop (event);
}
//--------------buttonPress buttonRelease
void ProcessWindow::buttonPress_window_topbar (XEvent *event) {
	if (event->xbutton.button != Button1)
		return;
	processEvent->currentClient = processEvent->processClient; //--save current
	processEvent->processClient.type = -1;
	processEvent->TopWindow ();

	processEvent->currentClient.myWindow->motionx = event->xbutton.x;
	processEvent->currentClient.myWindow->motiony = event->xbutton.y;
	if (processEvent->currentClient.myWindow->ismodel 
			|| processEvent->currentClient.myWindow->ismax)
		processEvent->currentClient.myWindow->motionx 
			= processEvent->currentClient.myWindow->oldWidth / 2;
	XUngrabPointer (desktop->display, CurrentTime);
	XGrabPointer (desktop->display, desktop->Get_desktop (), false, ButtonReleaseMask
			| PointerMotionMask, GrabModeAsync, GrabModeAsync, desktop->Get_desktop (),
			None, CurrentTime);
	XSync (desktop->display, false);
}
void ProcessWindow::buttonPress_window_btn5 (XEvent *event) {
	if (event->xbutton.button != Button1)
		return;
	processEvent->currentClient = processEvent->processClient; //--save current
	processEvent->processClient.type = -1;
	processEvent->currentClient.myWindow->motionx = event->xbutton.x + 
		(processEvent->currentClient.myWindow->oldWidth - 
		 processEvent->currentClient.myWindow->topbarHeight) + 
		processEvent->currentClient.myWindow->x;
	processEvent->currentClient.myWindow->motiony = event->xbutton.y + 
		processEvent->currentClient.myWindow->oldy;
	XUngrabPointer (desktop->display, CurrentTime);
	XGrabPointer (desktop->display, desktop->Get_desktop (), false, ButtonReleaseMask
			| PointerMotionMask, GrabModeAsync, GrabModeAsync,
			desktop->Get_desktop (), None, CurrentTime);
	XSync (desktop->display, false);
}
void ProcessWindow::buttonPress_window_icon (XEvent *event) {
	if (event->xbutton.button != Button1)
		return;
	processEvent->currentClient = processEvent->processClient; //--save current
	processEvent->processClient.type = -1;
}
void ProcessWindow::buttonRelease_window_icon (XEvent *event) {
	if (processEvent->currentClient.type == enum_window_icon) {
		processEvent->RestackWindows ();
	}
	processEvent->currentClient.type = -1;
}
void ProcessWindow::buttonPress_window_btn1 (XEvent *event) {
	if (event->xbutton.button != Button1)
		return;
	processEvent->currentClient = processEvent->processClient;
	processEvent->processClient.type = -1;
}
void ProcessWindow::buttonRelease_window_btn1 (XEvent *event) {
	if (event->xbutton.button == Button1) {
		if (processEvent->currentClient.type == enum_window_btn1) {
			processEvent->currentClient.myWindow->CloseWindow ();
			processEvent->currentClient.type = -1;
		}
	}
}
void ProcessWindow::buttonPress_window_btn2 (XEvent *event) {
	if (event->xbutton.button == Button1)
		processEvent->currentClient = processEvent->processClient;
	processEvent->processClient.type = -1;
}
void ProcessWindow::buttonRelease_window_btn2 (XEvent *event) {
	if (event->xbutton.button == Button1 
			&& processEvent->currentClient.type == enum_window_btn2) {
		if (processEvent->currentClient.myWindow->isicon)
			processEvent->currentClient.myWindow->ShowWindow ();
		else
			processEvent->currentClient.myWindow->HideWindow ();
	}
	processEvent->currentClient.type = -1;
}
void ProcessWindow::buttonPress_window_btn3 (XEvent *event) {
	if (event->xbutton.button == Button1)
		processEvent->currentClient = processEvent->processClient;
	processEvent->processClient.type = -1;
}
void ProcessWindow::buttonRelease_window_btn3 (XEvent *event) {
	if (event->xbutton.button == Button1 && processEvent->currentClient.type == enum_window_btn3) {
		processEvent->currentClient.myWindow->MaxWindow ();
		processEvent->TopWindow ();
	}
	processEvent->currentClient.type = -1;
}
void ProcessWindow::buttonPress_desktop_desktop (XEvent *event) {
	if (event->xbutton.button == Button1) {
		desktop->Set_motionx (event->xbutton.x);
		XUngrabPointer (desktop->display, CurrentTime);
		XGrabPointer (desktop->display, XDefaultRootWindow (desktop->display),
				false, ButtonReleaseMask | PointerMotionMask,
				GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
	}
	else if (event->xbutton.button == Button4 || event->xbutton.button == Button5) {
		desktop->MoveDesktop (event);
	}
}
void ProcessWindow::buttonRelease_desktop_desktop (XEvent *event) {
	if (event->xbutton.button == Button1) {
		MyWindow *temp = processEvent->currentClient.myWindow;
		if (!temp->ismodel && !temp->ismax) {
			temp->oldx = temp->x;
			temp->oldy = temp->y;
			temp->oldWidth = temp->width;
			temp->oldHeight = temp->height;
		}
		temp->MapWindow ();
		XResizeWindow (desktop->display, temp->Get_client (), temp->width, temp->height - temp->topbarHeight);
		XUngrabPointer (desktop->display, CurrentTime);
		XSync (desktop->display, false);
		processEvent->currentClient.type = -1;
	}
}
void ProcessWindow::buttonPress_desktop_topbar (XEvent *event) {
	if (event->xbutton.button == Button4 || event->xbutton.button == Button5)
		desktop->MoveDesktop (event);
}
void ProcessWindow::buttonRelease_desktop_root (XEvent *event) {
	XUngrabPointer (desktop->display, CurrentTime);
	XSync (desktop->display, false);
	processEvent->currentClient.type = -1;
}
void ProcessWindow::buttonPress_desktop_xtermbutton (XEvent *event) {
	processEvent->spawnF ("xterm");
}
void ProcessWindow::buttonPress_desktop_launchbutton (XEvent *event) {
	processEvent->spawnP ("./launch", "launch");
}
