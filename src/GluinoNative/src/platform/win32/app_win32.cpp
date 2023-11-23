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

void AppWin32::Register(const autostr className) const {
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof WNDCLASSEX;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = WS_EX_NOPARENTNOTIFY;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = CopyStr(className);

	RegisterClassEx(&wcex);

	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}

Window* AppWin32::SpawnWindow(WindowOptions* options) {
	Register(options->ClassName);

	const auto window = new WindowWin32(options, _hInstance);
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
