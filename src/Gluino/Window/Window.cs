using System.Drawing;
using Gluino.Interop;

namespace Gluino;

public partial class Window
{
    private readonly int _managedWindowThreadId;

    internal nint InstancePtr;
    internal NativeWindowOptions NativeOptions;
    internal NativeWindowEvents NativeEvents;
    
    /// <summary>
    /// Occurs when the window is being created.
    /// </summary>
    public event EventHandler Creating;
    /// <summary>
    /// Occurs when the window has been created.
    /// </summary>
    public event EventHandler Created;
    /// <summary>
    /// Occurs when the window is shown.
    /// </summary>
    public event EventHandler Shown;
    /// <summary>
    /// Occurs when the window is hidden.
    /// </summary>
    public event EventHandler Hidden;
    /// <summary>
    /// Occurs when the window is resized.
    /// </summary>
    public event EventHandler<Size> Resize;
    /// <summary>
    /// Occurs when the window resize operation starts.
    /// </summary>
    public event EventHandler<Size> ResizeStart;
    /// <summary>
    /// Occurs when the window resize operation ends.
    /// </summary>
    public event EventHandler<Size> ResizeEnd;
    /// <summary>
    /// Occurs when the window location is changed.
    /// </summary>
    public event EventHandler<Point> LocationChanged;
    /// <summary>
    /// Occurs when the window state is changed.
    /// </summary>
    public event EventHandler<WindowStateChangedEventArgs> WindowStateChanged; 
    /// <summary>
    /// Occurs when the window gains focus.
    /// </summary>
    public event EventHandler FocusIn;
    /// <summary>
    /// Occurs when the window loses focus.
    /// </summary>
    public event EventHandler FocusOut;
    /// <summary>
    /// Occurs when the window is closing.
    /// </summary>
    public event EventHandler<WindowClosingEventArgs> Closing;
    /// <summary>
    /// Occurs when the window is closed.
    /// </summary>
    public event EventHandler Closed;

    public Window()
    {
        _managedWindowThreadId = Environment.CurrentManagedThreadId;
        NativeOptions = new() {
            BorderStyle = WindowBorderStyle.Sizable,
            WindowState = WindowState.Normal,
            MaximumSize = new() {
                Width = int.MaxValue,
                Height = int.MaxValue
            },
            Size = new() {
                Width = 800, 
                Height = 600
            },
            MinimizeEnabled = true,
            MaximizeEnabled = true
        };

        Title = "Window";

        NativeEvents = new() {
            OnShown = InvokeShown,
            OnHidden = InvokeHidden,
            OnResize = InvokeResize,
            OnResizeStart = InvokeResizeStart,
            OnResizeEnd = InvokeResizeEnd,
            OnLocationChanged = InvokeLocationChanged,
            OnWindowStateChanged = InvokeWindowStateChanged,
            OnFocusIn = InvokeFocusIn,
            OnFocusOut = InvokeFocusOut,
            OnClosing = InvokeClosing,
            OnClosed = InvokeClosed
        };

        WebView = new(this);
    }

    /// <summary>
    /// Get the <see cref="WebView"/> associated with this window.
    /// </summary>
    public WebView WebView { get; }

    /// <summary>
    /// Get or set the title of the window.
    /// </summary>
    /// <remarks>
    /// Default: "Window"
    /// </remarks>
    public string Title {
        get => GetTitle();
        set => SetTitle(value);
    }

    /// <summary>
    /// Get or set the border style of the window.
    /// </summary>
    /// <remarks>
    /// Default: <see cref="WindowBorderStyle.Sizable"/>
    /// </remarks>
    public WindowBorderStyle BorderStyle {
        get => GetBorderStyle();
        set => SetBorderStyle(value);
    }

    /// <summary>
    /// Get or set the state of the window.
    /// </summary>
    /// <remarks>
    /// Default: <see cref="WindowState.Normal"/>
    /// </remarks>
    public WindowState WindowState {
        get => GetWindowState();
        set => SetWindowState(value);
    }

