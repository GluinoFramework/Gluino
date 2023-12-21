#pragma once

#ifndef GLUINO_COMMON_H
#define GLUINO_COMMON_H

#ifdef _WIN32
#include "utils.h"
#include <wchar.h>
#else
#include <cstring>
#endif

#include <iosfwd>
#include <sstream>
#include <string>
#include <vector>
#include <concepts>

namespace Gluino {

#ifdef _WIN32
#define EXPORT __declspec(dllexport)

typedef wchar_t* cstr;
typedef std::wstring cppstr;
typedef std::wstringstream cppstrstream;
#else
#define EXPORT

typedef char* cstr;
typedef std::string cppstr;
typedef std::stringstream cppstrstream;
#endif

enum class WindowBorderStyle {
    Sizable,
    SizableNoCaption,
    Fixed,
    FixedNoCaption,
    None
};

enum class WindowState {
    Normal,
    Minimized,
    Maximized
};

enum class WindowStartupLocation {
    Default,
    CenterScreen,
    CenterParent,
    Manual
};

enum class WindowEdge {
    Top,
    Bottom,
    Left,
    Right,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

enum class WindowTheme {
    System,
    Light,
    Dark
};

struct Size {
    int width;
    int height;
};

struct Point {
    int x;
    int y;
};

struct Rect {
    int x;
    int y;
    int width;
    int height;
};

struct WebResourceRequest {
    char* Url;
    char* Method;
};

struct WebResourceResponse {
    char* ContentType;
    void* Content;
    int ContentLength;
    int StatusCode;
    char* ReasonPhrase;
};

typedef void (*Delegate)();
typedef bool (*Predicate)();
typedef void (*SizeDelegate)(Size);
typedef void (*PointDelegate)(Point);
typedef void (*StringDelegate)(cstr);
typedef void (*IntDelegate)(int);
typedef void (*WebResourceDelegate)(WebResourceRequest, WebResourceResponse*);
typedef void (__stdcall *ExecuteScriptCallback)(bool success, cstr result);

inline cstr CStrCopy(cstr source) {
    cstr result;
#ifdef _WIN32
    size_t len = wcslen(source) + 1;
    result = new wchar_t[len];
    if (const errno_t err = wcscpy_s(result, len, source); err != 0) {
        delete[] result;
        result = nullptr;
    }
#else
    size_t len = strlen(source) + 1;
    result = new char[len];
    strcpy(result, source);
#endif

    return result;
}

template<typename T>
#ifdef _WIN32
concept cstr_ptr = std::is_same_v<T, wchar_t*> || std::is_same_v<T, const wchar_t*>;
#else
concept cstr_ptr = std::is_same_v<T, char*> || std::is_same_v<T, const char*>;
#endif

template<cstr_ptr... Args>
cstr CStrConcat(Args... args) {
    cppstrstream css;

    ((css << args), ...);

    const cppstr temp = css.str();
    const size_t concatenatedSize = temp.length() + 1;
#ifdef _WIN32
    auto concatenated = new wchar_t[concatenatedSize];
    wcscpy_s(concatenated, concatenatedSize, temp.c_str());
#else
    auto concatenated = new char[concatenatedSize];
    strncpy(concatenated, temp.c_str(), concatenatedSize);
#endif

    return concatenated;
}

}

#endif // !GLUINO_COMMON_H
