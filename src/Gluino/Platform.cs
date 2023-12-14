using System.Runtime.InteropServices;

namespace Gluino;

/// <summary>
/// Represents the operating system platform.
/// </summary>
public class Platform
{
    internal Platform() { }

    /// <summary>
    /// Gets the current operating system.
    /// </summary>
    public OSPlatform OS { get; } =
        RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? OSPlatform.Windows :
        RuntimeInformation.IsOSPlatform(OSPlatform.OSX) ? OSPlatform.OSX :
        RuntimeInformation.IsOSPlatform(OSPlatform.Linux) ? OSPlatform.Linux :
        throw new PlatformNotSupportedException();

    /// <summary>
    /// Gets the current architecture.
    /// </summary>
    public Architecture Arch { get; } = RuntimeInformation.ProcessArchitecture;

    /// <summary>
    /// Gets a value indicating whether the current operating system is Windows.
    /// </summary>
    public bool IsWindows { get; } = RuntimeInformation.IsOSPlatform(OSPlatform.Windows);

    /// <summary>
    /// Gets a value indicating whether the current operating system is macOS.
    /// </summary>
    public bool IsOSX { get; } = RuntimeInformation.IsOSPlatform(OSPlatform.OSX);

    /// <summary>
    /// Gets a value indicating whether the current operating system is Linux.
    /// </summary>
    public bool IsLinux { get; } = RuntimeInformation.IsOSPlatform(OSPlatform.Linux);
}