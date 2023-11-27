using System.Drawing;
using Gluino.Events;
using Gluino.Native;

namespace Gluino;

public partial class Window
{
    private const int CW_USEDEFAULT = unchecked((int)0x80000000);

    private readonly int _managedWindowThreadId;

    private nint _nativeInstance;
    private NativeWindowOptions _nativeOptions;
    private NativeWindowEvents _nativeEvents;

    public event EventHandler Creating;
    public event EventHandler Created;
    public event EventHandler Shown;
    public event EventHandler Hidden;
    public event EventHandler<Size> SizeChanged;
    public event EventHandler<Point> LocationChanged;
    public event EventHandler FocusIn;
    public event EventHandler FocusOut;
    public event EventHandler<WindowClosingEventArgs> Closing;
    public event EventHandler Closed;

    public Window()
    {
        _managedWindowThreadId = Environment.CurrentManagedThreadId;
        _nativeOptions = new() {
            WindowStyle = WindowStyle.Normal,
            WindowState = WindowState.Normal,
            Size = new() {
                Width = 800, 
                Height = 600
            },
            Location = new() {
                X = CW_USEDEFAULT, 
                Y = CW_USEDEFAULT
            }
        };

        Title = "Window";

        _nativeEvents = new() {
            OnShown = InvokeShown,
            OnHidden = InvokeHidden,
            OnSizeChanged = InvokeSizeChanged,
            OnLocationChanged = InvokeLocationChanged,
            OnFocusIn = InvokeFocusIn,
            OnFocusOut = InvokeFocusOut,
            OnClosing = InvokeClosing,
            OnClosed = InvokeClosed
        };
    }

    public string Title {
        get => GetTitle();
        set => SetTitle(value);
    }

    public WindowStyle WindowStyle {
        get => GetWindowStyle();
        set => SetWindowStyle(value);
    }

    public WindowState WindowState {
        get => GetWindowState();
        set => SetWindowState(value);
    }

    internal bool IsMain {
        set {
            if (_nativeInstance == nint.Zero)
                _nativeOptions.IsMain = value;
            else
                throw new InvalidOperationException();
        }
    }

    public Rectangle GetBounds()
    {
        var bounds = NativeRect.Empty;
        SafeInvoke(() => NativeWindow.GetBounds(_nativeInstance, out bounds));
        return new(bounds.X, bounds.Y, bounds.Width, bounds.Height);
    }

    public void Show()
    {
        if (_nativeInstance == nint.Zero) {
            InvokeCreating();

            if (App.Platform.IsWindows) {
                _nativeOptions.ClassName = $"{App.Name}.Window.{App.WindowCount}";
                App.WindowCount++;
            }

            _nativeInstance = NativeApp.SpawnWindow(App.NativeInstance, ref _nativeOptions, ref _nativeEvents);
            App.ActiveWindows.Add(this);

            InvokeCreated();
        }

        Invoke(() => NativeWindow.Show(_nativeInstance));
    }

    public void Hide()
    {
        SafeInvoke(() => NativeWindow.Hide(_nativeInstance));
    }

    public void Close()
    {
        if (_nativeInstance == nint.Zero)
            return;

        Invoke(() => NativeWindow.Close(_nativeInstance));

        if (App.MainWindow == this)
            App.Exit();
    }
    
    internal void Invoke(Action action)
    {
        if (Environment.CurrentManagedThreadId == _managedWindowThreadId)
            action();
        else
            NativeWindow.Invoke(_nativeInstance, action.Invoke);
    }

    internal T Invoke<T>(Func<T> func)
    {
        T result = default;
        Invoke((Action)(() => result = func()));
        return result;
    }

    internal void SafeInvoke(Action action)
    {
        if (_nativeInstance == nint.Zero)
            return;
        Invoke(action);
    }

    internal T SafeInvoke<T>(Func<T> func) => _nativeInstance == nint.Zero ? default : Invoke(func);

    protected virtual void OnCreating(EventArgs e) { }
    protected virtual void OnCreated(EventArgs e) { }
    protected virtual void OnShown(EventArgs e) { }
    protected virtual void OnHidden(EventArgs e) { }
    protected virtual void OnSizeChanged(Size e) { }
    protected virtual void OnLocationChanged(Point e) { }
    protected virtual void OnFocusIn(EventArgs e) { }
    protected virtual void OnFocusOut(EventArgs e) { }
    protected virtual void OnClosing(WindowClosingEventArgs e) { }
    protected virtual void OnClosed(EventArgs e) { }

    private void InvokeCreating()
    {
        OnCreating(EventArgs.Empty);
        Creating?.Invoke(this, EventArgs.Empty);
    }

    private void InvokeCreated()
    {
        OnCreated(EventArgs.Empty);
        Created?.Invoke(this, EventArgs.Empty);
    }

    private void InvokeShown()
    {
        OnShown(EventArgs.Empty);
        Shown?.Invoke(this, EventArgs.Empty);
    }

    private void InvokeHidden()
    {
        OnHidden(EventArgs.Empty);
        Hidden?.Invoke(this, EventArgs.Empty);
    }

    private void InvokeSizeChanged(NativeSize e)
    {
        OnSizeChanged(new(e.Width, e.Height));
        SizeChanged?.Invoke(this, new(e.Width, e.Height));
    }

    private void InvokeLocationChanged(NativePoint e)
    {
        OnLocationChanged(new(e.X, e.Y));
        LocationChanged?.Invoke(this, new(e.X, e.Y));
    }

    private void InvokeFocusIn()
    {
        OnFocusIn(EventArgs.Empty);
        FocusIn?.Invoke(this, EventArgs.Empty);
    }

    private void InvokeFocusOut()
    {
        OnFocusOut(EventArgs.Empty);
        FocusOut?.Invoke(this, EventArgs.Empty);
    }

    private bool InvokeClosing()
    {
        var e = new WindowClosingEventArgs(false);
        OnClosing(e);
        Closing?.Invoke(this, e);
        return e.Cancel;
    }

    private void InvokeClosed()
    {
        OnClosed(EventArgs.Empty);
        Closed?.Invoke(this, EventArgs.Empty);
    }
}