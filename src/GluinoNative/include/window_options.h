#pragma once

#ifndef GLUINO_WINDOW_OPTIONS_H
#define GLUINO_WINDOW_OPTIONS_H

#include "common.h"

namespace Gluino {

struct WindowOptions {
	bool IsMain;
	wchar_t* TitleW;
	char* TitleA;
	WindowBorderStyle BorderStyle;
	WindowState WindowState;
	WindowStartupLocation StartupLocation;
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