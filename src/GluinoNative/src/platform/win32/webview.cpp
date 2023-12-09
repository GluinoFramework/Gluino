#include "webview.h"

#include <shlobj.h>
#include <Shlwapi.h>
#include <wrl.h>

#pragma comment(lib, "Shlwapi.lib")

using namespace Microsoft::WRL;
using namespace Gluino;

WebView::~WebView() {
	delete[] _userAgent;
}

void WebView::Refit(const WindowBorderStyle& borderStyle) const {
	if (_webviewController == nullptr) return;

	RECT bounds;
	GetClientRect(_hWndWnd, &bounds);
	if (borderStyle == WindowBorderStyle::SizableNoCaption ||
		borderStyle == WindowBorderStyle::FixedNoCaption) {
		bounds.top += 1;
		bounds.left += 1;
		bounds.right -= 1;
		bounds.bottom -= 1;
	}

	_webviewController->put_Bounds(bounds);
}

void WebView::Focus() const {
	if (!_webviewController) return;
	_webviewController->MoveFocus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
}

void WebView::Attach(WindowBase* window) {
	_window = (Window*)window;
	_hWndWnd = _window->GetHandle();

	wchar_t dataPath[MAX_PATH];
	SHGetSpecialFolderPath(nullptr, dataPath, CSIDL_LOCAL_APPDATA, FALSE);

	CreateCoreWebView2EnvironmentWithOptions(nullptr, dataPath, nullptr,
		Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this,
			&WebView::OnWebView2CreateEnvironmentCompleted).Get());
}

void WebView::Navigate(const autostr url) {
	if (_webview == nullptr) return;
	_webview->Navigate(url);
}

void WebView::NativateToString(const autostr content) {
	if (_webview == nullptr) return;
	_webview->NavigateToString(content);
}

void WebView::PostWebMessage(const autostr message) {
	if (_webview == nullptr) return;
	_webview->PostWebMessageAsString(message);
}

bool WebView::GetGrantPermissions() const {
	return _grantPermissions;
}

bool WebView::GetContextMenuEnabled() {
	BOOL enabled;
	_webviewSettings->get_AreDefaultContextMenusEnabled(&enabled);
	return enabled;
}

void WebView::SetContextMenuEnabled(const bool enabled) {
	_webviewSettings->put_AreDefaultContextMenusEnabled(enabled);
}

bool WebView::GetDevToolsEnabled() {
	BOOL enabled;
	_webviewSettings->get_AreDevToolsEnabled(&enabled);
	return enabled;
}

void WebView::SetDevToolsEnabled(bool enabled) {
	_webviewSettings->put_AreDevToolsEnabled(enabled);
}

autostr WebView::GetUserAgent() {
	wchar_t* userAgent;
	_webviewSettings2->get_UserAgent(&userAgent);
	return userAgent;
}

void WebView::SetUserAgent(const autostr userAgent) {
	_webviewSettings2->put_UserAgent(userAgent);
}

HRESULT WebView::OnWebView2CreateEnvironmentCompleted(const HRESULT result, ICoreWebView2Environment* env) {
	if (result != S_OK) return result;
	const auto envResult = env->QueryInterface(&_webviewEnv);
	if (envResult != S_OK) return envResult;

	env->CreateCoreWebView2Controller(_hWndWnd,
		Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this,
			&WebView::OnWebView2CreateControllerCompleted).Get());

	return S_OK;
}

