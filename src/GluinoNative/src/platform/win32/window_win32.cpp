#include "window_win32.h"

#include <future>

#include "app_win32.h"

using namespace Gluino;

void Register(const autostr className) {
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof WNDCLASSEX;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = AppWin32::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = WS_EX_NOPARENTNOTIFY;
	wcex.hInstance = AppWin32::GetHInstance();
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = className;

	RegisterClassEx(&wcex);

	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}

WindowWin32::WindowWin32(WindowOptions* options, const WindowEvents* events) : Window(options, events) {
	_className = CopyStr(options->ClassName);
	_title = CopyStr(options->TitleW);

	Register(_className);

	_hWnd = CreateWindowEx(
		WS_EX_LAYERED,
		_className,
		_title,
		WS_OVERLAPPEDWINDOW,
		options->Location.x,
		options->Location.y,
		options->Size.width,
		options->Size.height,
		nullptr,
		nullptr,
		AppWin32::GetHInstance(),
		this
	);

	SetLayeredWindowAttributes(_hWnd, RGB(255, 255, 255), 0, LWA_COLORKEY);
}

WindowWin32::~WindowWin32() {
	delete[] _className;
	delete[] _title;
}

LRESULT WindowWin32::WndProc(const UINT msg, const WPARAM wParam, const LPARAM lParam) {
	switch (msg) {
		case WM_ACTIVATE: {
			if (LOWORD(wParam) == WA_INACTIVE) {
				_onFocusIn();
			}
			else {
				_onFocusOut();
				return 0;
			}
			break;
		}
		case WM_SIZE: {
			Rect bounds;
			GetBounds(&bounds);
			_onSizeChanged({ bounds.width, bounds.height });
			break;
		}
		case WM_MOVE: {
			Rect bounds;
			GetBounds(&bounds);
			_onLocationChanged({ bounds.x, bounds.y });
			break;
		}
		case WM_CLOSE: {
			const auto cancel = _onClosing();
			if (!cancel) {
				DestroyWindow(_hWnd);
				return 0;
			}
			break;
		}
		default:
			return DefWindowProc(_hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(_hWnd, msg, wParam, lParam);
}

void WindowWin32::Show() {
	ShowWindow(_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(_hWnd);
}

void WindowWin32::Hide() {
	ShowWindow(_hWnd, SW_HIDE);
}

void WindowWin32::Close() {
	PostMessage(_hWnd, WM_CLOSE, 0, 0);
}

void WindowWin32::Invoke(Delegate action) {
	std::promise<void> promise;
	const std::future<void> future = promise.get_future();

	PostMessage(_hWnd, WM_USER_INVOKE, (WPARAM)action, (LPARAM)&promise);

	future.wait();
}

void WindowWin32::GetBounds(Rect* bounds) {
	RECT rect;
	GetWindowRect(_hWnd, &rect);

	*bounds = {
		rect.right - rect.left,
		rect.bottom - rect.top,
		rect.left,
		rect.top,
		rect.top,
		rect.right,
		rect.bottom,
		rect.left
	};
}

autostr WindowWin32::GetTitle() {
	const auto length = GetWindowTextLength(_hWnd);
	const auto title = new wchar_t[length + 1];
	GetWindowText(_hWnd, title, length + 1);
	return title;
}

void WindowWin32::SetTitle(const autostr title) {
	_title = CopyStr(title);

	SetWindowText(_hWnd, _title);
}
