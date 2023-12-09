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
	void Navigate(autostr url) override;
	void NativateToString(autostr content) override;
	void PostWebMessage(autostr message) override;

	bool GetGrantPermissions() const;

	bool GetContextMenuEnabled() override;
	void SetContextMenuEnabled(bool enabled) override;

	bool GetDevToolsEnabled() override;
	void SetDevToolsEnabled(bool enabled) override;

	autostr GetUserAgent() override;
	void SetUserAgent(autostr userAgent) override;

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
};

}

#endif // !GLUINO_WEBVIEW_H
