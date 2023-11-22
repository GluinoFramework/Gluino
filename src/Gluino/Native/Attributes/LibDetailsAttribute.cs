namespace Gluino.Native;

[AttributeUsage(AttributeTargets.Class)]
internal class LibDetailsAttribute(string libName) : Attribute
{
    public string LibName { get; set; } = libName;
}