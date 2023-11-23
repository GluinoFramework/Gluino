#pragma once

#ifndef GLUINO_APP_WIN32_H
#define GLUINO_APP_WIN32_H

#include "app.h"
#include "window_win32.h"

#include <Windows.h>

namespace Gluino {

class AppWin32 final : public App {
public:
	explicit AppWin32(HINSTANCE hInstance);
	~AppWin32() override = default;

	void Register(autostr className) const;

	Window* SpawnWindow(WindowOptions* options) override;
	void DespawnWindow(Window* window) override;
	void Run() override;
	void Exit() override;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE _hInstance{};
};

}

#endif // !GLUINO_APP_H