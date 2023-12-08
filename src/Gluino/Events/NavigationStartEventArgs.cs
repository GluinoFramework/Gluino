namespace Gluino;

public class NavigationStartEventArgs(string url) : EventArgs
{
    public string Url { get; } = url;
}