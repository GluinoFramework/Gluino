#pragma once

#ifndef GLUINO_WINDOW_OPTIONS_H
#define GLUINO_WINDOW_OPTIONS_H

namespace Gluino {

struct WindowOptions {
	wchar_t* TitleW;
	char* TitleA;
	int Width;
	int Height;
	int X;
	int Y;
};

}

#endif // !GLUINO_WINDOW_OPTIONS_H