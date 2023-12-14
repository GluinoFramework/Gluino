#include "app.h"
#include "window.h"
#include "window_frame.h"

#include <future>
#include <algorithm>

using namespace Gluino;

Window::Window(WindowOptions* options, const WindowEvents* events, WebView* webView) : WindowBase(options, events) {
	_windowState = options->WindowState;
	_minSize = options->MinimumSize;
	_maxSize = options->MaximumSize;
	_minimizeEnabled = options->MinimizeEnabled;
	_maximizeEnabled = options->MaximizeEnabled;

	const auto x = options->StartupLocation == WindowStartupLocation::Default ? CW_USEDEFAULT : options->Location.x;
	const auto y = options->StartupLocation == WindowStartupLocation::Default ? CW_USEDEFAULT : options->Location.y;

    _hWnd = CreateWindowEx(
        WS_EX_NOREDIRECTIONBITMAP,
        App::GetWndClassName(),
        _title,
        WS_OVERLAPPEDWINDOW,
        x,
        y,
		options->Size.width,
		options->Size.height,
        nullptr,
        nullptr,
        App::GetHInstance(),
        this);

    InitDarkModeSupport();

	SetTheme(options->Theme);
	if (options->StartupLocation == WindowStartupLocation::CenterScreen)
		Center();
	if (options->TopMost)
		SetTopMost(options->TopMost);

	_frame = new WindowFrame(_hWnd);
	if (options->BorderStyle == WindowBorderStyle::SizableNoCaption)
		_frame->Attach();

	_webView = webView;
	_webView->Attach(this);
}

Window::~Window() {
	delete[] _title;
}

