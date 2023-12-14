namespace Gluino;

/// <summary>
/// Represents the event data for the <see cref="Window.WindowStateChanged"/> event.
/// </summary>
/// <param name="windowState"></param>
public class WindowStateChangedEventArgs(WindowState windowState) : EventArgs
{
    /// <summary>
    /// Gets the new state of the window.
    /// </summary>
    public WindowState WindowState { get; } = windowState;
}