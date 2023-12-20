#pragma once

#ifndef GLUINO_WEBVIEW_OPTIONS_H
#define GLUINO_WEBVIEW_OPTIONS_H

namespace Gluino {

struct WebViewOptions {
	char* StartUrl;
	char* StartContent;
	bool ContextMenuEnabled;
	bool DevToolsEnabled;
	char* UserAgent;
	bool GrantPermissions;
};

}

#endif // !GLUINO_WEBVIEW_OPTIONS_H