    /// <summary>
    /// Get or set the theme of the window.
    /// </summary>
    /// <remarks>
    /// Default: <see cref="WindowTheme.System"/><br />
    /// Only supported on systems that support light and dark themes.
    /// </remarks>
    public WindowTheme Theme {
        get => GetTheme();
        set => SetTheme(value);
    }

    /// <summary>
    /// Get whether the window's theme is in dark mode.
    /// </summary>
    /// <remarks>
    /// Only supported on systems that support light and dark themes.
    /// </remarks>
    public bool IsDarkMode => SafeInvoke(() => NativeWindow.GetIsDarkMode(InstancePtr));

    /// <summary>
    /// Get or set the startup location of the window.
    /// </summary>
    /// <remarks>
    /// Default: <see cref="WindowStartupLocation.Default"/>
    /// </remarks>
    public WindowStartupLocation StartupLocation
    {
        get => NativeOptions.StartupLocation;
        set => NativeOptions.StartupLocation = value;
    }

    /// <summary>
    /// Get or set the minimum size of the window.
    /// </summary>
    public Size MinimumSize {
        get => GetMinimumSize();
        set => SetMinimumSize(value);
    }

    /// <summary>
    /// Get or set the maximum size of the window.
    /// </summary>
    public Size MaximumSize {
        get => GetMaximumSize();
        set => SetMaximumSize(value);
    }

    /// <summary>
    /// Get or set the size of the window.
    /// </summary>
    public Size Size {
        get => GetSize();
        set => SetSize(value);
    }

    /// <summary>
    /// Get or set the width of the window.
    /// </summary>
    public int Width {
        get => GetSize().Width;
        set => SetSize(new() { Width = value, Height = GetSize().Height });
    }

    /// <summary>
    /// Get or set the height of the window.
    /// </summary>
    public int Height {
        get => GetSize().Height;
        set => SetSize(new() { Width = GetSize().Width, Height = value });
    }

    /// <summary>
    /// Get or set the location of the window.
    /// </summary>
    public Point Location {
        get => GetLocation();
        set => SetLocation(value);
    }

    /// <summary>
    /// Get or set the X coordinate of the window.
    /// </summary>
    public int X {
        get => GetLocation().X;
        set => SetLocation(new() { X = value, Y = GetLocation().Y });
    }

    /// <summary>
    /// Get or set the Y coordinate of the window.
    /// </summary>
    public int Y {
        get => GetLocation().Y;
        set => SetLocation(new() { X = GetLocation().X, Y = value });
    }

    /// <summary>
    /// Get or set whether the window can be minimized.
    /// </summary>
    /// <remarks>
    /// Default: true
    /// </remarks>
    public bool MinimizeEnabled {
        get => GetMinimizeEnabled();
        set => SetMinimizeEnabled(value);
    }

    /// <summary>
    /// Get or set whether the window can be maximized.
    /// </summary>
    /// <remarks>
    /// Default: true
    /// </remarks>
    public bool MaximizeEnabled {
        get => GetMaximizeEnabled();
        set => SetMaximizeEnabled(value);
    }

    /// <summary>
    /// Get or set whether the window is always on top of other windows.
    /// </summary>
    /// <remarks>
    /// Default: false
    /// </remarks>
    public bool TopMost {
        get => GetTopMost();
        set => SetTopMost(value);
    }

    /// <summary>
    /// Get the bounding rectangle of the window.
    /// </summary>
    public Rectangle Bounds {
        get {
            var bounds = NativeRect.Empty;
            SafeInvoke(() => NativeWindow.GetBounds(InstancePtr, out bounds));
            return new(bounds.X, bounds.Y, bounds.Width, bounds.Height);
        }
    }

