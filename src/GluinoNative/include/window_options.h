#pragma once

#ifndef GLUINO_WINDOW_OPTIONS_H
#define GLUINO_WINDOW_OPTIONS_H

namespace Gluino {

struct WindowOptions {
	wchar_t* ClassName; // Windows only
	bool IsMain;
	wchar_t* TitleW;
	char* TitleA;
	WindowStyle WindowStyle;
	WindowState WindowState;
	Size Size;
	Point Location;
};

}

#endif // !GLUINO_WINDOW_OPTIONS_H