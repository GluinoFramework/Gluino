using System.Runtime.InteropServices;

namespace Gluino.Native;

internal partial class NativeWindow
{
    private const string LibName = "GluinoNative";

    [LibraryImport(LibName, EntryPoint = "Gluino_Window_Create")]
    public static partial IntPtr Create(ref NativeWindowOptions options);

    [LibraryImport(LibName, EntryPoint = "Gluino_Window_Destroy")]
    public static partial void Destroy(nint window);

    [LibraryImport(LibName, EntryPoint = "Gluino_Window_Show")]
    public static partial void Show(nint window);

    [LibraryImport(LibName, EntryPoint = "Gluino_Window_Hide")]
    public static partial void Hide(nint window);
}