#include "app.h"

#include <dwmapi.h>
#include <future>
#include <iostream>
#include <map>

using namespace Gluino;

App* app{};
std::map<HWND, Window*> windowMap{};

App::App(const HINSTANCE hInstance) {
	_hInstance = hInstance;
	app = this;
}

WindowBase* App::SpawnWindow(WindowOptions* options, WindowEvents* events) {
	const auto window = new Window(options, events);
	windowMap[window->GetHandle()] = window;
	return window;
}

void App::DespawnWindow(WindowBase* window) {
	const auto win32Window = (Window*)window;
	const HWND hWnd = win32Window->GetHandle();

	WCHAR className[256];
	GetClassName(hWnd, className, 256);

	UnregisterClass(className, _hInstance);
	windowMap.erase(hWnd);

	if (window->IsMain())
		Exit();
}

void App::Run() {
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void App::Exit() {
	PostQuitMessage(0);
}

HINSTANCE App::GetHInstance() {
	return app->_hInstance;
}

LRESULT App::WndProc(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
	const auto window = windowMap[hWnd];

	switch (msg) {
		case WM_DESTROY: {
			if (windowMap.contains(hWnd))
				app->DespawnWindow(windowMap[hWnd]);
			break;
		}
		case WM_USER_INVOKE: {
			const auto callback = (Delegate)wParam;
			callback();

			const auto promise = (std::promise<void>*)lParam;
			promise->set_value();
			return 0;
		}
		default: break;
	}

	if (window)
		return window->WndProc(msg, wParam, lParam);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
