using System.Runtime.InteropServices;

namespace Gluino.Native;

[StructLayout(LayoutKind.Sequential)]
internal struct NativePoint
{
    [MarshalAs(UnmanagedType.I4)] public int X;
    [MarshalAs(UnmanagedType.I4)] public int Y;
}
