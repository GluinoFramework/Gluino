using System.Reflection;
using Gluino;
using TestApp.Properties;

namespace TestApp;

internal class Program
{
    [STAThread]
    public static void Main()
    {
        var htmlContent = string.Empty;
        var assembly = Assembly.GetExecutingAssembly();
        var indexResource = assembly.GetManifestResourceNames().FirstOrDefault(x => x.EndsWith("index.html"));
        if (indexResource != null) {
            using var stream = assembly.GetManifestResourceStream(indexResource);
            using var reader = new StreamReader(stream!);
            htmlContent = reader.ReadToEnd();
        }

        var window = new Window {
            Title = "Test Window",
            Icon = Resources.icon,
            //BorderStyle = WindowBorderStyle.SizableNoCaption,
            //WindowState = WindowState.Maximized,
            //Theme = WindowTheme.System,
            //Resizable = true,
            //TopMost = false,
            //StartupLocation = WindowStartupLocation.Default,
            //MinimizeEnabled = true,
            //MaximizeEnabled = false,

            WebView = {
                //StartUrl = "https://google.com",
                StartContent = htmlContent,
                ContextMenuEnabled = true,
                DevToolsEnabled = true
            }
        };
        var webView = window.WebView;

        webView.Bind("test", Test);

        window.Creating += (_, _) => LogWindowEvent("Creating");
        window.Created += (_, _) => {
            //window.Icon = Resources.icon;
            LogWindowEvent("Created");
        };
        window.Shown += (_, _) => LogWindowEvent("Shown");
        window.Hidden += (_, _) => LogWindowEvent("Hidden");
        window.Resize += (_, e) => LogWindowEvent($"Resize: {e.Width}, {e.Height}");
        window.ResizeStart += (_, e) => LogWindowEvent($"ResizeStart: {e.Width}, {e.Height}");
        window.ResizeEnd += (_, e) => LogWindowEvent($"ResizeEnd: {e.Width}, {e.Height}");
        window.LocationChanged += (_, e) => LogWindowEvent($"LocationChanged: {e.X}, {e.Y}");
        window.WindowStateChanged += (_, e) => LogWindowEvent($"WindowStateChanged: {e.WindowState}");
        window.FocusIn += (_, _) => {
            window.WebView.SendMessage("window-focus-in");

            LogWindowEvent("FocusIn");
        };
        window.FocusOut += (_, _) => {
            window.WebView.SendMessage("window-focus-out");

            LogWindowEvent("FocusOut");
        };
        window.Closing += (_, _) => LogWindowEvent("Closing");
        window.Closed += (_, _) => LogWindowEvent("Closed");

        webView.Created += (_, _) => LogWebViewEvent("Created");
        webView.NavigationStart += (_, e) => LogWebViewEvent($"NavigationStart: {e.Url}");
        webView.NavigationEnd += (_, _) => LogWebViewEvent("NavigationEnd");
        webView.MessageReceived += (_, e) => LogWebViewEvent($"MessageReceived: {e}");

        var resourceDir = $"{assembly.GetName().Name}.wwwroot";
        webView.ResourceRequested += (_, e) => {
            if (!e.Request.Url.StartsWith("app://")) {
                return;
            }

            var path = e.Request.Url[6..];
            var resourceName = $"{resourceDir}.{path.Trim('/').Replace('/', '.')}";
            var resource = assembly.GetManifestResourceStream(resourceName);

            e.Response.Content = resource;
            e.Response.ContentType = Path.GetExtension(path) switch {
                ".html" => "text/html",
                ".css" => "text/css",
                ".js" => "text/javascript",
                ".png" => "image/png",
                ".jpg" => "image/jpeg",
                ".jpeg" => "image/jpeg",
                ".gif" => "image/gif",
                ".svg" => "image/svg+xml",
                ".ico" => "image/x-icon",
                ".json" => "application/json",
                ".woff" => "font/woff",
                ".woff2" => "font/woff2",
                ".ttf" => "font/ttf",
                ".otf" => "font/otf",
                ".eot" => "application/vnd.ms-fontobject",
                ".sfnt" => "application/font-sfnt",
                ".xml" => "text/xml",
                ".txt" => "text/plain",
                _ => "application/octet-stream"
            };
            e.Response.StatusCode = 200;

            LogWebViewEvent($"ResourceRequested: {e.Request.Url}");
        };

        App.Run(window);
    }

    private static string Test(string arg1, string arg2)
    {
        Console.WriteLine($"[METHOD] [WebView] Test: {arg1}, {arg2}");
        return "Test result";
    }

    private static void LogWindowEvent(string message) => Console.WriteLine("[EVENT] [Window] {0}", message);
    private static void LogWebViewEvent(string message) => Console.WriteLine("[EVENT] [WebView] {0}", message);
}
