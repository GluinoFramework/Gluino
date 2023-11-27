#pragma once

#ifndef GLUINO_WINDOW_EVENTS_H
#define GLUINO_WINDOW_EVENTS_H

#include "common.h"

namespace Gluino {

struct WindowEvents {
	Delegate* OnShown;
	Delegate* OnHidden;
	SizeDelegate* OnSizeChanged;
	PointDelegate* OnLocationChanged;
	Delegate* OnFocusIn;
	Delegate* OnFocusOut;
	Predicate* OnClosing;
	Delegate* OnClosed;
};

}

#endif // !GLUINO_WINDOW_EVENTS_H