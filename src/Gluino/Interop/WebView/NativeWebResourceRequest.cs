using System.Runtime.InteropServices;

namespace Gluino.Interop;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeWebResourceRequest
{
    [MarshalAs(UnmanagedType.LPStr)] public string Url;
    [MarshalAs(UnmanagedType.LPStr)] public string Method;
}
