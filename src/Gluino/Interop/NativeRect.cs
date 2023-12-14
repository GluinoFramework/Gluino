using System.Runtime.InteropServices;

namespace Gluino.Interop;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeRect
{
    [MarshalAs(UnmanagedType.I4)] public int X;
    [MarshalAs(UnmanagedType.I4)] public int Y;
    [MarshalAs(UnmanagedType.I4)] public int Width;
    [MarshalAs(UnmanagedType.I4)] public int Height;
    
    public readonly int Left => X;
    public readonly int Top => Y;
    public readonly int Right => X + Width;
    public readonly int Bottom => Y + Height;

    public static readonly NativeRect Empty = new();
}