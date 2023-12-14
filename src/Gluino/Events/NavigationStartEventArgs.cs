namespace Gluino;

/// <summary>
/// Represents the event data for the <see cref="WebView.NavigationStart"/> event.
/// </summary>
/// <param name="url">The URL of the navigation.</param>
public class NavigationStartEventArgs(string url) : EventArgs
{
    /// <summary>
    /// Gets the URL of the navigation.
    /// </summary>
    public string Url { get; } = url;
}