namespace Gluino;

/// <summary>
/// Represents the event data for the <see cref="Window.Closing"/> event.
/// </summary>
/// <param name="cancel"></param>
public class WindowClosingEventArgs(bool cancel) : EventArgs
{
    /// <summary>
    /// Gets or sets a value indicating whether the window closing should be canceled.
    /// </summary>
    public bool Cancel { get; set; } = cancel;
}