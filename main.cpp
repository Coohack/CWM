#include <iostream>
#include <stdlib.h>
#include "window.h"
using namespace std;
int main (int argc, char **argv) {
	Desktop *desktop = Desktop::GetDesktop ();
	desktop->MapDesktop ();
	ProcessEvent *processEvent = ProcessEvent::GetProcessEvent ();
	XEvent event;
	while (1) {
		XNextEvent (desktop->display, &event);
		if (processEvent->handlerEvent[event.type]) {
			processEvent->handlerEvent[event.type] (&event);
		}
	}
	XCloseDisplay (desktop->display);
	return 0;
}
