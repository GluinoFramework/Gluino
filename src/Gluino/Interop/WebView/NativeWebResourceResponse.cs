using System.Runtime.InteropServices;

namespace Gluino.Interop;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeWebResourceResponse
{
    [MarshalAs(UnmanagedType.LPStr)] public string ContentType;
    public nint Content;
    [MarshalAs(UnmanagedType.I4)] public int ContentLength;
    [MarshalAs(UnmanagedType.I4)] public int StatusCode;
    [MarshalAs(UnmanagedType.LPStr)] public string ReasonPhrase;
}
