#pragma once

#ifndef GLUINO_WINDOW_EVENTS_H
#define GLUINO_WINDOW_EVENTS_H

#include "common.h"

namespace Gluino {

struct WindowEvents {
	Delegate* OnShown;
	Delegate* OnHidden;
	SizeDelegate* OnResize;
	SizeDelegate* OnResizeStart;
	SizeDelegate* OnResizeEnd;
	PointDelegate* OnLocationChanged;
	IntDelegate* OnWindowStateChanged;
	Delegate* OnFocusIn;
	Delegate* OnFocusOut;
	Predicate* OnClosing;
	Delegate* OnClosed;
};

}

#endif // !GLUINO_WINDOW_EVENTS_H
