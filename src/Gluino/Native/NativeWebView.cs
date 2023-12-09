namespace Gluino.Native;

[LibDetails("GluinoNative", ManagedType = typeof(WebView))]
internal partial class NativeWebView
{
    private const ManagedProperty PG = ManagedProperty.Get;
    private const ManagedProperty PS = ManagedProperty.Set;

    [LibImport("Gluino_WebView_LoadUrl")] public static partial void Navigate(nint webView, string url);
    [LibImport("Gluino_WebView_NativateToString")] public static partial void NativateToString(nint webView, string content);
    [LibImport("Gluino_WebView_PostWebMessage")] public static partial void PostWebMessage(nint webView, string message);

    [LibImport("Gluino_WebView_GetGrantPermissions", Managed = true, Property = PG, Option = nameof(NativeWebViewOptions.GrantPermissions))]
    public static partial bool GetGrantPermissions(nint webView);

    [LibImport("Gluino_WebView_GetContextMenuEnabled", Managed = true, Property = PG, Option = nameof(NativeWebViewOptions.ContextMenuEnabled))] 
    public static partial bool GetContextMenuEnabled(nint webView);

    [LibImport("Gluino_WebView_SetContextMenuEnabled", Managed = true, Property = PS, Option = nameof(NativeWebViewOptions.ContextMenuEnabled))] 
    public static partial void SetContextMenuEnabled(nint webView, bool enabled);

    [LibImport("Gluino_WebView_GetDevToolsEnabled", Managed = true, Property = PG, Option = nameof(NativeWebViewOptions.DevToolsEnabled))] 
    public static partial bool GetDevToolsEnabled(nint webView);

    [LibImport("Gluino_WebView_SetDevToolsEnabled", Managed = true, Property = PS, Option = nameof(NativeWebViewOptions.DevToolsEnabled))] 
    public static partial void SetDevToolsEnabled(nint webView, bool enabled);

    [LibImport("Gluino_WebView_GetUserAgent", Managed = true, Property = PG, Option = "UserAgent")] 
    public static partial string GetUserAgent(nint webView);

    [LibImport("Gluino_WebView_SetUserAgent", Managed = true, Property = PS, Option = "UserAgent")] 
    public static partial void SetUserAgent(nint webView, string userAgent);
}