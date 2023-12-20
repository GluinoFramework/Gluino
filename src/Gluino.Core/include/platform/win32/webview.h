#pragma once

#ifndef GLUINO_WEBVIEW_H
#define GLUINO_WEBVIEW_H

#include "webview_base.h"
#include "window.h"

#include <wil/com.h>
#include <WebView2.h>

namespace Gluino {

class WebView final : public WebViewBase {
public:
	explicit WebView(WebViewOptions* options, const WebViewEvents* events) : WebViewBase(options, events) {
		_contextMenuEnabled = options->ContextMenuEnabled;
		_devToolsEnabled = options->DevToolsEnabled;
		_grantPermissions = options->GrantPermissions;
	}
	~WebView() override;

	void Refit(const WindowBorderStyle& borderStyle) const;
	void Focus() const;

	void Attach(WindowBase* window) override;
	void Navigate(cstr url) override;
	void NativateToString(cstr content) override;
	void PostWebMessage(cstr message) override;
	void InjectScript(cstr script, bool onDocumentCreated) override;

	bool GetGrantPermissions() const;

	bool GetContextMenuEnabled() override;
	void SetContextMenuEnabled(bool enabled) override;

	bool GetDevToolsEnabled() override;
	void SetDevToolsEnabled(bool enabled) override;

	cstr GetUserAgent() override;
	void SetUserAgent(cstr userAgent) override;

private:
	Window* _window = nullptr;
	HWND _hWndWnd = nullptr;

	bool _contextMenuEnabled;
	bool _devToolsEnabled;
	bool _grantPermissions = false;

	wil::com_ptr<ICoreWebView2>            _webview;
	wil::com_ptr<ICoreWebView2Environment> _webviewEnv;
	wil::com_ptr<ICoreWebView2Controller>  _webviewController;
	wil::com_ptr<ICoreWebView2Controller2> _webviewController2;
	wil::com_ptr<ICoreWebView2Settings>    _webviewSettings;
	wil::com_ptr<ICoreWebView2Settings2>   _webviewSettings2;

	HRESULT OnWebView2CreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* env);
	HRESULT OnWebView2CreateControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
	HRESULT OnWebView2NavigationStarting(ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args);
	HRESULT OnWebView2NavigationCompleted(ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args);
	HRESULT OnWebView2WebMessageReceived(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args);
	HRESULT OnWebView2WebResourceRequested(ICoreWebView2* sender, ICoreWebView2WebResourceRequestedEventArgs* args);
	HRESULT OnWebView2PermissionRequested(ICoreWebView2* sender, ICoreWebView2PermissionRequestedEventArgs* args);
};

}

#endif // !GLUINO_WEBVIEW_H
