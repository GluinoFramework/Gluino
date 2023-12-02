#pragma once

#ifndef GLUINO_WINDOW_OPTIONS_H
#define GLUINO_WINDOW_OPTIONS_H

#include "common.h"

namespace Gluino {

struct WindowOptions {
	wchar_t* ClassName; // Windows only
	bool IsMain;
	wchar_t* TitleW;
	char* TitleA;
	WindowBorderStyle BorderStyle;
	WindowState WindowState;
	Size MinimumSize;
	Size MaximumSize;
	Size Size;
	Point Location;
	WindowStartupLocation StartupLocation;
	bool TopMost;
};

}

#endif // !GLUINO_WINDOW_OPTIONS_H