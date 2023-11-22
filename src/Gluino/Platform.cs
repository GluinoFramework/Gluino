using System.Runtime.InteropServices;

namespace Gluino;

public class Platform
{
    internal Platform() { }

    public OSPlatform OS { get; } =
        RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? OSPlatform.Windows :
        RuntimeInformation.IsOSPlatform(OSPlatform.Linux) ? OSPlatform.Linux :
        RuntimeInformation.IsOSPlatform(OSPlatform.OSX) ? OSPlatform.OSX :
        throw new PlatformNotSupportedException();

    public Architecture Arch { get; } = RuntimeInformation.ProcessArchitecture;

    public static bool IsWindows { get; } = RuntimeInformation.IsOSPlatform(OSPlatform.Windows);
    public static bool IsLinux { get; } = RuntimeInformation.IsOSPlatform(OSPlatform.Linux);
    public static bool IsOSX { get; } = RuntimeInformation.IsOSPlatform(OSPlatform.OSX);
}