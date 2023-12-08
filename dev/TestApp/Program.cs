using Gluino;

namespace TestApp;

internal class Program
{
    [STAThread]
    public static void Main()
    {
        var htmlContent = """
            <!DOCTYPE html>
            <html lang="en">
            <head>
              <meta charset="UTF-8">
              <meta name="viewport" content="width=device-width, initial-scale=1.0">
              <title>Test</title>
            
              <style>
                html {
                  padding: 0;
                  margin: 0;
                  box-sizing: border-box;
                  border: 1px solid red;
                  height: 100vh;
                }
            
                html, body {
                  background-color: transparent;
                  color: pink;
                  font-family: "Segoe UI", sans-serif;
                  font-size: 14px;
                  font-weight: 400;
                }
              </style>
            </head>
            <body>
              This is a test.
            </body>
            </html>
            """;

        var window = new Window {
            Title = "Test Window",
            //BorderStyle = WindowBorderStyle.SizableNoCaption,
            //WindowState = WindowState.Maximized,
            Theme = WindowTheme.System,
            //Resizable = true,
            //TopMost = false,
            //StartupLocation = WindowStartupLocation.Default,
            //MinimizeEnabled = true,
            //MaximizeEnabled = false

            WebView = {
                //StartUrl = "https://google.com",
                StartContent = htmlContent,
                ContextMenuEnabled = true,
                DevToolsEnabled = true
            }
        };

        window.Creating += (_, _) => LogWindowEvent("Creating");
        window.Created += (_, _) => LogWindowEvent("Created");
        window.Shown += (_, _) => LogWindowEvent("Shown");
        window.Hidden += (_, _) => LogWindowEvent("Hidden");
        window.Resize += (_, e) => LogWindowEvent($"Resize: {e.Width}, {e.Height}");
        window.ResizeStart += (_, e) => LogWindowEvent($"ResizeStart: {e.Width}, {e.Height}");
        window.ResizeEnd += (_, e) => LogWindowEvent($"ResizeEnd: {e.Width}, {e.Height}");
        window.LocationChanged += (_, e) => LogWindowEvent($"LocationChanged: {e.X}, {e.Y}");
        window.WindowStateChanged += (_, e) => LogWindowEvent($"WindowStateChanged: {e.WindowState}");
        window.FocusIn += (_, _) => LogWindowEvent("FocusIn");
        window.FocusOut += (_, _) => LogWindowEvent("FocusOut");
        window.Closing += (_, _) => LogWindowEvent("Closing");
        window.Closed += (_, _) => LogWindowEvent("Closed");

        window.WebView.Created += (_, _) => LogWebViewEvent("WebView Created");
        window.WebView.NavigationStart += (_, e) => LogWebViewEvent($"WebView NavigationStart: {e.Url}");
        window.WebView.NavigationEnd += (_, _) => LogWebViewEvent("WebView NavigationEnd");
        window.WebView.MessageReceived += (_, e) => LogWebViewEvent($"WebView MessageReceived: {e}");

        App.Run(window);
    }
    
    private static void LogWindowEvent(string message) => Console.WriteLine("[EVENT] [Window] {0}", message);
    private static void LogWebViewEvent(string message) => Console.WriteLine("[EVENT] [WebView] {0}", message);
}
