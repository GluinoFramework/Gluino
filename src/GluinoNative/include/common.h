#pragma once

#ifndef GLUINO_COMMON_H
#define GLUINO_COMMON_H

#ifdef _WIN32
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

typedef wchar_t* autostr;
#else

#define EXPORT
typedef char* autostr;
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

typedef void (*Delegate)();
typedef bool (*Predicate)();
typedef void (*SizeDelegate)(Size);
typedef void (*PointDelegate)(Point);
typedef void (*StringDelegate)(autostr);
typedef void (*IntDelegate)(int);

inline autostr CopyStr(autostr source) {
    autostr result;

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
concept wstr_ptr = std::is_same_v<T, wchar_t*> || std::is_same_v<T, const wchar_t*>;

template<wstr_ptr... Args>
wchar_t* ConcatStr(Args... args) {
    std::wstringstream wss;

    ((wss << args), ...);

    const std::wstring temp = wss.str();
    const size_t concatenatedSize = temp.length() + 1;
    const auto concatenated = new wchar_t[concatenatedSize];
    wcscpy_s(concatenated, concatenatedSize, temp.c_str());

    return concatenated;
}

}

#endif // !GLUINO_COMMON_H
