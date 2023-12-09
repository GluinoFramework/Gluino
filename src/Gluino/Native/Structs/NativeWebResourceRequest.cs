using System.Runtime.InteropServices;

namespace Gluino.Native;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeWebResourceRequest
{
    [MarshalAs(UnmanagedType.LPWStr)] public string UrlW;
    [MarshalAs(UnmanagedType.LPStr)] public string UrlA;
    [MarshalAs(UnmanagedType.LPWStr)] public string MethodW;
    [MarshalAs(UnmanagedType.LPStr)] public string MethodA;
}
