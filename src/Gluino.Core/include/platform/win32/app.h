#pragma once

#ifndef GLUINO_APP_H
#define GLUINO_APP_H

#include "app_base.h"
#include "window.h"
#include "webview.h"

#include <Windows.h>

#pragma comment(lib, "Dwmapi.lib")

#define WM_USER_INVOKE (WM_USER + 0x0002)

namespace Gluino {

class App final : public AppBase {
public:
	explicit App(HINSTANCE hInstance, wchar_t* appId);
	~App() override;

	void SpawnWindow(
		WindowOptions* windowOptions, WindowEvents* windowEvents,
		WebViewOptions* webViewOptions, WebViewEvents* webViewEvents,
		WindowBase** window, WebViewBase** webView) override;
	void DespawnWindow(WindowBase* window) override;
	void Run() override;
	void Exit() override;

	static HINSTANCE GetHInstance();
	static wchar_t* GetWndClassName();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE _hInstance;
	wchar_t* _appId;
	wchar_t* _wndClassName;
};

}

#endif // !GLUINO_APP_H