#include "app_win32.h"

#include <future>
#include <map>

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
	DestroyWindow(hWnd);
	windowMap.erase(hWnd);
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

LRESULT AppWin32::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_CREATE:
			break;
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
