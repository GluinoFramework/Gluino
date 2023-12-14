using System.Collections;

namespace Gluino;

public sealed class WindowCollection : IReadOnlyList<Window>
{
    private readonly List<Window> _windows = [];

    public int Count => _windows.Count;

    public Window this[int index] => _windows[index];

    public int IndexOf(Window window) => _windows.IndexOf(window);

    public IEnumerator<Window> GetEnumerator() => _windows.GetEnumerator();

    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

    internal void Add(Window window) => _windows.Add(window);

    internal void Remove(Window window) => _windows.Remove(window);
}