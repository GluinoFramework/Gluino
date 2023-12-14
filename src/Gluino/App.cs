using Gluino.Interop;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Gluino;

/// <summary>
/// Represents the application.
/// </summary>
public static class App
{
    internal static readonly nint AppHInstance;
    internal static readonly nint NativeInstance;

    static App()
    {
        AppHInstance = NativeLibrary.GetMainProgramHandle();

        if (Platform.IsWindows) {
            NativeInstance = NativeApp.Create(AppHInstance, GetAppId());
        }
        else {
            //TODO: NativeInstance = NativeApp.Create();
        }
    }
    
    /// <summary>
    /// Gets or sets the application name.
    /// </summary>
    public static string Name { get; set; } = Assembly.GetEntryAssembly()?.GetName().Name ?? "Gluino";
    
    /// <summary>
    /// Gets the main <see cref="Window"/>.
    /// </summary>
    public static Window MainWindow { get; internal set; }

    /// <summary>
    /// Gets the collection of active windows.
    /// </summary>
    public static WindowCollection ActiveWindows { get; } = [];

    /// <summary>
    /// Gets the <see cref="Platform"/> instance.
    /// </summary>
    public static Platform Platform { get; } = new();

    /// <summary>
    /// Runs the application, starting with the specified <see cref="Window"/> as the main window.
    /// </summary>
    /// <param name="mainWindow">The main <see cref="Window"/> to start with.</param>
    /// <exception cref="InvalidOperationException">The application is already running.</exception>
    /// <remarks>
    /// Should be executed on STA thread if running on Windows.<br />
    /// Blocks the thread until the exited.
    /// </remarks>
    public static void Run(Window mainWindow)
    {
        if (MainWindow != null)
            throw new InvalidOperationException("The application is already running");

        MainWindow = mainWindow;
        MainWindow.IsMain = true;
        MainWindow.Show();

        NativeApp.Run(NativeInstance);
    }

    /// <summary>
    /// Exits the application.
    /// </summary>
    /// <exception cref="InvalidOperationException">The application is not running.</exception>
    public static void Exit()
    {
        if (NativeInstance == nint.Zero)
            throw new InvalidOperationException("The application is not running");

        NativeApp.Exit(NativeInstance);
    }

    private static string GetAppId()
    {
        var ass = Assembly.GetEntryAssembly();
        if (ass == null) return Name;
        var companyAttr = ass.GetCustomAttribute<AssemblyCompanyAttribute>();
        var productAttr = ass.GetCustomAttribute<AssemblyProductAttribute>();
        var titleAttr = ass.GetCustomAttribute<AssemblyTitleAttribute>();
        var id = string.Join(".", new[] {
            companyAttr?.Company,
            productAttr?.Product,
            titleAttr?.Title
        }.Where(x => x != null));

        return string.IsNullOrEmpty(id) || string.IsNullOrWhiteSpace(id) ? Name : id;
    }
}