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
#ifdef _WIN32
		if (options->StartUrlW) _startUrl = CopyStr(options->StartUrlW);
		if (options->StartContentW) _startContent = CopyStr(options->StartContentW);
		if (options->UserAgentW) _userAgent = CopyStr(options->UserAgentW);
#else
		if (options->StartUrlA) _startUrl = CopyStr(options->StartUrlA);
		if (options->StartStrA) _startStr = CopyStr(options->StartStrA);
		if (options->UserAgentA) _userAgent = CopyStr(options->UserAgentA);
#endif

		_onCreated = (Delegate)events->OnCreated;
		_onNavigationStart = (StringDelegate)events->OnNavigationStart;
		_onNavigationEnd = (Delegate)events->OnNavigationEnd;
		_onMessageReceived = (StringDelegate)events->OnMessageReceived;
		_onResourceRequested = (WebResourceDelegate)events->OnResourceRequested;
	}
	virtual ~WebViewBase() = default;

	virtual void Attach(WindowBase* window) = 0;
	virtual void Navigate(autostr url) = 0;
	virtual void NativateToString(autostr content) = 0;
	virtual void PostWebMessage(autostr message) = 0;
	virtual void InjectScript(autostr script, bool onDocumentCreated) = 0;

	virtual bool GetContextMenuEnabled() = 0;
	virtual void SetContextMenuEnabled(bool enabled) = 0;

	virtual bool GetDevToolsEnabled() = 0;
	virtual void SetDevToolsEnabled(bool enabled) = 0;

	virtual autostr GetUserAgent() = 0;
	virtual void SetUserAgent(autostr userAgent) = 0;

protected:
	autostr _startUrl;
	autostr _startContent;
	autostr _userAgent;

	Delegate _onCreated;
	StringDelegate _onNavigationStart;
	Delegate _onNavigationEnd;
	StringDelegate _onMessageReceived;
	WebResourceDelegate _onResourceRequested;
};

}

#endif // !GLUINO_WEBVIEW_BASE_H
