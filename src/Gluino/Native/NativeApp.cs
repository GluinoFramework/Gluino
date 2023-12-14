using System.Runtime.InteropServices;

namespace Gluino.Native;

[LibDetails("Gluino.Core")]
internal partial class NativeApp
{
    [LibImport("Gluino_App_Create")] public static partial nint Create(nint hInstance, string appId);
    [LibImport("Gluino_App_Destroy")] public static partial void Destroy(nint app);
    [LibImport("Gluino_App_SpawnWindow")]
    public static partial void SpawnWindow(nint app,
        ref NativeWindowOptions windowOptions, ref NativeWindowEvents windowEvents,
        ref NativeWebViewOptions webViewOptions, ref NativeWebViewEvents webViewEvents,
        out nint window, out nint webView);
    [LibImport("Gluino_App_DespawnWindow")] public static partial void DespawnWindow(nint app, nint window);
    [LibImport("Gluino_App_Run")] public static partial void Run(nint app);
    [LibImport("Gluino_App_Exit")] public static partial void Exit(nint app);
}