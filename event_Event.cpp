#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xproto.h>
#include "window.h"
//-----------ProcessEvent
vector<MyWindow *> ProcessEvent::clients = vector<MyWindow *> ();
vector<Window> ProcessEvent::stacks = vector<Window> ();
ProcessClient ProcessEvent::processClient = ProcessClient ();
ProcessClient ProcessEvent::currentClient = ProcessClient ();
Desktop* ProcessEvent::desktop = Desktop::GetDesktop ();
ProcessWindow* ProcessEvent::processWindow = ProcessWindow::GetProcessWindow ();
//-----------------------
ProcessEvent* ProcessEvent::GetProcessEvent () {
	static ProcessEvent processEvent;
	return &processEvent;
}
ProcessEvent::ProcessEvent () {
	handlerEvent[EnterNotify] = &ProcessEvent::enterNotify;
	handlerEvent[LeaveNotify] = &ProcessEvent::leaveNotify;
	handlerEvent[Expose] = &ProcessEvent::expose;
	handlerEvent[MotionNotify] = &ProcessEvent::motionNotify;
	handlerEvent[ConfigureRequest] = &ProcessEvent::configureRequest;
	handlerEvent[MapRequest] = &ProcessEvent::mapRequest;
	handlerEvent[KeyPress] = &ProcessEvent::keyPress;
	handlerEvent[ButtonPress] = &ProcessEvent::buttonPress;
	handlerEvent[ButtonRelease] = &ProcessEvent::buttonRelease;
	handlerEvent[DestroyNotify] = &ProcessEvent::destroyNotify;
	handlerEvent[MappingNotify] = &ProcessEvent::mappingNotify;
}
//=======================
void ProcessEvent::expose (XEvent *event) {
	FindClient (event->xexpose.window);
	if (processClient.type == -1) {
		cerr << "ERROR: expose --no search client!" << endl;
		return;
		//exit (1);
	}
	if (processWindow->handlerWindow[Expose][processClient.type])
		processWindow->handlerWindow[Expose][processClient.type] (event);
}
void ProcessEvent::motionNotify (XEvent *event) {
	FindClient (event->xmotion.window);
	if (processClient.type == -1) {
		cerr << "ERROR: MotionNotify --no search client" << endl;
		return;
		//exit (1);
	}
	if (processWindow->handlerWindow[MotionNotify][processClient.type])
		processWindow->handlerWindow[MotionNotify][processClient.type] (event);
}
void ProcessEvent::buttonPress (XEvent *event) {
	FindClient (event->xbutton.window);
	if (processClient.type == -1) {
		cerr << "ERROR: ButtonPress --no search client" << endl;
		return;
		//exit (1);
	}
	if (!processClient.myWindow->isicon && processClient.type == enum_window_topbar) {
		XSetInputFocus (desktop->display, processClient.myWindow->Get_client (), RevertToPointerRoot, CurrentTime);
	}
	else
		XSetInputFocus (desktop->display, PointerRoot, RevertToPointerRoot, CurrentTime);
	if (processWindow->handlerWindow[ButtonPress][processClient.type])
		processWindow->handlerWindow[ButtonPress][processClient.type] (event);
}
void ProcessEvent::buttonRelease (XEvent *event) {
	FindClient (event->xbutton.window);
	if (processClient.type == -1) {
		cerr << "ERROR: buttonrelease --no search client" << endl;
		return;
	}
	if (processWindow->handlerWindow[ButtonRelease][processClient.type])
		processWindow->handlerWindow[ButtonRelease][processClient.type] (event);
}
//-----------------------------
void ProcessEvent::enterNotify (XEvent *event) {
	FindClient (event->xbutton.window);
	if (processClient.type == -1) {
		cerr << "ERROR: buttonrelease --no search client" << endl;
		return;
	}
	//----------
	if (processClient.myWindow->Get_content () && !processClient.myWindow->isicon)
		XSetInputFocus (desktop->display, processClient.myWindow->Get_content (), RevertToPointerRoot, CurrentTime);
}
void ProcessEvent::leaveNotify (XEvent *event) {
}
void ProcessEvent::destroyNotify (XEvent *event) {
	FindClient (event->xdestroywindow.window);
	if (processClient.type == -1) {
		cerr << "ERROR: destroynotify --no search client" << endl;
		return;
		//exit (1);
	}
	DeleteClient ();
	XUngrabKey (desktop->display, AnyKey, AnyModifier, XDefaultRootWindow (desktop->display));
	XSync (desktop->display, false);
	XSetInputFocus (desktop->display, PointerRoot, RevertToPointerRoot, CurrentTime);
	desktop->ManageIcon ();
	XDestroyWindow (desktop->display, processClient.myWindow->Get_icon ());
	XDestroyWindow (desktop->display, processClient.myWindow->Get_root ());
	XSync (desktop->display, false);
	delete processClient.myWindow;
	return;
}
void ProcessEvent::configureRequest (XEvent *event) {
	desktop->Configure (event);
}
void ProcessEvent::mapRequest (XEvent *event) {
	desktop->Map (event);
	desktop->ManageIcon ();
}
void ProcessEvent::keyPress (XEvent *event) {
	KeySym keysym = XLookupKeysym (&(event->xkey), 0);
	if (keysym == XK_q && event->xkey.state == Mod1Mask | ShiftMask)
		exit (1);
	if (keysym == XK_Return && event->xkey.state == Mod1Mask | ShiftMask)
		spawnF ("xterm");
}
//=======================================
void ProcessEvent::FindClient (Window window) {
	if (!window) {
		cerr << "ERROR: FindClient --window is null!" << endl;
		return;
		//exit (1);
	}
	if (window == desktop->Get_desktop ()) {
		processClient.window = window;
		processClient.type = enum_desktop_desktop;
		return;
	}
	if (window == desktop->Get_topbar ()) {
		processClient.window = window;
		processClient.type = enum_desktop_topbar;
		return;
	}
	if (window == desktop->Get_taskbar ()) {
		processClient.window = window;
		processClient.type = enum_desktop_taskbar;
		return;
	}
	if (window == desktop->Get_timebar ()) {
		processClient.window = window;
		processClient.type = enum_desktop_timebar;
		return;
	}
	if (window == desktop->Get_systembar ()) {
		processClient.window = window;
		processClient.type = enum_desktop_systembar;
		return;
	}
	if (window == desktop->Get_launchbutton ()) {
		processClient.window = window;
		processClient.type = enum_desktop_launchbutton;
		return;
	}
	if (window == desktop->Get_xtermbutton ()) {
		processClient.window = window;
		processClient.type = enum_desktop_xtermbutton;
		return;
	}
	if (window == XDefaultRootWindow (desktop->display)) {
		processClient.window = window;
		processClient.type = enum_desktop_root;
		return;
	}
	for (vector<MyWindow *>::iterator iter = clients.begin(); iter != clients.end(); ++iter)
	{
		if (window == (*iter)->Get_topbar ()) {
			processClient.myWindow = (*iter);
			processClient.type = enum_window_topbar;
			return;
		}
		if (window == (*iter)->Get_content ()) {
			processClient.myWindow = (*iter);
			processClient.type = enum_window_content;
			return;
		}
		if (window == (*iter)->Get_btn1 ()) {
			processClient.myWindow = (*iter);
			processClient.type = enum_window_btn1;
			return;
		}
		if (window == (*iter)->Get_btn2 ()) {
			processClient.myWindow = (*iter);
			processClient.type = enum_window_btn2;
			return;
		}
		if (window == (*iter)->Get_btn3 ()) {
			processClient.myWindow = (*iter);
			processClient.type = enum_window_btn3;
			return;
		}
		if (window == (*iter)->Get_btn5 ()) {
			processClient.myWindow = (*iter);
			processClient.type = enum_window_btn5;
			return;
		}
		if (window == (*iter)->Get_client ()) {
			processClient.myWindow = (*iter);
			processClient.type = enum_window_client;
			return;
		}
		if (window == (*iter)->Get_root ()) {
			processClient.myWindow = (*iter);
			processClient.type = enum_window_root;
			return;
		}
		if (window == (*iter)->Get_icon ()) {
			processClient.myWindow = (*iter);
			processClient.type = enum_window_icon;
			return;
		}
	}
	processClient.type = -1;
	return;
}
void ProcessEvent::DeleteClient () {
	for (vector<Window>::iterator iter = stacks.begin(); iter!= stacks.end(); ++iter) {
		if ((*iter) == processClient.myWindow->Get_root ()) {
			iter = stacks.erase (iter);
			break;
		}
	}
	for (vector<MyWindow *>::iterator iter = clients.begin(); iter != clients.end();
			++iter) {
		if ((*iter) == processClient.myWindow) {
			iter = clients.erase (iter);
			return;
		}
	}
}
void ProcessEvent::spawnF (const char *arg) {
	if(fork() == 0){
		setsid();
		cerr << "success open client" << endl;
		execlp (arg, arg, NULL);
		cerr << "open client error!!!!!!!!!!!!" << endl;
		exit(EXIT_SUCCESS);
	}
}
void ProcessEvent::spawnP (const char *path, const char *name) {
	if(fork() == 0){
		setsid();
		cerr << "success open client" << endl;
		execl (path, name, "", NULL);
		cerr << "open client error!!!!!!!!!!!!" << endl;
		exit(EXIT_SUCCESS);
	}
}
void ProcessEvent::TopWindow () {
	if (stacks.size() <= 1)
		return;
	Window top;
	for (vector<Window>::iterator iter = stacks.begin(); iter != stacks.end(); ++iter) {
		if ((*iter) == currentClient.myWindow->Get_root ()) {
			top = (*iter);
			iter = stacks.erase (iter);
			break;
		}
	}
	if (top)
		stacks.push_back (top);
	Window *stack = new Window[stacks.size()];
	for (unsigned int i = 0; i < stacks.size(); ++i)
		stack[i] = stacks[stacks.size() - i - 1];
	XRestackWindows (desktop->display, stack, stacks.size());
	delete[] stack;
	XSync (desktop->display, false);
}
void ProcessEvent::RestackWindows () {
	if (stacks.size() <= 1) {
		if (currentClient.myWindow->isicon)
			currentClient.myWindow->ShowWindow ();
		else
			currentClient.myWindow->HideWindow ();
		return;
	}
	if (currentClient.myWindow->isicon) {
		TopWindow ();
		currentClient.myWindow->ShowWindow ();
	}
	else {
		if (currentClient.myWindow->Get_root () == stacks[stacks.size()-1]) {
			swap (stacks[stacks.size()-1], stacks[stacks.size()-2]);
			currentClient.myWindow->HideWindow ();
		}
		else {
			TopWindow ();
		}
	}
}
void ProcessEvent::mappingNotify (XEvent *event) {
	grabkeys ();
}
void ProcessEvent::grabkeys () {
	XUngrabKey (desktop->display, AnyKey, AnyModifier, 
			XRootWindow (desktop->display, desktop->screenNum));
	KeyCode code;
	code = XKeysymToKeycode (desktop->display, XK_Return);
	XGrabKey (desktop->display, code, Mod1Mask | ShiftMask, 
			desktop->Get_desktop (), True, GrabModeAsync, GrabModeAsync);
	code = XKeysymToKeycode (desktop->display, XK_q);
	XGrabKey (desktop->display, code, Mod1Mask | ShiftMask, 
			desktop->Get_desktop (), True, GrabModeAsync, GrabModeAsync);
	XSync (desktop->display, false);
}