HRESULT WebView::OnWebView2CreateControllerCompleted(const HRESULT result, ICoreWebView2Controller* controller) {
	if (result != S_OK) return result;

	_onCreated();

	const auto controllerResult = controller->QueryInterface(&_webviewController);
	if (controllerResult != S_OK) return controllerResult;

	_webviewController->get_CoreWebView2(&_webview);

	_webviewController2 = _webviewController.query<ICoreWebView2Controller2>();
	_webviewController2->put_DefaultBackgroundColor({ 0, 0, 0, 0 });

	_webview->get_Settings(&_webviewSettings);
	_webviewSettings->put_AreHostObjectsAllowed(TRUE);
	_webviewSettings->put_IsScriptEnabled(TRUE);
	_webviewSettings->put_AreDefaultScriptDialogsEnabled(TRUE);
	_webviewSettings->put_IsWebMessageEnabled(TRUE);
	_webviewSettings->put_IsStatusBarEnabled(FALSE);
	_webviewSettings->put_AreDefaultContextMenusEnabled(_contextMenuEnabled);
	_webviewSettings->put_AreDevToolsEnabled(_devToolsEnabled);

	_webviewSettings2 = _webviewSettings.try_query<ICoreWebView2Settings2>();
	if (_userAgent) _webviewSettings2->put_UserAgent(_userAgent);

	EventRegistrationToken navigationStartingToken;
	_webview->add_NavigationStarting(Callback<ICoreWebView2NavigationStartingEventHandler>(
		[&](ICoreWebView2* _, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
		wil::unique_cotaskmem_string uri;
		if (const auto hr = args->get_Uri(&uri); hr != S_OK)
			return hr;
		_onNavigationStart(uri.get());
		return S_OK;
	}).Get(), &navigationStartingToken);

	EventRegistrationToken navigationCompletedToken;
	_webview->add_NavigationCompleted(Callback<ICoreWebView2NavigationCompletedEventHandler>([&](
		ICoreWebView2* _, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {
		_onNavigationEnd();
		return S_OK;
	}).Get(), &navigationCompletedToken);

	EventRegistrationToken webMessageReceivedToken;
	_webview->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>([&]
	(ICoreWebView2* _, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
		wil::unique_cotaskmem_string message;
		if (const auto hr = args->TryGetWebMessageAsString(&message); hr != S_OK)
			return hr;
		_onMessageReceived(message.get());
		return S_OK;
	}).Get(), &webMessageReceivedToken);

	EventRegistrationToken permissionRequestedToken;
	_webview->add_PermissionRequested(Callback<ICoreWebView2PermissionRequestedEventHandler>([&]
	(ICoreWebView2* _, ICoreWebView2PermissionRequestedEventArgs* args)->HRESULT {
		if (_grantPermissions)
			args->put_State(COREWEBVIEW2_PERMISSION_STATE_ALLOW);
		return S_OK;
	}).Get(), &permissionRequestedToken);

	EventRegistrationToken webResourceRequestedToken;
	_webview->AddWebResourceRequestedFilter(L"*", COREWEBVIEW2_WEB_RESOURCE_CONTEXT_ALL);
	_webview->add_WebResourceRequested(Callback<ICoreWebView2WebResourceRequestedEventHandler>([&]
	(ICoreWebView2* _, ICoreWebView2WebResourceRequestedEventArgs* args) -> HRESULT {
		ICoreWebView2WebResourceRequest* request;
		args->get_Request(&request);

		wil::unique_cotaskmem_string reqUri;
		request->get_Uri(&reqUri);

		wil::unique_cotaskmem_string reqMethod;
		request->get_Method(&reqMethod);

		const WebResourceRequest req{
			reqUri.get(),
			nullptr,
			reqMethod.get(),
			nullptr
		};
		WebResourceResponse res;
		_onResourceRequested(req, &res);

		const wil::unique_cotaskmem content(res.Content);

		if (content != nullptr) {
			const std::wstring contentTypeW(res.ContentTypeW);

			IStream* stream = SHCreateMemStream((BYTE*)content.get(), res.ContentLength);
			wil::com_ptr<ICoreWebView2WebResourceResponse> response;

			_webviewEnv->CreateWebResourceResponse(
				stream, 
				res.StatusCode, 
				res.ReasonPhraseW, 
				(L"" + contentTypeW).c_str(), 
				&response);

			args->put_Response(response.get());
		}

		return S_OK;
	}).Get(), &webResourceRequestedToken);

	if (_startUrl) 
		_webview->Navigate(_startUrl);
	else if (_startContent) 
		_webview->NavigateToString(_startContent);

	Refit(_window->GetBorderStyle());

	return S_OK;
}
