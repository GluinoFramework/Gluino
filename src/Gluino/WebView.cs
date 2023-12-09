using System.Text.RegularExpressions;
using Gluino.Native;

namespace Gluino;

/// <summary>
/// Represents the embedded WebView control.
/// </summary>
public partial class WebView
{
    [GeneratedRegex("^https?://", RegexOptions.IgnoreCase | RegexOptions.Compiled, "en-US")]
    private static partial Regex CreateHttpRegex();
    private static readonly Regex HttpRegex = CreateHttpRegex();

    private readonly Window _window;

    internal nint InstancePtr;
    internal NativeWebViewOptions NativeOptions;
    internal NativeWebViewEvents NativeEvents;

    /// <summary>
    /// Occurs when the WebView is created.
    /// </summary>
    public event EventHandler Created;
    /// <summary>
    /// Occurs when the WebView begins navigating to a new URL.
    /// </summary>
    public event EventHandler<NavigationStartEventArgs> NavigationStart;
    /// <summary>
    /// Occurs when the WebView finishes navigating.
    /// </summary>
    public event EventHandler NavigationEnd;
    /// <summary>
    /// Occurs when the WebView receives a message from the page.
    /// </summary>
    public event EventHandler<string> MessageReceived;
    /// <summary>
    /// Occurs when the WebView requests a resource.
    /// </summary>
    public event EventHandler<WebResourceRequestedEventArgs> ResourceRequested;

    internal WebView(Window window)
    {
        NativeOptions = new() {
            ContextMenuEnabled = true
        };

        NativeEvents = new() {
            OnCreated = InvokeCreated,
            OnNavigationStart = InvokeNavigationStart,
            OnNavigationEnd = InvokeNavigationEnd,
            OnMessageReceived = InvokeMessageReceived,
            OnResourceRequested = InvokeResourceRequested
        };

        _window = window;
    }

    /// <summary>
    /// Gets or sets the URL to load when the WebView is created.
    /// </summary>
    public string StartUrl {
        get => App.Platform.IsWindows ? NativeOptions.StartUrlW : NativeOptions.StartUrlA;
        set {
            if (App.Platform.IsWindows) NativeOptions.StartUrlW = value;
            else NativeOptions.StartUrlA = value;
        }
    }

    /// <summary>
    /// Gets or sets the HTML content to load when the WebView is created.
    /// </summary>
    public string StartContent {
        get => App.Platform.IsWindows ? NativeOptions.StartContentW : NativeOptions.StartContentA;
        set {
            if (App.Platform.IsWindows) NativeOptions.StartContentW = value;
            else NativeOptions.StartContentA = value;
        }
    }

    /// <summary>
    /// Gets or sets whether the WebView should display a context menu when right-clicked.
    /// </summary>
    /// <remarks>
    /// Default: true
    /// </remarks>
    public bool ContextMenuEnabled {
        get => GetContextMenuEnabled();
        set => SetContextMenuEnabled(value);
    }

    /// <summary>
    /// Gets or sets whether to enable the WebView's dev tools.
    /// </summary>
    /// <remarks>
    /// Default: false
    /// </remarks>
    public bool DevToolsEnabled {
        get => GetDevToolsEnabled();
        set => SetDevToolsEnabled(value);
    }

    /// <summary>
    /// Gets or sets the user agent string to use when making requests.
    /// </summary>
    public string UserAgent {
        get => GetUserAgent();
        set => SetUserAgent(value);
    }

    /// <summary>
    /// Gets or sets whether the WebView should be granted permissions to access local resources (camera, microphone, etc).
    /// </summary>
    /// <remarks>
    /// Default: false<br />
    /// Windows only.
    /// </remarks>
    public bool GrantPermissions {
        get => GetGrantPermissions();
        set {
            if (InstancePtr != nint.Zero) return;
            NativeOptions.GrantPermissions = value;
        }
    }

    /// <summary>
    /// Navigates to the specified URL or file.
    /// </summary>
    /// <param name="url">The URL or file to navigate to.</param>
    public void Navigate(string url)
    {
        if (HttpRegex.IsMatch(url)) {
            Navigate(new Uri(url));
            return;
        }

        var fullPath = Path.GetFullPath(url);
        if (!File.Exists(fullPath)) 
            throw new FileNotFoundException("The specified URL or file could not be found.", url);

        Navigate(new Uri(fullPath, UriKind.Absolute));
    }

    /// <summary>
    /// Navigates to the specified URL.
    /// </summary>
    /// <param name="uri"> The URL to navigate to.</param>
    public void Navigate(Uri uri)
    {
        if (InstancePtr == nint.Zero) {
            if (App.Platform.IsWindows) NativeOptions.StartUrlW = uri.ToString();
            else NativeOptions.StartUrlA = uri.ToString();
            return;
        }

        Invoke(() => NativeWebView.Navigate(InstancePtr, uri.ToString()));
    }

    /// <summary>
    /// Navigates to the specified HTML content.
    /// </summary>
    /// <param name="content">The HTML content to navigate to.</param>
    public void NativateToString(string content)
    {
        if (InstancePtr == nint.Zero) {
            if (App.Platform.IsWindows) NativeOptions.StartContentW = content;
            else NativeOptions.StartContentA = content;
            return;
        }

        Invoke(() => NativeWebView.NativateToString(InstancePtr, content));
    }

    /// <summary>
    /// Sends a message to the WebView.
    /// </summary>
    /// <param name="message">The message to send.</param>
    public void SendMessage(string message) => SafeInvoke(() => NativeWebView.PostWebMessage(InstancePtr, message));

    private void Invoke(Action action) => _window.Invoke(action);
    private void SafeInvoke(Action action) => _window.SafeInvoke(action);
    private T SafeInvoke<T>(Func<T> func) => _window.SafeInvoke(func);
    
    private void InvokeCreated() => Created?.Invoke(this, EventArgs.Empty);
    private void InvokeNavigationStart(string url) => NavigationStart?.Invoke(this, new (url));
    private void InvokeNavigationEnd() => NavigationEnd?.Invoke(this, EventArgs.Empty);
    private void InvokeMessageReceived(string message) => MessageReceived?.Invoke(this, message);

    private void InvokeResourceRequested(NativeWebResourceRequest request, out NativeWebResourceResponse response)
    {
        var req = new WebResourceRequest(request);
        var res = new WebResourceResponse();

        ResourceRequested?.Invoke(this, new(req, res));

        response = res.Native;
    }
}