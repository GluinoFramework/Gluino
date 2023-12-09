using System.Runtime.InteropServices;

namespace Gluino.Native;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeWebResourceResponse
{
    [MarshalAs(UnmanagedType.LPWStr)] public string ContentTypeW;
    [MarshalAs(UnmanagedType.LPStr)] public string ContentTypeA;
    public nint Content;
    [MarshalAs(UnmanagedType.I4)] public int ContentLength;
    [MarshalAs(UnmanagedType.I4)] public int StatusCode;
    [MarshalAs(UnmanagedType.LPWStr)] public string ReasonPhraseW;
    [MarshalAs(UnmanagedType.LPStr)] public string ReasonPhraseA;
}
