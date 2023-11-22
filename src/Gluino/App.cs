using Gluino.Native;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Gluino;

public static class App
{
    internal static readonly nint NativeInstance;
    internal static readonly nint AppHInstance;

    static App()
    {
        NativeInstance = NativeApp.Create();
        AppHInstance = NativeLibrary.GetMainProgramHandle();
    }
    
    public static string Name { get; set; } = Assembly.GetEntryAssembly()?.GetName().Name ?? "Gluino";
    
    public static Window MainWindow { get; internal set; }

    public static WindowCollection ActiveWindows { get; } = [];

    public static Platform Platform { get; } = new();

    public static void Run(Window mainWindow)
    {
        if (MainWindow != null)
            throw new InvalidOperationException("The application is already running");
        
        if (Platform.IsWindows) {
            SetWin32AppId();
            NativeApp.Register(NativeInstance, AppHInstance, Name);
        }

        MainWindow = mainWindow;

        NativeApp.Run(NativeInstance);
    }

    public static void Exit()
    {
        if (NativeInstance == nint.Zero)
            throw new InvalidOperationException("The application is not running");

        NativeApp.Exit(NativeInstance);
    }

    private static void SetWin32AppId()
    {
        var ass = Assembly.GetEntryAssembly();
        if (ass == null) return;
        var companyAttr = ass.GetCustomAttribute<AssemblyCompanyAttribute>();
        var productAttr = ass.GetCustomAttribute<AssemblyProductAttribute>();
        var titleAttr = ass.GetCustomAttribute<AssemblyTitleAttribute>();
        var id = string.Join(".", new[] {
            companyAttr?.Company,
            productAttr?.Product,
            titleAttr?.Title
        }.Where(x => x != null));

        NativeApp.SetCurrentProcessExplicitAppUserModelID(string.IsNullOrEmpty(id) || string.IsNullOrWhiteSpace(id) ? Name : id);
    }
}