LRESULT Window::WndProc(const UINT msg, const WPARAM wParam, const LPARAM lParam) {
	switch (msg) {
		case WM_ACTIVATE: {
			if (LOWORD(wParam) == WA_INACTIVE) {
				_onFocusOut();
			}
			else {
				_webView->Focus();
				_onFocusIn();
				return 0;
			}
			break;
		}
		case WM_SIZE: {
			std::this_thread::sleep_for(std::chrono::nanoseconds(5000));

			_webView->Refit(_borderStyle);

			_onResize(GetSize());

			switch (wParam) {
				case SIZE_MAXIMIZED:
					_onWindowStateChanged((int)WindowState::Maximized);
					_isResizing = false;
					break;
				case SIZE_MINIMIZED:
					_onWindowStateChanged((int)WindowState::Minimized);
					_isResizing = false;
					break;
				case SIZE_RESTORED:
					if (!_isResizing) {
						_onWindowStateChanged((int)WindowState::Normal);
					}
					break;
				default: break;
			}

			return 0;
		}
		case WM_ENTERSIZEMOVE: {
			_onResizeStart(GetSize());
			_isResizing = true;
			break;
		}
		case WM_EXITSIZEMOVE: {
			if (_borderStyle == WindowBorderStyle::SizableNoCaption)
				_frame->Update();

			_onResizeEnd(GetSize());
			_isResizing = false;
			break;
		}
		case WM_MOVE: {
			_onLocationChanged(GetLocation());
			break;
		}
		case WM_GETMINMAXINFO: {
			const auto mmi = reinterpret_cast<MINMAXINFO*>(lParam);

			if (_minSize.width > 0)        mmi->ptMinTrackSize.x = _minSize.width;
			if (_minSize.height > 0)       mmi->ptMinTrackSize.y = _minSize.height;
			if (_maxSize.width < INT_MAX)  mmi->ptMaxTrackSize.x = _maxSize.width;
			if (_maxSize.height < INT_MAX) mmi->ptMaxTrackSize.y = _maxSize.height;
			break;
		}
		case WM_SHOWWINDOW: {
			if (wParam == TRUE) _onShown();
			else                _onHidden();
			break;
		}
		case WM_CLOSE: {
			if (const auto cancel = _onClosing(); !cancel) {
				DestroyWindow(_hWnd);				
			}
			return 0;
		}
		case WM_NCCALCSIZE: {
			if (wParam == TRUE && 
				_borderStyle == WindowBorderStyle::SizableNoCaption || 
				_borderStyle == WindowBorderStyle::FixedNoCaption) {
				auto& [rgrc, _] = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				AdjustMaximizedClientRect(_hWnd, rgrc[0]);
				return 0;
			}
			break;
		}
		case WM_SETTINGCHANGE: {
			if (_theme == WindowTheme::System && IsColorSchemeChange(lParam)) {
				SendMessageW(_hWnd, WM_THEMECHANGED, 0, 0);
			}
			break;
		}
		case WM_THEMECHANGED: {
			ApplyWindowStyle(_hWnd, IsDarkModeEnabled());
			break;
		}
		default:
			return DefWindowProc(_hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(_hWnd, msg, wParam, lParam);
}

void Window::Show() {
	ShowWindow(_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(_hWnd);

	if (_windowState != WindowState::Normal)
		SetWindowState(_windowState);
	if (_borderStyle != WindowBorderStyle::Sizable)
		SetBorderStyle(_borderStyle);
	if (!_minimizeEnabled)
		SetMinimizeEnabled(_minimizeEnabled);
	if (!_maximizeEnabled)
		SetMaximizeEnabled(_maximizeEnabled);
}

void Window::Hide() {
	ShowWindow(_hWnd, SW_HIDE);
}

void Window::Close() {
	PostMessage(_hWnd, WM_CLOSE, 0, 0);
}

void Window::Center() {
	const auto screenDpi = GetDpiForWindow(_hWnd);
	const auto screenWidth = GetSystemMetricsForDpi(SM_CXSCREEN, screenDpi);
	const auto screenHeight = GetSystemMetricsForDpi(SM_CYSCREEN, screenDpi);

	RECT wndRect = {};
	GetWindowRect(_hWnd, &wndRect);
	const auto wndWidth = wndRect.right - wndRect.left;
	const auto wndHeight = wndRect.bottom - wndRect.top;
	const auto wndX = (screenWidth - wndWidth) / 2;
	const auto wndY = (screenHeight - wndHeight) / 2;
	Point loc = { wndX, wndY };

	SetLocation(loc);
}

void Window::DragMove() {
	ReleaseCapture();
	PostMessage(_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
}

void Window::Invoke(Delegate action) {
	std::promise<void> promise;
	const std::future<void> future = promise.get_future();

	PostMessage(_hWnd, WM_USER_INVOKE, (WPARAM)action, (LPARAM)&promise);

	future.wait();
}

void Window::GetBounds(Rect* bounds) {
	RECT rect;
	GetWindowRect(_hWnd, &rect);

	*bounds = {
		rect.right - rect.left,
		rect.bottom - rect.top,
		rect.left,
		rect.top
	};
}

bool Window::GetIsDarkMode() {
	return _theme == WindowTheme::System ? IsDarkModeEnabled() : _theme == WindowTheme::Dark;
}

autostr Window::GetTitle() {
	const auto length = GetWindowTextLength(_hWnd);
	const auto title = new wchar_t[length + 1];
	GetWindowText(_hWnd, title, length + 1);
	return title;
}

void Window::SetTitle(const autostr title) {
	_title = CopyStr(title);

	SetWindowText(_hWnd, _title);
}

WindowBorderStyle Window::GetBorderStyle() {
	return _borderStyle;
}

void Window::SetBorderStyle(const WindowBorderStyle style) {
	if (style == WindowBorderStyle::SizableNoCaption ||
		style == WindowBorderStyle::FixedNoCaption) {
		ApplyBorderlessStyle(_hWnd, true);
		if (style == WindowBorderStyle::SizableNoCaption)
			_frame->Attach();
	}
	else if (style == WindowBorderStyle::Sizable || 
			 style == WindowBorderStyle::Fixed) {
		ApplyBorderlessStyle(_hWnd, false);
		_frame->Detach();

		const auto wndStyle = GetWindowLongPtr(_hWnd, GWL_STYLE);
		if (style == WindowBorderStyle::Sizable) {
			SetWindowLongPtr(_hWnd, GWL_STYLE, wndStyle | WS_THICKFRAME);
		}
		else if (style == WindowBorderStyle::Fixed) {
			SetWindowLongPtr(_hWnd, GWL_STYLE, wndStyle & ~WS_THICKFRAME);
		}
	}
	else if (style == WindowBorderStyle::None) {
		//TODO
	}

	_borderStyle = style;	
}

WindowState Window::GetWindowState() {
	WINDOWPLACEMENT placement;
	if (GetWindowPlacement(_hWnd, &placement))
		return WindowState::Normal;
	switch (placement.showCmd) {
		case SW_MAXIMIZE: return WindowState::Maximized;
		case SW_MINIMIZE: return WindowState::Minimized;
		default:          return WindowState::Normal;
	}
}

void Window::SetWindowState(const WindowState state) {
	WINDOWPLACEMENT placement;
	if (!GetWindowPlacement(_hWnd, &placement))
		return;
	switch (state) {
		case WindowState::Maximized: placement.showCmd = SW_MAXIMIZE; break;
		case WindowState::Minimized: placement.showCmd = SW_MINIMIZE; break;
		case WindowState::Normal:	 placement.showCmd = SW_NORMAL;	  break;
	}
	SetWindowPlacement(_hWnd, &placement);
}

WindowTheme Window::GetTheme() {
	return _theme;
}

void Window::SetTheme(const WindowTheme theme) {
	_theme = theme;
	const auto darkMode =
		theme == WindowTheme::System ? IsDarkModeEnabled() :
		theme == WindowTheme::Dark;
	ApplyWindowStyle(_hWnd, darkMode);
	UpdateWindow(_hWnd);
}

Size Window::GetMinimumSize() {
	return _minSize;
}

void Window::SetMinimumSize(Size& size) {
	_minSize = size;

	const auto [width, height] = GetSize();
	if (Size newSize = {
		std::max(width, _minSize.width),
		std::max(height, _minSize.height)
		}; newSize.width != width || newSize.height != height) {
		SetSize(newSize);
	}
}

Size Window::GetMaximumSize() {
	return _maxSize;
}

void Window::SetMaximumSize(Size& size) {
	_maxSize = size;

	const auto [width, height] = GetSize();
	if (Size newSize = {
			std::min(width, _maxSize.width),
			std::min(height, _maxSize.height)
		}; newSize.width != width || newSize.height != height) {
		SetSize(newSize);
	}
}

Size Window::GetSize() {
	RECT rect;
	GetWindowRect(_hWnd, &rect);
	return { rect.right - rect.left, rect.bottom - rect.top };
}

void Window::SetSize(Size& size) {
	SetWindowPos(_hWnd, HWND_TOP, 0, 0, size.width, size.height, SWP_NOMOVE | SWP_NOZORDER);
}

Point Window::GetLocation() {
	RECT rect;
	GetWindowRect(_hWnd, &rect);
	return { rect.left, rect.top };
}

void Window::SetLocation(Point& location) {
	SetWindowPos(_hWnd, HWND_TOP, location.x, location.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

bool Window::GetMinimizeEnabled() {
	if (_borderStyle == WindowBorderStyle::SizableNoCaption ||
		_borderStyle == WindowBorderStyle::FixedNoCaption) {
		return _minimizeEnabled;
	}

	const auto style = GetWindowLong(_hWnd, GWL_STYLE);
	return (style & WS_MINIMIZEBOX) != 0;
}

void Window::SetMinimizeEnabled(const bool enabled) {
	_minimizeEnabled = enabled;

	if (_borderStyle == WindowBorderStyle::SizableNoCaption ||
		_borderStyle == WindowBorderStyle::FixedNoCaption) {
		return;
	}

	auto style = GetWindowLong(_hWnd, GWL_STYLE);
	if (enabled) style |= WS_MINIMIZEBOX;
	else         style &= ~WS_MINIMIZEBOX;
	SetWindowLong(_hWnd, GWL_STYLE, style);
	SetWindowPos(_hWnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

bool Window::GetMaximizeEnabled() {
	if (_borderStyle == WindowBorderStyle::SizableNoCaption ||
		_borderStyle == WindowBorderStyle::FixedNoCaption) {
		return _maximizeEnabled;
	}

	const auto style = GetWindowLong(_hWnd, GWL_STYLE);
	return (style & WS_MAXIMIZEBOX) != 0;
}

void Window::SetMaximizeEnabled(const bool enabled) {
	_maximizeEnabled = enabled;

	if (_borderStyle == WindowBorderStyle::SizableNoCaption ||
		_borderStyle == WindowBorderStyle::FixedNoCaption) {
		return;
	}

	auto style = GetWindowLong(_hWnd, GWL_STYLE);
	if (enabled) style |= WS_MAXIMIZEBOX;
	else         style &= ~WS_MAXIMIZEBOX;
	SetWindowLong(_hWnd, GWL_STYLE, style);
	SetWindowPos(_hWnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

bool Window::GetTopMost() {
	const auto exStyle = GetWindowLongPtr(_hWnd, GWL_EXSTYLE);
	return (exStyle & WS_EX_TOPMOST) != 0;
}

void Window::SetTopMost(const bool topMost) {
    SetWindowPos(_hWnd, topMost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

