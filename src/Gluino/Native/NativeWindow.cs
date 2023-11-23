using System.Runtime.InteropServices;

namespace Gluino.Native;

[UnmanagedFunctionPointer(CallingConvention.Cdecl)] internal delegate void NativeAction();

[LibDetails("GluinoNative")]
internal partial class NativeWindow
{
    [LibImport("Gluino_Window_Show")] public static partial void Show(nint window);
    [LibImport("Gluino_Window_Hide")] public static partial void Hide(nint window);
    [LibImport("Gluino_Window_Close")] public static partial void Close(nint window);
    [LibImport("Gluino_Window_Invoke")] public static partial void Invoke(nint window, NativeAction action);
    [LibImport("Gluino_Window_SetTitle")] public static partial void SetTitle(nint window, string title);
}