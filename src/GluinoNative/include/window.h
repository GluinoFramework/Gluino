#pragma once

#ifndef GLUINO_WINDOW_H
#define GLUINO_WINDOW_H

#include "common.h"
#include "window_options.h"

namespace Gluino {

class Window {
public:
	explicit Window(WindowOptions* options) {
#ifdef _WIN32
		_title = CopyStr(options->TitleW);
#else
		_title = CopyStr(options->TitleA);
#endif
	}

	virtual ~Window() {
		delete[] _title;
	}

	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual void Close() = 0;
	virtual void Invoke(Action action) = 0;

	virtual void SetTitle(autostr title) = 0;

protected:
	autostr _title;
};

}

#endif // !GLUINO_WINDOW_H