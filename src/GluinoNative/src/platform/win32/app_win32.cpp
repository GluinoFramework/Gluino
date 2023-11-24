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

Window* AppWin32::SpawnWindow(WindowOptions* options) {
	const auto window = new WindowWin32(options);
	windowMap[window->GetHandle()] = window;
	return window;
}

void AppWin32::DespawnWindow(Window* window) {
	const auto win32Window = (WindowWin32*)window;
	const HWND hWnd = win32Window->GetHandle();

	WCHAR className[256];
	GetClassName(hWnd, className, 256);

	UnregisterClass(className, _hInstance);
	//DestroyWindow(hWnd);
	windowMap.erase(hWnd);

	
	if (windowMap.empty())
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
	//constexpr int mica_entry = 1029, mica_value = 0x01; // Windows 11 23523-
	constexpr int mica_entry = 38, mica_value = 0x02; // Windows 11 23523+
	constexpr bool theme = true;

	DwmExtendFrameIntoClientArea(hWnd, &margins);
	DwmSetWindowAttribute(hWnd, 20, &theme, sizeof(int));
	DwmSetWindowAttribute(hWnd, mica_entry, &mica_value, sizeof(int));
}

LRESULT AppWin32::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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
			const auto callback = (Action)wParam;
			callback();

			const auto promise = (std::promise<void>*)lParam;
			promise->set_value();
			return 0;
		}
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
