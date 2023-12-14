using System.Collections.Concurrent;
using System.Text.Json;

namespace Gluino;

internal class WebViewBinder
{
    private const string BindPrefix = "bind:";

    private static readonly JsonSerializerOptions JsonOptions = new() {
        PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
        PropertyNameCaseInsensitive = true
    };

    private readonly WebView _webView;
    private readonly ConcurrentDictionary<string, Delegate> _bindings = new();
    private readonly List<string> _initBindings = [];

    public WebViewBinder(WebView webView)
    {
        _webView = webView;
        _webView.MessageReceived += OnWebViewMessageReceived;
        _webView.Created += OnWebViewCreated;
    }

    private void OnWebViewCreated(object sender, EventArgs e)
    {
        _webView.InjectScriptOnDocumentCreated(
            """
            (function () {
              window.gluino.uuid = function () {
                if (crypto.randomUUID) return crypto.randomUUID();
            
                const data = new Uint8Array(16);
                crypto.getRandomValues(data);
                data[6] = (data[6] & 0x0f) | 0x40;
                data[8] = (data[8] & 0x3f) | 0x80;
                const hex = Array.from(data, (byte) => byte.toString(16).padStart(2, '0')).join('');
                return `${hex.substring(0, 8)}-${hex.substring(8, 12)}-4${hex.substring(13, 16)}-${((data[15] & 0x3f) | 0x80)
                  .toString(16)
                  .padStart(2, '0')}${hex.substring(17, 20)}-${hex.substring(20, 32)}`;
              };
            
              const __bindPrefix = 'bind:';
              const __bindCallbacks = {};
            
              window.gluino.addListener(function (e) {
                if (!e.startsWith(__bindPrefix)) return;
                const cbData = JSON.parse(e.slice(__bindPrefix.length));
                if (__bindCallbacks[cbData.id]) {
                  __bindCallbacks[cbData.id](cbData.ret);
                  delete __bindCallbacks[cbData.id];
                }
              });
            
              window.gluino.invoke = function(name, args, cb) {
                const fnData = {
                  id: window.gluino.uuid(),
                  name,
                  args,
                };
                __bindCallbacks[fnData.id] = cb;
                window.gluino.sendMessage(__bindPrefix + JSON.stringify(fnData));
              }
            })();
            """);

        foreach (var jsFunc in _initBindings)
            _webView.InjectScriptOnDocumentCreated(jsFunc);
    }

    public void Bind(string name, Delegate fn)
    {
        var methodInfo = fn.Method;
        var parameters = methodInfo.GetParameters();

        var jsArgs = string.Join(", ", parameters.Select(p => p.Name));
        var jsFunc =
            $$"""
            window.gluino.{{name}} = function({{jsArgs}}) {
              return new Promise((resolve) => {
                window.gluino.invoke('{{name}}', [{{jsArgs}}], resolve);
              });
            }
            """;

        if (!_bindings.ContainsKey(name))
            _bindings.TryAdd(name, fn);
        else
            _bindings[name] = fn;

        if (_webView.InstancePtr == nint.Zero) {
            _initBindings.Add(jsFunc);
            return;
        }

        _webView.InjectScript(jsFunc);
    }

    private void SendData(object data)
    {
        var json = JsonSerializer.Serialize(data, JsonOptions);
        _webView.SendMessage(BindPrefix + json);
    }

    private void OnWebViewMessageReceived(object sender, string e)
    {
        if (!e.StartsWith(BindPrefix)) return;

        var json = e[BindPrefix.Length..];
        var data = JsonSerializer.Deserialize<BindData>(json, JsonOptions);
        
        if (!_bindings.ContainsKey(data.Name)) {
            if (data.Id != null) {
                SendData(new { data.Id });
            }
            return;
        }
        if (!_bindings.TryGetValue(data.Name, out var fn)) return;

        var parameters = fn.Method.GetParameters();
        var args = data.Args.Select((arg, i) => {
            var type = parameters[i].ParameterType;
            return arg.Deserialize(type, JsonOptions);
        }).ToArray();

        var result = fn.DynamicInvoke(args);

        SendData(new {
            data.Id,
            Ret = result
        });
    }
    
    protected class BindData
    {
        public string Id { get; set; }
        public string Name { get; set; }
        public List<JsonElement> Args { get; set; }
    }
}