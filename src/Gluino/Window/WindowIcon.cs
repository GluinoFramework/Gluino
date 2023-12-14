using System.Reflection;
using System.Runtime.InteropServices;

namespace Gluino;

/// <summary>
/// Represents an icon for a <see cref="Window"/>.
/// </summary>
public class WindowIcon
{
    private WindowIcon(byte[] data) => Data = data;

    /// <summary>
    /// The icon data as a byte array.
    /// </summary>
    public readonly byte[] Data;

    /// <summary>
    /// Gets the icon data as a stream.
    /// </summary>
    /// <returns>The icon data as a stream.</returns>
    public Stream GetStream() => new MemoryStream(Data);

    /// <summary>
    /// Creates a new <see cref="WindowIcon"/> from the specified byte array.
    /// </summary>
    /// <param name="data">The icon data as a byte array.</param>
    /// <returns>A new <see cref="WindowIcon"/> instance.</returns>
    public static WindowIcon FromBytes(byte[] data) => new(data);

    /// <summary>
    /// Creates a new <see cref="WindowIcon"/> from the specified file path.
    /// </summary>
    /// <param name="path">The path to the icon file.</param>
    /// <returns>A new <see cref="WindowIcon"/> instance.</returns>
    public static WindowIcon FromFile(string path)
    {
        var data = File.ReadAllBytes(path);
        return new WindowIcon(data);
    }

    /// <summary>
    /// Creates a new <see cref="WindowIcon"/> from the specified resource.
    /// </summary>
    /// <param name="name">The name of the resource. This is case-sensitive.</param>
    /// <returns>A new <see cref="WindowIcon"/> instance.</returns>
    /// <exception cref="FileNotFoundException">The resource could not be found.</exception>
    public static WindowIcon FromResource(string name)
    {
        var assembly = Assembly.GetCallingAssembly();
        var resource =
            assembly.GetManifestResourceStream(name) ??
            throw new FileNotFoundException($"Resource '{name}' not found in assembly '{assembly.FullName}'.");
        return FromStream(resource);
    }

    /// <summary>
    /// Creates a new <see cref="WindowIcon"/> from the specified stream.
    /// </summary>
    /// <param name="stream">The stream containing the icon data. The stream will be disposed after reading.</param>
    /// <returns>A new <see cref="WindowIcon"/> instance.</returns>
    public static WindowIcon FromStream(Stream stream)
    {
        using var reader = new BinaryReader(stream);
        var data = reader.ReadBytes((int)stream.Length);
        return new WindowIcon(data);
    }

    /// <summary>
    /// Implicitly converts a byte array to a <see cref="WindowIcon"/>.
    /// </summary>
    /// <param name="data">The icon data as a byte array.</param>
    public static implicit operator WindowIcon(byte[] data) => FromBytes(data);

    /// <summary>
    /// Implicitly converts a <see cref="WindowIcon"/> to a byte array.
    /// </summary>
    /// <param name="icon">The <see cref="WindowIcon"/> instance to convert.</param>
    public static implicit operator byte[](WindowIcon icon) => icon.Data;

    internal nint ToNative(out int size)
    {
        GCHandle handle = default;
        nint ptr;
        try {
            handle = GCHandle.Alloc(Data, GCHandleType.Pinned);
            ptr = handle.AddrOfPinnedObject();
        }
        finally {
            if (handle != default)
                handle.Free();
        }

        size = Data.Length;
        return ptr;
    }

    internal static WindowIcon FromNative(nint ptr, int size)
    {
        var data = new byte[size];
        try {
            Marshal.Copy(ptr, data, 0, size);
        }
        finally {
            Marshal.FreeHGlobal(ptr);
        }

        return new WindowIcon(data);
    }
}