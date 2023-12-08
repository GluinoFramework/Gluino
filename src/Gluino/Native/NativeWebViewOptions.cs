using System.Runtime.InteropServices;

namespace Gluino.Native;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeWebViewOptions
{
    [MarshalAs(UnmanagedType.LPWStr)] public string StartUrlW;
    [MarshalAs(UnmanagedType.LPStr)] public string StartUrlA;
    [MarshalAs(UnmanagedType.LPWStr)] public string StartContentW;
    [MarshalAs(UnmanagedType.LPStr)] public string StartContentA;
    [MarshalAs(UnmanagedType.I1)] public bool ContextMenuEnabled;
    [MarshalAs(UnmanagedType.I1)] public bool DevToolsEnabled;
    [MarshalAs(UnmanagedType.LPWStr)] public string UserAgentW;
    [MarshalAs(UnmanagedType.LPStr)] public string UserAgentA;
    [MarshalAs(UnmanagedType.I1)] public bool GrantPermissions;
}