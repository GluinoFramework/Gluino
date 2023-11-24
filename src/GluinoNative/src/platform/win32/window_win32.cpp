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

WindowWin32::WindowWin32(WindowOptions* options) : Window(options) {
	_className = CopyStr(options->ClassName);
	_title = CopyStr(options->TitleW);

	Register(_className);

	_hWnd = CreateWindowEx(
		WS_EX_LAYERED,
		_className,
		_title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800, 600,
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

void WindowWin32::Show() {
	ShowWindow(_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(_hWnd);
}

void WindowWin32::Hide() {
}

void WindowWin32::Close() {
}

void WindowWin32::Invoke(Action action) {
	std::promise<void> promise;
	const std::future<void> future = promise.get_future();

	PostMessage(_hWnd, WM_USER_INVOKE, (WPARAM)action, (LPARAM)&promise);

	future.wait();
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
