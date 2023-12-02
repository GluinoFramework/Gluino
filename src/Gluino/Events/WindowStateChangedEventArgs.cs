namespace Gluino.Events;

public class WindowStateChangedEventArgs(WindowState windowState) : EventArgs
{
    public WindowState WindowState { get; } = windowState;
}