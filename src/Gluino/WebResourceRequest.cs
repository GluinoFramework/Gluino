using Gluino.Native;

namespace Gluino;

public class WebResourceRequest
{
    private readonly NativeWebResourceRequest _native;

    internal WebResourceRequest(NativeWebResourceRequest native) => _native = native;

    public string Url => App.Platform.IsWindows ? _native.UrlW : _native.UrlA;

    public string Method => App.Platform.IsWindows ? _native.MethodW : _native.MethodA;
}