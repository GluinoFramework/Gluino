using System.Runtime.InteropServices;

namespace Gluino.Native;

[AttributeUsage(AttributeTargets.Method)]
internal class LibImportAttribute(string libName, string entryPoint) : Attribute
{
    public LibImportAttribute(string entryPoint) : this(null, entryPoint) { }

    public string LibName = libName;
    public string EntryPoint = entryPoint;
    public CallingConvention CallingConvention = CallingConvention.Cdecl;
    public CharSet CharSet= CharSet.Auto;
    public bool SetLastError = true;
    public ManagedMember Member = ManagedMember.None;
    public Type[] MethodParamTypes = [];
    public ManagedProperty Property = ManagedProperty.None;
}