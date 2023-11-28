#include "app.h"

#include <dwmapi.h>
#include <future>
#include <map>

#pragma comment(lib, "Dwmapi.lib")

using namespace Gluino;

App* appWin32{};
std::map<HWND, Window*> windowMap{};

App::App(const HINSTANCE hInstance) {
	_hInstance = hInstance;
	appWin32 = this;
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
	return appWin32->_hInstance;
}

void ApplyMica(const HWND hWnd, bool enabled) {
	constexpr MARGINS margins = { -1, -1, -1, -1 };
	DwmExtendFrameIntoClientArea(hWnd, &margins);

	constexpr BOOL mode = TRUE;
	DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &mode, sizeof mode);

	constexpr DWM_SYSTEMBACKDROP_TYPE value = DWMSBT_MAINWINDOW;
	DwmSetWindowAttribute(hWnd, DWMWA_SYSTEMBACKDROP_TYPE, &value, sizeof value);

	constexpr BOOL bkdbrush = TRUE;
	DwmSetWindowAttribute(hWnd, DWMWA_USE_HOSTBACKDROPBRUSH, &bkdbrush, sizeof bkdbrush);
}

LRESULT App::WndProc(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
	const auto window = windowMap[hWnd];

	switch (msg) {
		case WM_CREATE: {
			ApplyMica(hWnd, true);
			break;
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			const HDC hdc = BeginPaint(hWnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(hWnd, &ps);
			return 0;
		}
		case WM_DESTROY: {
			if (windowMap.find(hWnd) != windowMap.end())
				appWin32->DespawnWindow(windowMap[hWnd]);
			break;
		}
		case WM_USER_INVOKE: {
			const auto callback = (Delegate)wParam;
			callback();

			const auto promise = (std::promise<void>*)lParam;
			promise->set_value();
			return 0;
		}
		default: {
			if (window)
				return window->WndProc(msg, wParam, lParam);
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
