namespace Gluino;

public class WebResourceRequestedEventArgs(WebResourceRequest request, WebResourceResponse response) : EventArgs
{
    public WebResourceRequest Request { get; } = request;
    public WebResourceResponse Response { get; } = response;
}