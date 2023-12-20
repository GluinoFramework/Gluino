using System.Runtime.InteropServices;

namespace Gluino.Interop;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeWebViewOptions
{
    [MarshalAs(UnmanagedType.LPStr)] public string StartUrl;
    [MarshalAs(UnmanagedType.LPStr)] public string StartContent;
    [MarshalAs(UnmanagedType.I1)] public bool ContextMenuEnabled;
    [MarshalAs(UnmanagedType.I1)] public bool DevToolsEnabled;
    [MarshalAs(UnmanagedType.LPStr)] public string UserAgent;
    [MarshalAs(UnmanagedType.I1)] public bool GrantPermissions;
}