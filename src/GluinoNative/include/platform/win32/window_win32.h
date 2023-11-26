#pragma once

#ifndef GLUINO_WINDOW_WIN32_H
#define GLUINO_WINDOW_WIN32_H

#include "window.h"

#include <Windows.h>

namespace Gluino {

class WindowWin32 final : public Window {
public:
	explicit WindowWin32(WindowOptions* options, const WindowEvents* events);
	~WindowWin32() override;

	HWND GetHandle() const { return _hWnd; }
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void Show() override;
	void Hide() override;
	void Close() override;
	void Invoke(Delegate action) override;

	void GetBounds(Rect* bounds) override;

	autostr GetTitle() override;
	void SetTitle(autostr title) override;
	

private:
	wchar_t* _className;
	HWND _hWnd;
};

}

#endif // !GLUINO_WINDOW_WIN32_H

