namespace Gluino;

public class WindowStateChangedEventArgs(WindowState windowState) : EventArgs
{
    public WindowState WindowState { get; } = windowState;
}