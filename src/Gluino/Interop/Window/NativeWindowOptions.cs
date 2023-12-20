using System.Runtime.InteropServices;

namespace Gluino.Interop;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeWindowOptions
{
    [MarshalAs(UnmanagedType.I1)] public bool IsMain;
    [MarshalAs(UnmanagedType.LPStr)] public string Title;
    public nint Icon;
    [MarshalAs(UnmanagedType.I4)] public int IconSize;
    public WindowBorderStyle BorderStyle;
    public WindowState WindowState;
    public WindowStartupLocation StartupLocation;
    public WindowTheme Theme;
    public NativeSize MinimumSize;
    public NativeSize MaximumSize;
    public NativeSize Size;
    public NativePoint Location;
    [MarshalAs(UnmanagedType.I1)] public bool MinimizeEnabled;
    [MarshalAs(UnmanagedType.I1)] public bool MaximizeEnabled;
    [MarshalAs(UnmanagedType.I1)] public bool TopMost;
}