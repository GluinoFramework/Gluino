#pragma once

#ifndef GLUINO_COMMON_H
#define GLUINO_COMMON_H

#ifdef _WIN32
#include <wchar.h>

#define EXPORT __declspec(dllexport)
typedef wchar_t* autostr;
#else
#include <cstring>

#define EXPORT
typedef char* autostr;
#endif

inline autostr CopyStr(autostr source) {
    autostr result;

#ifdef _WIN32
    size_t len = wcslen(source) + 1;
    result = new wchar_t[len];
    errno_t err = wcscpy_s(result, len, source);
    if (err != 0) {
        delete[] result;
    }
#else
    size_t len = strlen(source) + 1;
    result = new char[len];
    errno_t err = strcpy_s(result, len, source);
    if (err != 0) {
        delete[] result;
    }
#endif

    return result;
}

#endif // !GLUINO_COMMON_H
