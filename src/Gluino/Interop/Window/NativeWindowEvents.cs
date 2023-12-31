﻿using System.Runtime.InteropServices;

namespace Gluino.Interop;

[StructLayout(LayoutKind.Sequential)]
internal struct NativeWindowEvents
{
    
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeDelegate OnShown;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeDelegate OnHidden;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeSizeDelegate OnResize;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeSizeDelegate OnResizeStart;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeSizeDelegate OnResizeEnd;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativePointDelegate OnLocationChanged;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeIntDelegate OnWindowStateChanged;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeDelegate OnFocusIn;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeDelegate OnFocusOut;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativePredicate OnClosing;
    [MarshalAs(UnmanagedType.FunctionPtr)] public NativeDelegate OnClosed;
}