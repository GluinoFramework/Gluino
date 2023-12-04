using System.Runtime.InteropServices;

namespace Gluino.Native;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeWindowOptions
{
    [MarshalAs(UnmanagedType.LPWStr)] public string ClassName;
    [MarshalAs(UnmanagedType.I1)] public bool IsMain;
    [MarshalAs(UnmanagedType.LPWStr)] public string TitleW;
    [MarshalAs(UnmanagedType.LPStr)] public string TitleA;
    public WindowBorderStyle BorderStyle;
    public WindowState WindowState;
    public NativeSize MinimumSize;
    public NativeSize MaximumSize;
    public NativeSize Size;
    public NativePoint Location;
    public WindowStartupLocation StartupLocation;
    [MarshalAs(UnmanagedType.I1)] public bool TopMost;
}