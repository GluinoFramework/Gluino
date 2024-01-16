#pragma once

#ifndef GLUINO_WINDOW_OPTIONS_H
#define GLUINO_WINDOW_OPTIONS_H

#include "common.h"

namespace Gluino {

struct WindowOptions {
	bool IsMain;
	char* Title;
	void* Icon;
	int IconSize;
	WindowBorderStyle BorderStyle;
	WindowState WindowState;
	WindowStartupLocation StartupLocation;
	WindowTheme Theme;
	Size MinimumSize;
	Size MaximumSize;
	Size Size;
	Point Location;
	bool MinimizeEnabled;
	bool MaximizeEnabled;
	bool TopMost;
};

}

#endif // !GLUINO_WINDOW_OPTIONS_H