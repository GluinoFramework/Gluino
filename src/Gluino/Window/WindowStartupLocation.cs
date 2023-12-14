namespace Gluino;

/// <summary>
/// Represents the startup location of a <see cref="Window"/>.
/// </summary>
public enum WindowStartupLocation
{
    /// <summary>
    /// The window will be positioned at the default location.
    /// </summary>
    Default,
    /// <summary>
    /// The window will be positioned at the center of the screen.
    /// </summary>
    CenterScreen,
    /// <summary>
    /// The window will be positioned at the center of its parent window.
    /// </summary>
    CenterParent,
    /// <summary>
    /// The window will be positioned at the location specified by <see cref="Window.Location"/>.
    /// </summary>
    Manual
}