namespace Gluino.Interop;

[LibDetails("Gluino.Core", ManagedType = typeof(Window))]
internal partial class NativeWindow
{
    private const ManagedProperty PG = ManagedProperty.Get;
    private const ManagedProperty PS = ManagedProperty.Set;

    [LibImport("Gluino_Window_Show")]
    public static partial void Show(nint window);

    [LibImport("Gluino_Window_Hide")]
    public static partial void Hide(nint window);

    [LibImport("Gluino_Window_Close")]
    public static partial void Close(nint window);

    [LibImport("Gluino_Window_Center")]
    public static partial void Center(nint window);

    [LibImport("Gluino_Window_DragMove")]
    public static partial void DragMove(nint window);

    [LibImport("Gluino_Window_Invoke")]
    public static partial void Invoke(nint window, NativeDelegate action);

    [LibImport("Gluino_Window_GetHandle")]
    public static partial nint GetHandle(nint window);

    [LibImport("Gluino_Window_GetBounds")]
    public static partial void GetBounds(nint window, out NativeRect bounds);

    [LibImport("Gluino_Window_GetIsDarkMode")]
    public static partial bool GetIsDarkMode(nint window);

    [LibImport("Gluino_Window_GetTitle", Managed = true, Property = PG, Option = "Title")]
    public static partial string GetTitle(nint window);

    [LibImport("Gluino_Window_SetTitle", Managed = true, Property = PS, Option = "Title")]
    public static partial void SetTitle(nint window, string title);

    [LibImport("Gluino_Window_GetIcon")]
    public static partial void GetIcon(nint window, out nint data, out int size);

    [LibImport("Gluino_Window_SetIcon")]
    public static partial void SetIcon(nint window, nint data, int size);

    [LibImport("Gluino_Window_GetBorderStyle", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.BorderStyle))]
    public static partial WindowBorderStyle GetBorderStyle(nint window);

    [LibImport("Gluino_Window_SetBorderStyle", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.BorderStyle))]
    public static partial void SetBorderStyle(nint window, WindowBorderStyle style);

    [LibImport("Gluino_Window_GetWindowState", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.WindowState))]
    public static partial WindowState GetWindowState(nint window);

    [LibImport("Gluino_Window_SetWindowState", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.WindowState))]
    public static partial void SetWindowState(nint window, WindowState state);

    [LibImport("Gluino_Window_GetTheme", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.Theme))]
    public static partial WindowTheme GetTheme(nint window);

    [LibImport("Gluino_Window_SetTheme", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.Theme))]
    public static partial void SetTheme(nint window, WindowTheme theme);

    [LibImport("Gluino_Window_GetMinimumSize", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.MinimumSize))]
    public static partial NativeSize GetMinimumSize(nint window);

    [LibImport("Gluino_Window_SetMinimumSize", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.MinimumSize))]
    public static partial NativeSize SetMinimumSize(nint window, NativeSize size);

    [LibImport("Gluino_Window_GetMaximumSize", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.MaximumSize))]
    public static partial NativeSize GetMaximumSize(nint window);

    [LibImport("Gluino_Window_SetMaximumSize", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.MaximumSize))]
    public static partial NativeSize SetMaximumSize(nint window, NativeSize size);

    [LibImport("Gluino_Window_GetSize", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.Size))]
    public static partial NativeSize GetSize(nint window);

    [LibImport("Gluino_Window_SetSize", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.Size))]
    public static partial NativeSize SetSize(nint window, NativeSize size);

    [LibImport("Gluino_Window_GetLocation", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.Location))]
    public static partial NativePoint GetLocation(nint window);

    [LibImport("Gluino_Window_SetLocation", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.Location))]
    public static partial NativePoint SetLocation(nint window, NativePoint location);

    [LibImport("Gluino_Window_GetMinimizeEnabled", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.MinimizeEnabled))]
    public static partial bool GetMinimizeEnabled(nint window);

    [LibImport("Gluino_Window_SetMinimizeEnabled", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.MinimizeEnabled))]
    public static partial void SetMinimizeEnabled(nint window, bool enabled);

    [LibImport("Gluino_Window_GetMaximizeEnabled", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.MaximizeEnabled))]
    public static partial bool GetMaximizeEnabled(nint window);

    [LibImport("Gluino_Window_SetMaximizeEnabled", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.MaximizeEnabled))]
    public static partial void SetMaximizeEnabled(nint window, bool enabled);

    [LibImport("Gluino_Window_GetTopMost", Managed = true, Property = PG, Option = nameof(NativeWindowOptions.TopMost))]
    public static partial bool GetTopMost(nint window);

    [LibImport("Gluino_Window_SetTopMost", Managed = true, Property = PS, Option = nameof(NativeWindowOptions.TopMost))]
    public static partial void SetTopMost(nint window, bool topMost);
}