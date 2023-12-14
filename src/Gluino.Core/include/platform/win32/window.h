#pragma once

#ifndef GLUINO_WINDOW_H
#define GLUINO_WINDOW_H

#include "window_base.h"
#include "window_frame.h"

#undef min
#undef max

namespace Gluino {

class WebView;

class Window final : public WindowBase {
public:
	explicit Window(WindowOptions* options, const WindowEvents* events, WebView* webView);
	~Window() override;

	HWND GetHandle() const { return _hWnd; }
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void Show() override;
	void Hide() override;
	void Close() override;
	void Center() override;
	void DragMove() override;
	void Invoke(Delegate action) override;

	void GetBounds(Rect* bounds) override;
	bool GetIsDarkMode() override;

	autostr GetTitle() override;
	void SetTitle(autostr title) override;

	WindowBorderStyle GetBorderStyle() override;
	void SetBorderStyle(WindowBorderStyle style) override;
	
	WindowState GetWindowState() override;
	void SetWindowState(WindowState state) override;

	WindowTheme GetTheme() override;
	void SetTheme(WindowTheme theme) override;

	Size GetMinimumSize() override;
	void SetMinimumSize(Size& size) override;

	Size GetMaximumSize() override;
	void SetMaximumSize(Size& size) override;

	Size GetSize() override;
	void SetSize(Size& size) override;

	Point GetLocation() override;
	void SetLocation(Point& location) override;

	bool GetMinimizeEnabled() override;
	void SetMinimizeEnabled(bool enabled) override;

	bool GetMaximizeEnabled() override;
	void SetMaximizeEnabled(bool enabled) override;

	bool GetTopMost() override;
	void SetTopMost(bool topMost) override;

private:
	HWND _hWnd;
	WindowFrame* _frame;
	bool _isResizing;

	WindowState _windowState;
	Size _minSize;
	Size _maxSize;
	bool _minimizeEnabled;
	bool _maximizeEnabled;

	WebView* _webView;
};

}

#endif // !GLUINO_WINDOW_H

