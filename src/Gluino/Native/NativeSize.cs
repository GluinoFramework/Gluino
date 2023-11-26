using System.Runtime.InteropServices;

namespace Gluino.Native;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeSize
{
    [MarshalAs(UnmanagedType.I4)] public int Width;
    [MarshalAs(UnmanagedType.I4)] public int Height;
}