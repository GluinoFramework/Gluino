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

    /// <summary>
    /// Whether or not this function is static.
    /// </summary>
    public bool IsStatic = false;

    /// <summary>
    /// Whether or not to generate a managed wrapper for this function.
    /// </summary>
    public bool Managed = false;

    /// <summary>
    /// Whether or not this is a property getter/setter.
    /// </summary>
    public ManagedProperty Property = ManagedProperty.None;

    /// <summary>
    /// Name of the native option if this is a property getter/setter.
    /// </summary>
    public string Option = null;
}