#pragma once

#ifndef GLUINO_WEBVIEW_BASE_H
#define GLUINO_WEBVIEW_BASE_H

#include "webview_options.h"
#include "webview_events.h"
#include "window_base.h"

namespace Gluino {

class WebViewBase {
public:
	explicit WebViewBase(WebViewOptions* options, const WebViewEvents* events) {
		_startUrl = CStrWiden(options->StartUrl);
		_startContent = CStrWiden(options->StartContent);
		_userAgent = CStrWiden(options->UserAgent);

		_onCreated = (Delegate)events->OnCreated;
		_onNavigationStart = (StringDelegate)events->OnNavigationStart;
		_onNavigationEnd = (Delegate)events->OnNavigationEnd;
		_onMessageReceived = (StringDelegate)events->OnMessageReceived;
		_onResourceRequested = (WebResourceDelegate)events->OnResourceRequested;
	}
	virtual ~WebViewBase() = default;

	virtual void Attach(WindowBase* window) = 0;
	virtual void Navigate(cstr url) = 0;
	virtual void NativateToString(cstr content) = 0;
	virtual void PostWebMessage(cstr message) = 0;
	virtual void InjectScript(cstr script, bool onDocumentCreated) = 0;

	virtual bool GetContextMenuEnabled() = 0;
	virtual void SetContextMenuEnabled(bool enabled) = 0;

	virtual bool GetDevToolsEnabled() = 0;
	virtual void SetDevToolsEnabled(bool enabled) = 0;

	virtual cstr GetUserAgent() = 0;
	virtual void SetUserAgent(cstr userAgent) = 0;

protected:
	cstr _startUrl;
	cstr _startContent;
	cstr _userAgent;

	Delegate _onCreated;
	StringDelegate _onNavigationStart;
	Delegate _onNavigationEnd;
	StringDelegate _onMessageReceived;
	WebResourceDelegate _onResourceRequested;
};

}

#endif // !GLUINO_WEBVIEW_BASE_H
