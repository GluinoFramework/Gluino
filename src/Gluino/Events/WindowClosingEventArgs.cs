namespace Gluino.Events;

public class WindowClosingEventArgs(bool cancel) : EventArgs
{
    public bool Cancel { get; set; } = cancel;
}