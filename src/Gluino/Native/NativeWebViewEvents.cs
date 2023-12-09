using System.Runtime.InteropServices;

namespace Gluino.Native;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeWebViewEvents
{
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeDelegate OnCreated;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeStringDelegate OnNavigationStart;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeDelegate OnNavigationEnd;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeStringDelegate OnMessageReceived;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeWebResourceDelegate OnResourceRequested;
}