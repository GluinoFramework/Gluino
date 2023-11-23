using System.Runtime.InteropServices;

namespace Gluino.Native;

[LibDetails("GluinoNative")]
internal partial class NativeApp
{
    [LibImport("Gluino_App_Create")] public static partial nint Create(nint hInstance);
    [LibImport("Gluino_App_Destroy")] public static partial void Destroy(nint app);
    [LibImport("Gluino_App_SpawnWindow")] public static partial nint SpawnWindow(nint app, ref NativeWindowOptions options);
    [LibImport("Gluino_App_DespawnWindow")] public static partial void DespawnWindow(nint app, nint window);
    [LibImport("Gluino_App_Run")] public static partial void Run(nint app);
    [LibImport("Gluino_App_Exit")] public static partial void Exit(nint app);

    [DllImport("shell32.dll", SetLastError = true)]
    public static extern void SetCurrentProcessExplicitAppUserModelID([MarshalAs(UnmanagedType.LPWStr)] string appId);
}