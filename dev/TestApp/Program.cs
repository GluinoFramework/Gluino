using Gluino;
using Gluino.Events;

namespace TestApp;

internal class Program
{
    [STAThread]
    public static void Main()
    {
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
        };

        window.Creating += (_, _) => LogEvent("Creating");
        window.Created += (_, _) => LogEvent("Created");
        window.Shown += (_, _) => LogEvent("Shown");
        window.Hidden += (_, _) => LogEvent("Hidden");
        window.Resize += (_, e) => LogEvent($"Resize: {e.Width}, {e.Height}");
        window.ResizeStart += (_, e) => LogEvent($"ResizeStart: {e.Width}, {e.Height}");
        window.ResizeEnd += (_, e) => LogEvent($"ResizeEnd: {e.Width}, {e.Height}");
        window.LocationChanged += (_, e) => LogEvent($"LocationChanged: {e.X}, {e.Y}");
        window.WindowStateChanged += (_, e) => LogEvent($"WindowStateChanged: {e.WindowState}");
        window.FocusIn += (_, _) => LogEvent("FocusIn");
        window.FocusOut += (_, _) => LogEvent("FocusOut");
        window.Closing += (_, _) => LogEvent("Closing");
        window.Closed += (_, _) => LogEvent("Closed");

        App.Run(window);
    }
    
    private static void LogEvent(string message)
    {
        Console.WriteLine("[EVENT] {0}", message);
    }
}
