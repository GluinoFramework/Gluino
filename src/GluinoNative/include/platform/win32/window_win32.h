#pragma once

#ifndef GLUINO_WINDOW_WIN32_H
#define GLUINO_WINDOW_WIN32_H

#include "window.h"

#include <Windows.h>

namespace Gluino {

class WindowWin32 final : public Window {
public:
	explicit WindowWin32(WindowOptions* options);
	~WindowWin32() override;

	HWND GetHandle() const { return _hWnd; }
	void Register(autostr className) const;

	void Show() override;
	void Hide() override;
	void Close() override;
	void Invoke(Action action) override;

	autostr GetTitle() override;
	void SetTitle(autostr title) override;

private:
	wchar_t* _className;
	HWND _hWnd;
};

}

#endif // !GLUINO_WINDOW_WIN32_H

