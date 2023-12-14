#include "app.h"
#include "window.h"
#include "webview.h"

using namespace Gluino;

extern "C" {
#ifdef _WIN32
	EXPORT WindowsOSVersion Gluino_GetWindowsOSVersion() { return GetWindowsOSVersion(); }
	EXPORT App* Gluino_App_Create(const HINSTANCE hInstance, const autostr appId) { return new App(hInstance, appId); }
	EXPORT HWND Gluino_Window_GetHandle(const Window* window) { return window->GetHandle(); }
#else
	//TODO: EXPORT App* Gluino_App_Create() { return new App(); }
#endif

	EXPORT void Gluino_App_Destroy(const App* app) { delete app; }
	EXPORT void Gluino_App_SpawnWindow(App* app, 
		WindowOptions* windowOptions, WindowEvents* windowEvents, 
		WebViewOptions* webViewOptions, WebViewEvents* webViewEvents, 
		WindowBase** window, WebViewBase** webView) {
		app->SpawnWindow(windowOptions, windowEvents, webViewOptions, webViewEvents, window, webView);
	}
	EXPORT void Gluino_App_DespawnWindow(App* app, Window* window) { app->DespawnWindow(window); }
	EXPORT void Gluino_App_Run(App* app) { app->Run(); }
	EXPORT void Gluino_App_Exit(App* app) { app->Exit(); }


	EXPORT void Gluino_Window_Show(Window* window) { window->Show(); }
	EXPORT void Gluino_Window_Hide(Window* window) { window->Hide(); }
	EXPORT void Gluino_Window_Close(Window* window) { window->Close(); }
	EXPORT void Gluino_Window_Center(Window* window) { window->Center(); }
	EXPORT void Gluino_Window_DragMove(Window* window) { window->DragMove(); }
	EXPORT void Gluino_Window_Invoke(Window* window, const Delegate action) { window->Invoke(action); }

	EXPORT void Gluino_Window_GetBounds(Window* window, Rect* bounds) { window->GetBounds(bounds); }

	EXPORT bool Gluino_Window_GetIsDarkMode(Window* window) { return window->GetIsDarkMode(); }

	EXPORT autostr Gluino_Window_GetTitle(Window* window) { return window->GetTitle(); }
	EXPORT void Gluino_Window_SetTitle(Window* window, const autostr title) { window->SetTitle(title); }

	EXPORT void Gluino_Window_GetIcon(Window* window, void** data, int* size) { window->GetIcon(data, size); }
	EXPORT void Gluino_Window_SetIcon(Window* window, void* data, const int size) { window->SetIcon(data, size); }

	EXPORT WindowBorderStyle Gluino_Window_GetBorderStyle(Window* window) { return window->GetBorderStyle(); }
	EXPORT void Gluino_Window_SetBorderStyle(Window* window, const WindowBorderStyle style) { window->SetBorderStyle(style); }

	EXPORT WindowState Gluino_Window_GetWindowState(Window* window) { return window->GetWindowState(); }
	EXPORT void Gluino_Window_SetWindowState(Window* window, const WindowState state) { window->SetWindowState(state); }

	EXPORT WindowTheme Gluino_Window_GetTheme(Window* window) { return window->GetTheme(); }
	EXPORT void Gluino_Window_SetTheme(Window* window, const WindowTheme theme) { window->SetTheme(theme); }

	EXPORT Size Gluino_Window_GetMinimumSize(Window* window) { return window->GetMinimumSize(); }
	EXPORT void Gluino_Window_SetMinimumSize(Window* window, Size size) { window->SetMinimumSize(size); }

	EXPORT Size Gluino_Window_GetMaximumSize(Window* window) { return window->GetMaximumSize(); }
	EXPORT void Gluino_Window_SetMaximumSize(Window* window, Size size) { window->SetMaximumSize(size); }

	EXPORT Size Gluino_Window_GetSize(Window* window) { return window->GetSize(); }
	EXPORT void Gluino_Window_SetSize(Window* window, Size size) { window->SetSize(size); }

	EXPORT Point Gluino_Window_GetLocation(Window* window) { return window->GetLocation(); }
	EXPORT void Gluino_Window_SetLocation(Window* window, Point location) { window->SetLocation(location); }

	EXPORT bool Gluino_Window_GetMinimizeEnabled(Window* window) { return window->GetMinimizeEnabled(); }
	EXPORT void Gluino_Window_SetMinimizeEnabled(Window* window, const bool enabled) { window->SetMinimizeEnabled(enabled); }

	EXPORT bool Gluino_Window_GetMaximizeEnabled(Window* window) { return window->GetMaximizeEnabled(); }
	EXPORT void Gluino_Window_SetMaximizeEnabled(Window* window, const bool enabled) { window->SetMaximizeEnabled(enabled); }

	EXPORT bool Gluino_Window_GetTopMost(Window* window) { return window->GetTopMost(); }
	EXPORT void Gluino_Window_SetTopMost(Window* window, const bool topMost) { window->SetTopMost(topMost); }


	EXPORT void Gluino_WebView_Navigate(WebView* webView, const autostr url) { webView->Navigate(url); }
	EXPORT void Gluino_WebView_NativateToString(WebView* webView, const autostr str) { webView->NativateToString(str); }
	EXPORT void Gluino_WebView_PostWebMessage(WebView* webView, const autostr message) { webView->PostWebMessage(message); }
	EXPORT void Gluino_WebView_InjectScript(WebView* webView, const autostr script, const bool onDocumentCreated) { webView->InjectScript(script, onDocumentCreated); }

	EXPORT bool Gluino_WebView_GetGrantPermissions(const WebView* webView) { return webView->GetGrantPermissions(); }

	EXPORT bool Gluino_WebView_GetContextMenuEnabled(WebView* webView) { return webView->GetContextMenuEnabled(); }
	EXPORT void Gluino_WebView_SetContextMenuEnabled(WebView* webView, const bool enabled) { webView->SetContextMenuEnabled(enabled); }

	EXPORT bool Gluino_WebView_GetDevToolsEnabled(WebView* webView) { return webView->GetDevToolsEnabled(); }
	EXPORT void Gluino_WebView_SetDevToolsEnabled(WebView* webView, const bool enabled) { webView->SetDevToolsEnabled(enabled); }

	EXPORT autostr Gluino_WebView_GetUserAgent(WebView* webView) { return webView->GetUserAgent(); }
	EXPORT void Gluino_WebView_SetUserAgent(WebView* webView, const autostr userAgent) { webView->SetUserAgent(userAgent); }
}