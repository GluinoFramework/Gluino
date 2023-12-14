#pragma once

#ifndef GLUINO_WINDOW_BASE_H
#define GLUINO_WINDOW_BASE_H

#include "window_events.h"
#include "window_options.h"

namespace Gluino {

class WebViewBase;

class WindowBase {
public:
	explicit WindowBase(WindowOptions* options, const WindowEvents* events) {
		_isMain = options->IsMain;
#ifdef _WIN32
		_title = CopyStr(options->TitleW);
#else
		_title = CopyStr(options->TitleA);
#endif
		_icon = options->Icon;
		_iconSize = options->IconSize;
		_borderStyle = options->BorderStyle;
		_theme = options->Theme;

		_onShown = (Delegate)events->OnShown;
		_onHidden = (Delegate)events->OnHidden;
		_onResize = (SizeDelegate)events->OnResize;
		_onResizeStart = (SizeDelegate)events->OnResizeStart;
		_onResizeEnd = (SizeDelegate)events->OnResizeEnd;
		_onLocationChanged = (PointDelegate)events->OnLocationChanged;
		_onWindowStateChanged = (IntDelegate)events->OnWindowStateChanged;
		_onFocusIn = (Delegate)events->OnFocusIn;
		_onFocusOut = (Delegate)events->OnFocusOut;
		_onClosing = (Predicate)events->OnClosing;
		_onClosed = (Delegate)events->OnClosed;
	}

	virtual ~WindowBase() {
		delete[] _title;
	}

	[[nodiscard]] bool IsMain() const { return _isMain; }

	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual void Close() = 0;
	virtual void Center() = 0;
	virtual void DragMove() = 0;
	virtual void Invoke(Delegate action) = 0;

	virtual void GetBounds(Rect* bounds) = 0;
	virtual bool GetIsDarkMode() = 0;

	virtual autostr GetTitle() = 0;
	virtual void SetTitle(autostr title) = 0;

	virtual void GetIcon(void** data, int* size) = 0;
	virtual void SetIcon(void* data, int size) = 0;

	virtual WindowBorderStyle GetBorderStyle() = 0;
	virtual void SetBorderStyle(WindowBorderStyle style) = 0;

	virtual WindowState GetWindowState() = 0;
	virtual void SetWindowState(WindowState state) = 0;

	virtual WindowTheme GetTheme() = 0;
	virtual void SetTheme(WindowTheme theme) = 0;

	virtual Size GetMinimumSize() = 0;
	virtual void SetMinimumSize(Size& size) = 0;

	virtual Size GetMaximumSize() = 0;
	virtual void SetMaximumSize(Size& size) = 0;

	virtual Size GetSize() = 0;
	virtual void SetSize(Size& size) = 0;

	virtual Point GetLocation() = 0;
	virtual void SetLocation(Point& location) = 0;

	virtual bool GetMinimizeEnabled() = 0;
	virtual void SetMinimizeEnabled(bool enabled) = 0;

	virtual bool GetMaximizeEnabled() = 0;
	virtual void SetMaximizeEnabled(bool enabled) = 0;

	virtual bool GetTopMost() = 0;
	virtual void SetTopMost(bool topMost) = 0;

protected:
	bool _isMain;
	autostr _title;
	void* _icon;
	int _iconSize;
	WindowBorderStyle _borderStyle;
	WindowTheme _theme;

	Delegate _onShown;
	Delegate _onHidden;
	SizeDelegate _onResize;
	SizeDelegate _onResizeStart;
	SizeDelegate _onResizeEnd;
	PointDelegate _onLocationChanged;
	IntDelegate _onWindowStateChanged;
	Delegate _onFocusIn;
	Delegate _onFocusOut;
	Predicate _onClosing;
	Delegate _onClosed;
};

}

#endif // !GLUINO_WINDOW_H