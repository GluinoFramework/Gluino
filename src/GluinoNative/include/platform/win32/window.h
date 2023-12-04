#pragma once

#ifndef GLUINO_WINDOW_H
#define GLUINO_WINDOW_H

#include "window_base.h"

#include <Windows.h>
#include <dwmapi.h>
#include <wil/com.h>
#include <WebView2.h>

#undef min
#undef max

namespace Gluino {

class WindowFrame;

class Window final : public WindowBase {
public:
	explicit Window(WindowOptions* options, const WindowEvents* events);
	~Window() override;

	HWND GetHandle() const { return _hWnd; }
	LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	void SetBorderlessStyle(bool borderless) const;
	void AttachWebView();
	void RefitWebView() const;
	void FocusWebView() const;

	void Show() override;
	void Hide() override;
	void Close() override;
	void Invoke(Delegate action) override;

	void GetBounds(Rect* bounds) override;

	autostr GetTitle() override;
	void SetTitle(autostr title) override;

	WindowBorderStyle GetBorderStyle() override;
	void SetBorderStyle(WindowBorderStyle style) override;
	
	WindowState GetWindowState() override;
	void SetWindowState(WindowState state) override;

	Size GetMinimumSize() override;
	void SetMinimumSize(Size& size) override;

	Size GetMaximumSize() override;
	void SetMaximumSize(Size& size) override;

	Size GetSize() override;
	void SetSize(Size& size) override;

	Point GetLocation() override;
	void SetLocation(Point& location) override;

	bool GetTopMost() override;
	void SetTopMost(bool topMost) override;

private:
	wchar_t* _className;
	HWND _hWnd;
	Size _minSize;
	Size _maxSize;
	WindowFrame* _frame;

	wil::com_ptr<ICoreWebView2>            _webview;
	wil::com_ptr<ICoreWebView2Environment> _webviewEnv;
	wil::com_ptr<ICoreWebView2Controller>  _webviewController;
	wil::com_ptr<ICoreWebView2Controller2> _webviewController2;
	wil::com_ptr<ICoreWebView2Settings>    _webviewSettings;
	wil::com_ptr<ICoreWebView2Settings2>   _webviewSettings2;

	HRESULT OnWebView2CreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* env);
	HRESULT OnWebView2CreateControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
	HRESULT OnWebView2WebMessageReceived(ICoreWebView2* webview, ICoreWebView2WebMessageReceivedEventArgs* args);
	HRESULT OnWebView2WebResourceRequested(ICoreWebView2* webview, ICoreWebView2WebResourceRequestedEventArgs* args);
	HRESULT OnWebView2PermissionRequested(ICoreWebView2* sender, ICoreWebView2PermissionRequestedEventArgs* args);
};

}

#endif // !GLUINO_WINDOW_H

