#pragma once

#ifndef GLUINO_APP_H
#define GLUINO_APP_H

#include "utils.h"
#include "app_base.h"
#include "window.h"

#include <Windows.h>

#pragma comment(lib, "Dwmapi.lib")

#define WM_USER_INVOKE (WM_USER + 0x0002)

namespace Gluino {

class App final : public AppBase {
public:
	explicit App(HINSTANCE hInstance);
	~App() override = default;

	WindowBase* SpawnWindow(WindowOptions* options, WindowEvents* events) override;
	void DespawnWindow(WindowBase* window) override;
	void Run() override;
	void Exit() override;

	static HINSTANCE GetHInstance();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE _hInstance;
};

}

#endif // !GLUINO_APP_H