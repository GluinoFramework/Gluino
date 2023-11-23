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

    public bool IsWindows { get; } = RuntimeInformation.IsOSPlatform(OSPlatform.Windows);
    public bool IsLinux { get; } = RuntimeInformation.IsOSPlatform(OSPlatform.Linux);
    public bool IsOSX { get; } = RuntimeInformation.IsOSPlatform(OSPlatform.OSX);
}