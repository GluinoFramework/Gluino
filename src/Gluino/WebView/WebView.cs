using System.Text.RegularExpressions;
using Gluino.Interop;

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
    private readonly WebViewBinder _binder;

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
        _binder = new WebViewBinder(this);
    }

    /// <summary>
    /// Gets or sets the URL to load when the WebView is created.
    /// </summary>
    public string StartUrl {
        get => NativeOptions.StartUrl;
        set => NativeOptions.StartUrl = value;
    }

    /// <summary>
    /// Gets or sets the HTML content to load when the WebView is created.
    /// </summary>
    public string StartContent {
        get => NativeOptions.StartContent;
        set => NativeOptions.StartContent = value;
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
            NativeOptions.StartUrl = uri.ToString();
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
            NativeOptions.StartContent = content;
            return;
        }

        Invoke(() => NativeWebView.NativateToString(InstancePtr, content));
    }

    /// <summary>
    /// Sends a message to the WebView.
    /// </summary>
    /// <param name="message">The message to send.</param>
    public void SendMessage(string message) => SafeInvoke(() => NativeWebView.PostWebMessage(InstancePtr, message));

    /// <summary>
    /// Injects the specified JavaScript code.
    /// </summary>
    /// <param name="script">The JavaScript code to inject.</param>
    public void InjectScript(string script) => SafeInvoke(() => NativeWebView.InjectScript(InstancePtr, script, false));

    /// <summary>
    /// Injects the specified JavaScript code when the document is created.
    /// </summary>
    /// <param name="script">The JavaScript code to inject.</param>
    public void InjectScriptOnDocumentCreated(string script) => SafeInvoke(() => NativeWebView.InjectScript(InstancePtr, script, true));
    
    /// <summary>
    /// Bind a C# method to JavaScript.
    /// </summary>
    /// <param name="name">The name of the function that will be created in JavaScript.</param>
    /// <param name="fn">The method to bind.</param>
    /// <remarks>
    /// Example binding:
    /// <code>
    /// // C#
    /// webView.Bind("test", (string arg1, string arg2) => {
    ///     Console.WriteLine($"arg1: {arg1}, arg2: {arg2}"); // arg1: Hello, arg2: World!
    ///     return "Hello from C#!";
    /// });
    /// </code>
    /// <code>
    /// // JavaScript
    /// const result = await window.gluino.test("Hello", "World!");
    /// cosnole.log(result); // Hello from C#!
    /// </code>
    /// </remarks>
    public void Bind(string name, Delegate fn) => _binder.Bind(name, fn);

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