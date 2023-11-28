#include "app_win32.h"

#include <dwmapi.h>
#include <future>
#include <map>

#pragma comment(lib, "Dwmapi.lib")

using namespace Gluino;

AppWin32* appWin32{};
std::map<HWND, WindowWin32*> windowMap{};

AppWin32::AppWin32(const HINSTANCE hInstance) {
	_hInstance = hInstance;
	appWin32 = this;
}

Window* AppWin32::SpawnWindow(WindowOptions* options, WindowEvents* events) {
	const auto window = new WindowWin32(options, events);
	windowMap[window->GetHandle()] = window;
	return window;
}

void AppWin32::DespawnWindow(Window* window) {
	const auto win32Window = (WindowWin32*)window;
	const HWND hWnd = win32Window->GetHandle();

	WCHAR className[256];
	GetClassName(hWnd, className, 256);

	UnregisterClass(className, _hInstance);
	windowMap.erase(hWnd);

	if (window->IsMain())
		Exit();
}

void AppWin32::Run() {
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

void AppWin32::Exit() {
	PostQuitMessage(0);
}

HINSTANCE AppWin32::GetHInstance() {
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

LRESULT AppWin32::WndProc(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) {
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
