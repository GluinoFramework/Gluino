#pragma once

#ifndef GLUINO_WINDOW_WIN32_H
#define GLUINO_WINDOW_WIN32_H

#include "window_base.h"

#include <Windows.h>
#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

namespace Gluino {

class Window final : public WindowBase {
public:
	explicit Window(WindowOptions* options, const WindowEvents* events);
	~Window() override;

	static bool IsCompositionEnabled() {
		BOOL enabled = FALSE;
		const auto success = DwmIsCompositionEnabled(&enabled) == S_OK;
		return enabled && success;
	}

	HWND GetHandle() const { return _hWnd; }
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void AdjustMaximizedClientRect(RECT& rect);
	void SetBorderlessStyle(bool borderless) const;

	void Show() override;
	void Hide() override;
	void Close() override;
	void Invoke(Delegate action) override;

	void GetBounds(Rect* bounds) override;

	autostr GetTitle() override;
	void SetTitle(autostr title) override;

	WindowStyle GetWindowStyle() override;
	void SetWindowStyle(WindowStyle style) override;
	
	WindowState GetWindowState() override;
	void SetWindowState(WindowState state) override;
private:
	wchar_t* _className;
	HWND _hWnd;
};

}

#endif // !GLUINO_WINDOW_WIN32_H

