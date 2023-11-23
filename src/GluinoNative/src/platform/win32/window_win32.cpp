#include "window_win32.h"

#include <future>

using namespace Gluino;

WindowWin32::WindowWin32(WindowOptions* options, const HINSTANCE hInstance) : Window(options) {
	_className = CopyStr(options->ClassName);
	_title = CopyStr(options->TitleW);

	_hWnd = CreateWindowEx(
		WS_EX_COMPOSITED,
		_className,
		_title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		800, 600,
		nullptr,
		nullptr,
		hInstance,
		this
	);
}

WindowWin32::~WindowWin32() {
	delete[] _className;
	delete[] _title;
}

HWND WindowWin32::GetHandle() const {
	return _hWnd;
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

void WindowWin32::SetTitle(const autostr title) {
	_title = CopyStr(title);

	SetWindowText(_hWnd, _title);
}
