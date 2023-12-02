#pragma once

#ifndef GLUINO_COMMON_H
#define GLUINO_COMMON_H

#ifdef _WIN32
#include <wchar.h>
#endif

namespace Gluino {

#ifdef _WIN32
#define EXPORT __declspec(dllexport)

typedef wchar_t* autostr;
#else
#include <cstring>

#define EXPORT
typedef char* autostr;
#endif

enum class WindowBorderStyle {
    Normal,
    Borderless,
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

struct Size {
    int width;
    int height;
};

struct Point {
    int x;
    int y;
};

struct Rect {
    int width;
    int height;
    int x;
    int y;
    int top;
    int right;
    int bottom;
    int left;
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
    errno_t err = wcscpy_s(result, len, source);
    if (err != 0) {
        delete[] result;
        result = nullptr;
    }
#else
    size_t len = strlen(source) + 1;
    result = new char[len];
    errno_t err = strcpy_s(result, len, source);
    if (err != 0) {
        delete[] result;
        result = nullptr;
    }
#endif

    return result;
}

}

#endif // !GLUINO_COMMON_H
