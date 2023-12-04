#include "app.h"
#include "window.h"
#include "window_frame.h"

#include <future>
#include <algorithm>
#include <iostream>
#include <shlobj.h>
#include <dwmapi.h>
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace Gluino;

Window::Window(WindowOptions* options, const WindowEvents* events) : WindowBase(options, events) {
    _className = CopyStr(options->ClassName);
    _title = CopyStr(options->TitleW);
	_windowState = options->WindowState;
	_minSize = options->MinimumSize;
	_maxSize = options->MaximumSize;

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof WNDCLASSEX;
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = App::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = WS_EX_NOPARENTNOTIFY;
    wcex.hInstance = App::GetHInstance();
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = _className;

    RegisterClassEx(&wcex);

	const auto x = options->StartupLocation == WindowStartupLocation::Default ? CW_USEDEFAULT : options->Location.x;
	const auto y = options->StartupLocation == WindowStartupLocation::Default ? CW_USEDEFAULT : options->Location.y;

    _hWnd = CreateWindowEx(
        WS_EX_NOREDIRECTIONBITMAP,
        _className,
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

    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    InitDarkModeSupport();
    ApplyWindowStyle(_hWnd, true);

	if (options->StartupLocation == WindowStartupLocation::CenterScreen) {
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

	SetResizable(options->Resizable);
	SetTopMost(options->TopMost);

	_frame = new WindowFrame(_hWnd);
}



Window::~Window() {
	delete[] _className;
	delete[] _title;
}

LRESULT Window::WndProc(const UINT msg, const WPARAM wParam, const LPARAM lParam) {
	switch (msg) {
		case WM_ACTIVATE: {
			if (LOWORD(wParam) == WA_INACTIVE) {
				_onFocusOut();
			}
			else {
				FocusWebView();
				_onFocusIn();
				return 0;
			}
			break;
		}
		case WM_SIZE: {
			if (_borderStyle == WindowBorderStyle::Borderless)
				_frame->Update();

			RefitWebView();
			
			_onSizeChanged(GetSize());

			if (LOWORD(wParam) == SIZE_MAXIMIZED)      _onWindowStateChanged((int)WindowState::Maximized);
			else if (LOWORD(wParam) == SIZE_MINIMIZED) _onWindowStateChanged((int)WindowState::Minimized);
			else if (LOWORD(wParam) == SIZE_RESTORED)  _onWindowStateChanged((int)WindowState::Normal);
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
				return 0;
			}
			break;
		}
		case WM_NCCALCSIZE: {
			if (wParam == TRUE && _borderStyle == WindowBorderStyle::Borderless) {
				auto& [rgrc, _] = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				AdjustMaximizedClientRect(_hWnd, rgrc[0]);
				return 0;
			}
			break;
		}
		case WM_SETTINGCHANGE: {
			if (IsColorSchemeChange(lParam)) {
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

void Window::SetBorderlessStyle(const bool borderless) const {
	const auto compositionEnabled = IsCompositionEnabled();

	constexpr DWORD aeroBorderless = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
	constexpr DWORD basicBorderless = WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
	const DWORD borderlessStyle = compositionEnabled ? aeroBorderless : basicBorderless;

	const DWORD newStyle = borderless ? borderlessStyle : WS_OVERLAPPEDWINDOW;
	const DWORD oldStyle = GetWindowLong(_hWnd, GWL_STYLE);

	if (newStyle == oldStyle) {
		return;
	}

	SetWindowLongW(_hWnd, GWL_STYLE, static_cast<LONG>(newStyle));

	if (compositionEnabled) {
		constexpr MARGINS shadowState[2] = { {0, 0, 0, 0}, {1, 1, 1, 1} };
		DwmExtendFrameIntoClientArea(_hWnd, &shadowState[borderless]);
	}

	SetWindowPos(_hWnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	ShowWindow(_hWnd, SW_SHOW);
}

void Window::AttachWebView() {
	wchar_t dataPath[MAX_PATH];
	SHGetSpecialFolderPath(nullptr, dataPath, CSIDL_LOCAL_APPDATA, FALSE);

	CreateCoreWebView2EnvironmentWithOptions(nullptr, dataPath, nullptr,
		Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this,
			&Window::OnWebView2CreateEnvironmentCompleted).Get());
}

void Window::RefitWebView() const {
	if (_webviewController == nullptr) return;

	RECT bounds;
	GetClientRect(_hWnd, &bounds);
	if (_borderStyle == WindowBorderStyle::Borderless) {
		bounds.top += 1;
		bounds.left += 1;
		bounds.right -= 1;
		bounds.bottom -= 1;
	}

	_webviewController->put_Bounds(bounds);
}

void Window::FocusWebView() const {
	if (!_webviewController) return;

	_webviewController->MoveFocus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
}

void Window::Show() {
	ShowWindow(_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(_hWnd);

	if (_windowState != WindowState::Normal)
	if (_windowState != WindowState::Normal)
		SetWindowState(_windowState);

	if (_borderStyle != WindowBorderStyle::Normal)
		SetBorderStyle(_borderStyle);

	AttachWebView();
}

void Window::Hide() {
	ShowWindow(_hWnd, SW_HIDE);
}

void Window::Close() {
	PostMessage(_hWnd, WM_CLOSE, 0, 0);
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
	if (style == WindowBorderStyle::Borderless) {
		SetBorderlessStyle(true);
		if (_resizable)	_frame->Attach();
	}
	else if (_borderStyle == WindowBorderStyle::Borderless && style != WindowBorderStyle::Borderless) {
		SetBorderlessStyle(false);
		_frame->Detach();
	}
	else if (style == WindowBorderStyle::None) {
		//TODO
		if (_resizable)	_frame->Attach();
	}

	_borderStyle = style;	
}

WindowState Window::GetWindowState() {
	WINDOWPLACEMENT placement;
	if (GetWindowPlacement(_hWnd, &placement))
		return WindowState::Normal;
	switch (placement.showCmd) {
		case SW_MAXIMIZE:
			return WindowState::Maximized;
		case SW_MINIMIZE:
			return WindowState::Minimized;
		default:
			return WindowState::Normal;
	}
}

void Window::SetWindowState(WindowState state) {
	WINDOWPLACEMENT placement;
	if (!GetWindowPlacement(_hWnd, &placement))
		return;
	switch (state) { // NOLINT(clang-diagnostic-switch-enum)
		case WindowState::Maximized:
			placement.showCmd = SW_MAXIMIZE;
			break;
		case WindowState::Minimized:
			placement.showCmd = SW_MINIMIZE;
			break;
		default:
			placement.showCmd = SW_NORMAL;
			break;
	}
	SetWindowPlacement(_hWnd, &placement);
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

bool Window::GetTopMost() {
	const auto exStyle = GetWindowLongPtr(_hWnd, GWL_EXSTYLE);
	return (exStyle & WS_EX_TOPMOST) != 0;
}

void Window::SetTopMost(const bool topMost) {
    SetWindowPos(_hWnd, topMost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

bool Window::GetResizable() {
	if (_borderStyle == WindowBorderStyle::Borderless) return _resizable;

	const auto style = GetWindowLongPtr(_hWnd, GWL_STYLE);
	return (style & WS_THICKFRAME) != 0;
}

void Window::SetResizable(const bool resizable) {
	_resizable = resizable;

	if (_borderStyle == WindowBorderStyle::Borderless) return;

	const auto style = GetWindowLongPtr(_hWnd, GWL_STYLE);
	if (resizable) {
		SetWindowLongPtr(_hWnd, GWL_STYLE, style | WS_THICKFRAME);
	}
	else {
		SetWindowLongPtr(_hWnd, GWL_STYLE, style & ~WS_THICKFRAME);
	}
}

HRESULT Window::OnWebView2CreateEnvironmentCompleted(const HRESULT result, ICoreWebView2Environment* env) {
	if (result != S_OK) return result;
	const auto envResult = env->QueryInterface(&_webviewEnv);
	if (envResult != S_OK) return envResult;

	env->CreateCoreWebView2Controller(_hWnd,
		Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this,
			&Window::OnWebView2CreateControllerCompleted).Get());

	return S_OK;
}

HRESULT Window::OnWebView2CreateControllerCompleted(HRESULT result, ICoreWebView2Controller* controller) {
	if (result != S_OK) return result;

	const auto controllerResult = controller->QueryInterface(&_webviewController);
	if (controllerResult != S_OK) return controllerResult;

	_webviewController->get_CoreWebView2(&_webview);

	_webviewController2 = _webviewController.query<ICoreWebView2Controller2>();
	_webviewController2->put_DefaultBackgroundColor({ 0, 0, 0, 0 });

	_webview->get_Settings(&_webviewSettings);
	_webviewSettings->put_AreHostObjectsAllowed(TRUE);
	_webviewSettings->put_IsScriptEnabled(TRUE);
	_webviewSettings->put_AreDefaultScriptDialogsEnabled(TRUE);
	_webviewSettings->put_IsWebMessageEnabled(TRUE);
	_webviewSettings->put_IsStatusBarEnabled(FALSE);

	_webviewSettings2 = _webviewSettings.try_query<ICoreWebView2Settings2>();

	/*EventRegistrationToken permissionRequestedToken;
	_webview->add_PermissionRequested(
		Callback<ICoreWebView2PermissionRequestedEventHandler>(this,
			&WindowWin32::OnWebView2PermissionRequested).Get(), &permissionRequestedToken);*/

	const auto htmlContent = L"\
<!DOCTYPE html>\n\
<html lang=\"en\">\n\
<head>\n\
  <meta charset=\"UTF-8\">\n\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
  <title>Test</title>\n\
\n\
  <style>\n\
	html {\n\
      padding: 0;\n\
	  margin: 0;\n\
      box-sizing: border-box;\n\
	  border: 1px solid red;\n\
      height: 100vh;\n\
    }\n\
\n\
    html, body {\n\
      background-color: transparent;\n\
      color: pink;\n\
      font-family: \"Segoe UI\", sans-serif;\n\
      font-size: 14px;\n\
      font-weight: 400;\n\
    }\n\
  </style>\n\
</head>\n\
<body>\n\
  This is a test.\n\
</body>\n\
</html>";

	_webview->NavigateToString(htmlContent);

	RefitWebView();

	return S_OK;
}

HRESULT Window::OnWebView2WebMessageReceived(ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args) {
	return S_OK;
}

HRESULT Window::OnWebView2WebResourceRequested(ICoreWebView2* webview, ICoreWebView2WebResourceRequestedEventArgs* args) {
	return S_OK;
}

HRESULT Window::OnWebView2PermissionRequested(ICoreWebView2* sender, ICoreWebView2PermissionRequestedEventArgs* args) {
	return S_OK;
}

