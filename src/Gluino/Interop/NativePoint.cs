using System.Drawing;
using System.Runtime.InteropServices;

namespace Gluino.Interop;

[StructLayout(LayoutKind.Sequential)]
internal struct NativePoint
{
    [MarshalAs(UnmanagedType.I4)] public int X;
    [MarshalAs(UnmanagedType.I4)] public int Y;

    public static implicit operator Point(NativePoint point) => new(point.X, point.Y);
    public static implicit operator NativePoint(Point point) => new() { X = point.X, Y = point.Y };
}
