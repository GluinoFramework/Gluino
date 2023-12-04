#include "app.h"

#include <dwmapi.h>
#include <future>
#include <iostream>
#include <map>
#include <shobjidl_core.h>

using namespace Gluino;

App* app{};
std::map<HWND, Window*> windowMap{};

App::App(const HINSTANCE hInstance, wchar_t* appId) {
	_hInstance = hInstance;
	_appId = CopyStr(appId);
	_wndClassName = ConcatStr(_appId, L"Window");

	SetCurrentProcessExplicitAppUserModelID(_appId);

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
	wcex.lpszClassName = _wndClassName;

	RegisterClassEx(&wcex);

	app = this;
}

App::~App() {
	delete[] _appId;
	delete[] _wndClassName;
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

wchar_t* App::GetWndClassName() {
	return app->_wndClassName;
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
