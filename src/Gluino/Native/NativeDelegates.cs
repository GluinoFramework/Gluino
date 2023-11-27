﻿using System.Runtime.InteropServices;

namespace Gluino.Native;

[UnmanagedFunctionPointer(CallingConvention.Cdecl)] internal delegate void NativeDelegate();
[UnmanagedFunctionPointer(CallingConvention.Cdecl)] internal delegate bool NativePredicate();
[UnmanagedFunctionPointer(CallingConvention.Cdecl)] internal delegate void NativeSizeDelegate(NativeSize size);
[UnmanagedFunctionPointer(CallingConvention.Cdecl)] internal delegate void NativePointDelegate(NativePoint point);
[UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Auto)] internal delegate void StringDelegate(string str);