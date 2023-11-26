using System.Drawing;

namespace Gluino.Native;

internal static class NativeExtensions
{
    public static NativePoint ToNative(this Point point) => new() {
        X = point.X,
        Y = point.Y
    };

    public static NativeSize ToNative(this Size size) => new() {
        Width = size.Width,
        Height = size.Height
    };

    public static Point ToManaged(this NativePoint point) => new(point.X, point.Y);

    public static Size ToManaged(this NativeSize size) => new(size.Width, size.Height);
}