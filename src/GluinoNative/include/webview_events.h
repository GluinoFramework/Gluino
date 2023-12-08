#pragma once

#ifndef GLUINO_WEBVIEW_EVENTS_H
#define GLUINO_WEBVIEW_EVENTS_H

#include "common.h"

namespace Gluino {

struct WebViewEvents {
	Delegate* OnCreated;
	StringDelegate* OnNavigationStart;
	Delegate* OnNavigationEnd;
	StringDelegate* OnMessageReceived;
};

}

#endif // !GLUINO_WEBVIEW_EVENTS_H
