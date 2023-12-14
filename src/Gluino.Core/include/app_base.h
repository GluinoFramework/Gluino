#pragma once

#ifndef GLUINO_APP_BASE_H
#define GLUINO_APP_BASE_H

#include "window_base.h"
#include "webview_base.h"

namespace Gluino {

class AppBase {
public:
	virtual ~AppBase() = default;

	virtual void SpawnWindow(
		WindowOptions* windowOptions, WindowEvents* windowEvents,
		WebViewOptions* webViewOptions,	WebViewEvents* webViewEvents,
		WindowBase** window, WebViewBase** webView) = 0;
	virtual void DespawnWindow(WindowBase* window) = 0;

	virtual void Run() = 0;
	virtual void Exit() = 0;
};

}

#endif // !GLUINO_APP_BASE_H
