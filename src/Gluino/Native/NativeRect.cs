using System.Runtime.InteropServices;

namespace Gluino.Native;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeRect
{
    [MarshalAs(UnmanagedType.I4)] public int Width;
    [MarshalAs(UnmanagedType.I4)] public int Height;
    [MarshalAs(UnmanagedType.I4)] public int X;
    [MarshalAs(UnmanagedType.I4)] public int Y;
    [MarshalAs(UnmanagedType.I4)] public int Top;
    [MarshalAs(UnmanagedType.I4)] public int Right;
    [MarshalAs(UnmanagedType.I4)] public int Bottom;
    [MarshalAs(UnmanagedType.I4)] public int Left;

    public static readonly NativeRect Empty = new();
}