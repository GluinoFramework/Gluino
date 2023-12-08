#pragma once

#ifndef GLUINO_WEBVIEW_OPTIONS_H
#define GLUINO_WEBVIEW_OPTIONS_H

namespace Gluino {

struct WebViewOptions {
	wchar_t* StartUrlW;
	char* StartUrlA;
	wchar_t* StartContentW;
	char* StartContentA;
	bool ContextMenuEnabled;
	bool DevToolsEnabled;
	wchar_t* UserAgentW;
	char* UserAgentA;
	bool GrantPermissions;
};

}

#endif // !GLUINO_WEBVIEW_OPTIONS_H
