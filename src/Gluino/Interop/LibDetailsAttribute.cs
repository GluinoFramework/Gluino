namespace Gluino.Interop;

[AttributeUsage(AttributeTargets.Class)]
internal class LibDetailsAttribute(string libName) : Attribute
{
    public string LibName = libName;
    public Type ManagedType;
}