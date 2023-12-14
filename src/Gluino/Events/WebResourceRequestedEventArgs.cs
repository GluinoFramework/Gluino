namespace Gluino;

/// <summary>
/// Represents the event arguments for the <see cref="WebView.ResourceRequested"/> event.
/// </summary>
/// <param name="request"></param>
/// <param name="response"></param>
public class WebResourceRequestedEventArgs(WebResourceRequest request, WebResourceResponse response) : EventArgs
{
    /// <summary>
    /// Gets the request.
    /// </summary>
    public WebResourceRequest Request { get; } = request;
    
    /// <summary>
    /// Gets the response.
    /// </summary>
    public WebResourceResponse Response { get; } = response;
}