    /// <summary>
    /// Get the native window handle.
    /// </summary>
    /// <remarks>
    /// Windows only.
    /// </remarks>
    public nint Handle {
        get {
            if (!App.Platform.IsWindows)
                throw new PlatformNotSupportedException();
            return SafeInvoke(() => NativeWindow.GetHandle(InstancePtr));
        }
    }

    internal bool IsMain {
        set {
            if (InstancePtr == nint.Zero)
                NativeOptions.IsMain = value;
            else
                throw new InvalidOperationException();
        }
    }

    /// <summary>
    /// Show the window.
    /// </summary>
    public void Show()
    {
        if (InstancePtr == nint.Zero) {
            InvokeCreating();

            NativeApp.SpawnWindow(App.NativeInstance, 
                ref NativeOptions, ref NativeEvents, 
                ref WebView.NativeOptions, ref WebView.NativeEvents, 
                out InstancePtr, out WebView.InstancePtr);
            App.ActiveWindows.Add(this);
            InvokeCreated();
        }

        Invoke(() => NativeWindow.Show(InstancePtr));
    }

    /// <summary>
    /// Hide the window.
    /// </summary>
    public void Hide() => SafeInvoke(() => NativeWindow.Hide(InstancePtr));

    /// <summary>
    /// Close the window.
    /// </summary>
    public void Close()
    {
        if (InstancePtr == nint.Zero)
            return;

        Invoke(() => NativeWindow.Close(InstancePtr));

        if (App.MainWindow == this)
            App.Exit();
    }

    /// <summary>
    /// Center the window on the screen.
    /// </summary>
    public void Center() => SafeInvoke(() => NativeWindow.Center(InstancePtr));

    /// <summary>
    /// Begin a drag move operation.
    /// </summary>
    public void DragMove() => SafeInvoke(() => NativeWindow.DragMove(InstancePtr));

    internal void Invoke(Action action)
    {
        if (Environment.CurrentManagedThreadId == _managedWindowThreadId)
            action();
        else
            NativeWindow.Invoke(InstancePtr, action.Invoke);
    }

    internal T Invoke<T>(Func<T> func)
    {
        T result = default;
        Invoke((Action)(() => result = func()));
        return result;
    }

    internal void SafeInvoke(Action action)
    {
        if (InstancePtr == nint.Zero)
            return;
        Invoke(action);
    }

    internal T SafeInvoke<T>(Func<T> func) => InstancePtr == nint.Zero ? default : Invoke(func);

    protected virtual void OnCreating(EventArgs e) { }
    protected virtual void OnCreated(EventArgs e) { }
    protected virtual void OnShown(EventArgs e) { }
    protected virtual void OnHidden(EventArgs e) { }
    protected virtual void OnResize(Size e) { }
    protected virtual void OnResizeStart(Size e) { }
    protected virtual void OnResizeEnd(Size e) { }
    protected virtual void OnLocationChanged(Point e) { }
    protected virtual void OnWindowStateChanged(WindowStateChangedEventArgs e) { }
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

    private void InvokeResize(NativeSize e)
    {
        OnResize(new(e.Width, e.Height));
        Resize?.Invoke(this, new(e.Width, e.Height));
    }

    private void InvokeResizeStart(NativeSize e)
    {
        OnResizeStart(new(e.Width, e.Height));
        ResizeStart?.Invoke(this, new(e.Width, e.Height));
    }

    private void InvokeResizeEnd(NativeSize e)
    {
        OnResizeEnd(new(e.Width, e.Height));
        ResizeEnd?.Invoke(this, new(e.Width, e.Height));
    }

    private void InvokeLocationChanged(NativePoint e)
    {
        OnLocationChanged(new(e.X, e.Y));
        LocationChanged?.Invoke(this, new(e.X, e.Y));
    }

    private void InvokeWindowStateChanged(int e)
    {
        OnWindowStateChanged(new((WindowState)e));
        WindowStateChanged?.Invoke(this, new((WindowState)e));
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