using System.Runtime.InteropServices;

namespace Gluino.Native;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeWindowOptions
{
    [MarshalAs(UnmanagedType.LPWStr)] public string TitleW;
    [MarshalAs(UnmanagedType.LPStr)] public string TitleA;
    public int Width;
    public int Height;
    public int X;
    public int Y;
}