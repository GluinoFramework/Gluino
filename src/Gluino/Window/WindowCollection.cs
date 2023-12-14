using System.Collections;

namespace Gluino;

/// <summary>
/// Represents a collection of <see cref="Window"/> instances.
/// </summary>
public sealed class WindowCollection : IReadOnlyList<Window>
{
    private readonly List<Window> _windows = [];

    /// <summary>
    /// Gets the number of windows in the collection.
    /// </summary>
    public int Count => _windows.Count;

    /// <summary>
    /// Gets the window at the specified index.
    /// </summary>
    /// <param name="index">The zero-based index of the <see cref="Window"/> to get.</param>
    /// <returns>The <see cref="Window"/> at the specified index.</returns>
    public Window this[int index] => _windows[index];

    /// <summary>
    /// Gets the index of the specified window.
    /// </summary>
    /// <param name="window">The <see cref="Window"/> to get the index of.</param>
    /// <returns>
    /// The zero-based index of the first occurrence of the specified <see cref="Window"/> in the collection if found; otherwise, -1.
    /// </returns>
    public int IndexOf(Window window) => _windows.IndexOf(window);

    /// <summary>
    /// Returns an enumerator that iterates through the collection.
    /// </summary>
    /// <returns>An enumerator that can be used to iterate through the collection.</returns>
    public IEnumerator<Window> GetEnumerator() => _windows.GetEnumerator();

    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

    internal void Add(Window window) => _windows.Add(window);

    internal void Remove(Window window) => _windows.Remove(window);
}