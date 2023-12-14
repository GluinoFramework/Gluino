using System.Drawing;
using System.Runtime.InteropServices;

namespace Gluino.Interop;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeSize
{
    [MarshalAs(UnmanagedType.I4)] public int Width;
    [MarshalAs(UnmanagedType.I4)] public int Height;

    public static implicit operator Size(NativeSize size) => new(size.Width, size.Height);
    public static implicit operator NativeSize(Size size) => new() { Width = size.Width, Height = size.Height };
}