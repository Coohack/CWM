#ifndef _EVENT_H_
#define _EVENT_H_
#include <iostream>
#include <string.h>
#include <vector>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
using namespace std;
enum {enum_desktop_root, enum_desktop_desktop, enum_desktop_topbar, enum_desktop_taskbar, 
	enum_desktop_timebar, enum_desktop_systembar,
	enum_desktop_launchbutton, enum_desktop_xtermbutton,
	enum_window_root, enum_window_topbar, enum_window_content, enum_window_btn1,
	enum_window_btn2, enum_window_btn3, enum_window_btn5,
	enum_window_client, enum_window_icon,
	enum_last};
//------------class
class Desktop;
class MyWindow;
class ProcessEvent;
class ProcessWindow;

struct ProcessClient {
	union {
		MyWindow *myWindow;
		Window window;
	};
	int type;
};
//----------------
class Desktop {
	public:
		Display *display;
		int screenNum;
		static ProcessEvent* processEvent;
		//------------------
		void MapDesktop ();
		void MoveDesktop (XEvent *event);
		void Configure (XEvent *event);
		void Map (XEvent *event);
		void ManageIcon ();

		~Desktop () {}
		static Desktop* GetDesktop ();
		//---------get fields
		inline Window Get_desktop () { return desktop;}
		inline Window Get_topbar () { return topbar;}
		inline Window Get_taskbar () { return taskbar;}
		inline Window Get_timebar () { return timebar;}
		inline Window Get_systembar () { return systembar;}
		inline Window Get_launchbutton () { return launchbutton;}
		inline Window Get_xtermbutton () { return xtermbutton;}
		inline GC Get_gc () { return gc;}
		inline unsigned int Get_width () { return width;}
		inline unsigned int Get_height () { return height;}
		inline unsigned int Get_topbarHeight () { return topbarHeight;}
		inline unsigned int Get_taskbarWidth () { return taskbarWidth;}
		inline unsigned int Get_timebarWidth () { return timebarWidth;}
		inline unsigned int Get_systembarWidth () { return systembarWidth;}
		inline unsigned int Get_buttonWidth () { return buttonWidth;}
		inline unsigned int Get_x () { return x;}
		inline int Get_motionx () { return motionx;}
		inline void Set_motionx (int mx) {this->motionx = mx;}
	protected:
		Desktop ();
	private:
		Window desktop;
		Window topbar;
		Window taskbar;
		Window timebar;
		Window systembar;
		Window launchbutton;
		Window xtermbutton;
		Cursor cursor;
		GC gc;

		unsigned int width;
		unsigned int height;
		unsigned int topbarHeight;
		unsigned int taskbarWidth;
		unsigned int timebarWidth;
		unsigned int systembarWidth;
		unsigned int buttonWidth;
		int motionx, x;
};
class MyWindow {
	public:
		static Desktop* desktop;
		static ProcessEvent* processEvent;

		int x, y, oldx, oldy, motionx, motiony;
		unsigned int width, height, oldWidth, oldHeight;
		unsigned int topbarHeight;
		bool ismodel;
		bool isicon;
		bool ismax;
		int isx;
		//---------
		~MyWindow () {}
		MyWindow (int x, int y, unsigned int w, unsigned int h, Window client);
		void MapWindow ();
		void MapWindowBack ();
		void ShowWindow ();
		void HideWindow ();
		void CloseWindow ();
		void MaxWindow ();
		//----------get fields
		inline char* Get_name () { return name;}
		inline void Set_name (const char *n) { strcpy (name, n); }
		inline Window Get_root () { return root;}
		inline Window Get_topbar () { return topbar;}
		inline Window Get_content () { return content;}
		inline Window Get_btn1 () { return btn1;}
		inline Window Get_btn2 () { return btn2;}
		inline Window Get_btn3 () { return btn3;}
		inline Window Get_btn5 () { return btn5;}
		inline Window Get_icon () { return icon;}
		inline Window Get_client () { return client;}
	private:
		char name[256];
		Window root;
		Window topbar;
		Window content;
		Window btn1;
		Window btn2;
		Window btn3;
		Window btn5;
		Window icon;
		Window client;
};
class ProcessEvent {
	public:
		static Desktop* desktop;
		static ProcessWindow* processWindow;
		static vector<MyWindow *> clients;
		static vector<Window> stacks;
		static ProcessClient processClient;
		static ProcessClient currentClient;
		void (*handlerEvent[LASTEvent]) (XEvent *);
		//-------
		~ProcessEvent () {}
		static ProcessEvent* GetProcessEvent ();
		static void FindClient (Window window);
		static void DeleteClient ();
		static void RestackWindows ();
		static void TopWindow ();
		static void spawnF (const char *arg);
		static void spawnP (const char *path, const char *name);
		//-------
		static void enterNotify (XEvent *event);
		static void leaveNotify (XEvent *event);
		static void expose (XEvent *event);
		static void motionNotify (XEvent *event);
		static void configureRequest (XEvent *event);
		static void mapRequest (XEvent *event);
		static void keyPress (XEvent *event);
		static void buttonPress (XEvent *event);
		static void buttonRelease (XEvent *event);
		static void destroyNotify (XEvent *event);
		static void mappingNotify (XEvent *event);
		static void grabkeys ();
	protected:
		ProcessEvent ();
};
class ProcessWindow {
	public:
		static ProcessEvent* processEvent;
		static Desktop* desktop;
		void (*handlerWindow[LASTEvent][enum_last]) (XEvent *event);
		//-------
		~ProcessWindow () {}
		static ProcessWindow* GetProcessWindow ();
		//-------expose motionNotify
		static void expose_window_topbar (XEvent *event);
		static void expose_window_icon (XEvent *event);
		static void expose_window_btn1 (XEvent *event);
		static void expose_window_btn2 (XEvent *event);
		static void expose_window_btn3 (XEvent *event);
		static void expose_window_btn5 (XEvent *event);
		static void expose_desktop_launchbutton (XEvent *event);
		static void expose_desktop_xtermbutton (XEvent *event);
		static void motionNotify_desktop_desktop (XEvent *event);
		static void motionNotify_desktop_root (XEvent *event);
		//-------buttonPress buttonRelease
		static void buttonPress_window_topbar (XEvent *event);
		static void buttonPress_window_btn5 (XEvent *event);
		static void buttonPress_window_icon (XEvent *event);
		static void buttonRelease_window_icon (XEvent *event);
		static void buttonPress_window_btn1 (XEvent *event);
		static void buttonRelease_window_btn1 (XEvent *event);
		static void buttonPress_window_btn2 (XEvent *event);
		static void buttonRelease_window_btn2 (XEvent *event);
		static void buttonPress_window_btn3 (XEvent *event);
		static void buttonRelease_window_btn3 (XEvent *event);
		static void buttonPress_desktop_desktop (XEvent *event);
		static void buttonRelease_desktop_desktop (XEvent *event);
		static void buttonPress_desktop_topbar (XEvent *event);
		static void buttonRelease_desktop_root (XEvent *event);
		static void buttonPress_desktop_xtermbutton (XEvent *event);
		static void buttonPress_desktop_launchbutton (XEvent *event);
	protected:
		ProcessWindow ();
};
#endif
