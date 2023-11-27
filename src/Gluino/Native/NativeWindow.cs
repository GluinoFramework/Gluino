using System.Runtime.InteropServices;

namespace Gluino.Native;

[LibDetails("GluinoNative", ManagedType = typeof(Window))]
internal partial class NativeWindow
{
    private const ManagedProperty PG = ManagedProperty.Get;
    private const ManagedProperty PS = ManagedProperty.Set;

    [LibImport("Gluino_Window_Show", Managed = true)]
    public static partial void Show(nint window);

    [LibImport("Gluino_Window_Hide", Managed = true)]
    public static partial void Hide(nint window);

    [LibImport("Gluino_Window_Close", Managed = true)]
    public static partial void Close(nint window);

    [LibImport("Gluino_Window_Invoke")]
    public static partial void Invoke(nint window, NativeDelegate action);

    [LibImport("Gluino_Window_GetBounds")]
    public static partial void GetBounds(nint window, out NativeRect bounds);

    [LibImport("Gluino_Window_GetTitle", Managed = true, Property = PG, Option = "Title")]
    public static partial string GetTitle(nint window);

    [LibImport("Gluino_Window_SetTitle", Managed = true, Property = PS, Option = "Title")]
    public static partial void SetTitle(nint window, string title);

    [LibImport("Gluino_Window_GetWindowStyle", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.WindowStyle))]
    public static partial WindowStyle GetWindowStyle(nint window);

    [LibImport("Gluino_Window_SetWindowStyle", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.WindowStyle))]
    public static partial void SetWindowStyle(nint window, WindowStyle style);

    [LibImport("Gluino_Window_GetWindowState", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.WindowState))]
    public static partial WindowState GetWindowState(nint window);

    [LibImport("Gluino_Window_SetWindowState", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.WindowState))]
    public static partial void SetWindowState(nint window, WindowState state);
}