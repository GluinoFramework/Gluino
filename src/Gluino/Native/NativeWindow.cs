namespace Gluino.Native;

[LibDetails("GluinoNative")]
internal partial class NativeWindow
{
    [LibImport("Gluino_Window_Create")]
    public static partial nint Create(ref NativeWindowOptions options);

    [LibImport("Gluino_Window_Destroy")]
    public static partial void Destroy(nint window);

    [LibImport("Gluino_Window_Show")]
    public static partial void Show(nint window);

    [LibImport("Gluino_Window_Hide")]
    public static partial void Hide(nint window);

    [LibImport("Gluino_Window_Close")]
    public static partial void Close(nint window);

    [LibImport("Gluino_Window_SetTitle")]
    public static partial void SetTitle(nint window, string title);
}