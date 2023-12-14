using Gluino.Interop;

namespace Gluino;

/// <summary>
/// Represents a request for a resource.
/// </summary>
public class WebResourceRequest
{
    private readonly NativeWebResourceRequest _native;

    internal WebResourceRequest(NativeWebResourceRequest native) => _native = native;

    /// <summary>
    /// Gets the URL of the requested resource.
    /// </summary>
    public string Url => App.Platform.IsWindows ? _native.UrlW : _native.UrlA;

    /// <summary>
    /// Gets the HTTP method of the request.
    /// </summary>
    public string Method => App.Platform.IsWindows ? _native.MethodW : _native.MethodA;
}