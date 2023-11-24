using System.Runtime.InteropServices;
using System.Xml.Linq;
using Gluino.Native;

namespace Gluino;

public class Window
{
    private readonly int _managedWindowThreadId;

    private nint _nativeInstance;
    private NativeWindowOptions _nativeOptions;


    public Window()
    {
        _managedWindowThreadId = Environment.CurrentManagedThreadId;
        _nativeOptions = new() {
            Width = -1,
            Height = -1,
            X = -1,
            Y = -1,
        };

        Title = "Window";
    }

    public string Title {
        set {
            if (_nativeInstance == nint.Zero) {
                if (App.Platform.IsWindows)
                    _nativeOptions.TitleW = value;
                else
                    _nativeOptions.TitleA = value;
            }
            else
                Invoke(() => NativeWindow.SetTitle(_nativeInstance, value));
        }
    }

    public void Show()
    {
        if (_nativeInstance == nint.Zero) {
            if (App.Platform.IsWindows) {
                _nativeOptions.ClassName = $"{App.Name}.Window.{App.WindowCount}";
                App.WindowCount++;
            }

            _nativeInstance = NativeApp.SpawnWindow(App.NativeInstance, ref _nativeOptions);
            App.ActiveWindows.Add(this);
        }

        Invoke(() => NativeWindow.Show(_nativeInstance));
    }

    internal T Invoke<T>(Func<T> func)
    {
        T result = default;
        Invoke((Action)(() => result = func()));
        return result;
    }

    internal void Invoke(Action action)
    {
        if (Environment.CurrentManagedThreadId == _managedWindowThreadId)
            action();
        else
            NativeWindow.Invoke(_nativeInstance, action.Invoke);
    }
}