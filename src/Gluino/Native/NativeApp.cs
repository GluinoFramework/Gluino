using System.Runtime.InteropServices;

namespace Gluino.Native;

[LibDetails("GluinoNative")]
internal partial class NativeApp
{
    [LibImport("Gluino_App_Create")]
    public static partial nint Create();

    [LibImport("Gluino_App_Destroy")]
    public static partial void Destroy(nint app);

    [LibImport("Gluino_App_Register")]
    public static partial void Register(nint app, nint hInstance, string className);

    [LibImport("Gluino_App_Run")]
    public static partial void Run(nint app);

    [LibImport("Gluino_App_Exit")]
    public static partial void Exit(nint app);

    [DllImport("shell32.dll", SetLastError = true)]
    public static extern void SetCurrentProcessExplicitAppUserModelID([MarshalAs(UnmanagedType.LPWStr)] string appId);
}