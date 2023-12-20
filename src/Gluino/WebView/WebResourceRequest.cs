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
    public string Url => _native.Url;

    /// <summary>
    /// Gets the HTTP method of the request.
    /// </summary>
    public string Method => _native.Method;
}