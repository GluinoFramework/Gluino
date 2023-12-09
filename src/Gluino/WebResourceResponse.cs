using System.Runtime.InteropServices;
using Gluino.Native;

namespace Gluino;

public class WebResourceResponse
{
    private static readonly Dictionary<int, string> StatusCodes = new() {
        {100, "Continue"},
        {101, "Switching Protocols"},
        {102, "Processing"},
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {307, "Temporary Redirect"},
        {308, "Permanent Redirect"},
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Timeout"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
        {412, "Precondition Failed"},
        {413, "Payload Too Large"},
        {414, "URI Too Long"},
        {415, "Unsupported Media Type"},
        {416, "Range Not Satisfiable"},
        {417, "Expectation Failed"},
        {418, "I'm a teapot"},
        {421, "Misdirected Request"},
        {422, "Unprocessable Entity"},
        {423, "Locked"},
        {424, "Failed Dependency"},
        {425, "Too Early"},
        {426, "Upgrade Required"},
        {428, "Precondition Required"},
        {429, "Too Many Requests"},
        {431, "Request Header Fields Too Large"},
        {451, "Unavailable For Legal Reasons"},
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Timeout"},
        {505, "HTTP Version Not Supported"},
        {506, "Variant Also Negotiates"},
        {507, "Insufficient Storage"},
        {508, "Loop Detected"},
        {510, "Not Extended"},
        {511, "Network Authentication Required"},
    };

    private NativeWebResourceResponse _native;

    public string ContentType {
        set {
            if (App.Platform.IsWindows) _native.ContentTypeW = value;
            else _native.ContentTypeA = value;
        }
    }

    public Stream Content {
        set {
            if (value == null) return;

            using var ms = new MemoryStream();
            value.CopyTo(ms);
            _native.ContentLength = (int)ms.Position;

            var buffer = Marshal.AllocHGlobal(_native.ContentLength);
            Marshal.Copy(ms.GetBuffer(), 0, buffer, _native.ContentLength);
            _native.Content = buffer;
        }
    }

    public int StatusCode {
        set {
            _native.StatusCode = value;

            var reasonPhrase = StatusCodes.GetValueOrDefault(value, "Unknown");
            if (App.Platform.IsWindows) _native.ReasonPhraseW = reasonPhrase;
            else _native.ReasonPhraseA = reasonPhrase;
        }
    }

    internal NativeWebResourceResponse Native => _native;
}