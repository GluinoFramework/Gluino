#pragma once

#ifndef GLUINO_WINDOW_H
#define GLUINO_WINDOW_H

#include "window_events.h"
#include "window_options.h"

namespace Gluino {

class Window {
public:
	explicit Window(WindowOptions* options, const WindowEvents* events) {
		_isMain = options->IsMain;
#ifdef _WIN32
		_title = CopyStr(options->TitleW);
#else
		_title = CopyStr(options->TitleA);
#endif
		_style = options->WindowStyle;

		_onShown = (Delegate)events->OnShown;
		_onHidden = (Delegate)events->OnHidden;
		_onSizeChanged = (SizeDelegate)events->OnSizeChanged;
		_onLocationChanged = (PointDelegate)events->OnLocationChanged;
		_onFocusIn = (Delegate)events->OnFocusIn;
		_onFocusOut = (Delegate)events->OnFocusOut;
		_onClosing = (Predicate)events->OnClosing;
		_onClosed = (Delegate)events->OnClosed;
	}

	virtual ~Window() {
		delete[] _title;
	}

	bool IsMain() const { return _isMain; }

	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual void Close() = 0;
	virtual void Invoke(Delegate action) = 0;

	virtual void GetBounds(Rect* bounds) = 0;

	virtual autostr GetTitle() = 0;
	virtual void SetTitle(autostr title) = 0;

	virtual WindowStyle GetWindowStyle() = 0;
	virtual void SetWindowStyle(WindowStyle style) = 0;

	virtual WindowState GetWindowState() = 0;
	virtual void SetWindowState(WindowState state) = 0;

protected:
	bool _isMain;
	autostr _title;
	WindowStyle _style;

	Delegate _onShown;
	Delegate _onHidden;
	SizeDelegate _onSizeChanged;
	PointDelegate _onLocationChanged;
	Delegate _onFocusIn;
	Delegate _onFocusOut;
	Predicate _onClosing;
	Delegate _onClosed;
};

}

#endif // !GLUINO_WINDOW_H