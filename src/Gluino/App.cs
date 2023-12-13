using Gluino.Native;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Gluino;

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
    
    public static string Name { get; set; } = Assembly.GetEntryAssembly()?.GetName().Name ?? "Gluino";
    
    public static Window MainWindow { get; internal set; }

    public static WindowCollection ActiveWindows { get; } = [];

    public static Platform Platform { get; } = new();

    public static void Run(Window mainWindow)
    {
        if (MainWindow != null)
            throw new InvalidOperationException("The application is already running");

        MainWindow = mainWindow;
        MainWindow.IsMain = true;
        MainWindow.Show();

        NativeApp.Run(NativeInstance);
    }

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