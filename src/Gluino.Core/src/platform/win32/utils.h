#pragma once

#ifndef GLUINO_UTILS_H
#define GLUINO_UTILS_H

#include <Windows.h>

namespace Gluino {

enum class WindowsOSVersion {
    Unknown,
    Win7,
    Win8,
    Win10
};

enum class PreferredAppMode {
    Default = 0,
    AllowDark = 1,
    ForceDark = 2,
    ForceLight = 3,
    Max = 4,
};

enum IMMERSIVEHCCACHEMODE {
    IHCM_USE_CACHED_VALUE = 0,
    IHCM_REFRESH = 1,
};

enum WINDOWCOMPOSITIONATTRIB {
    WCA_UNDEFINED = 0,
    WCA_NCRENDERING_ENABLED = 1,
    WCA_NCRENDERING_POLICY = 2,
    WCA_TRANSITIONS_FORCEDISABLED = 3,
    WCA_ALLOW_NCPAINT = 4,
    WCA_CAPTION_BUTTON_BOUNDS = 5,
    WCA_NONCLIENT_RTL_LAYOUT = 6,
    WCA_FORCE_ICONIC_REPRESENTATION = 7,
    WCA_EXTENDED_FRAME_BOUNDS = 8,
    WCA_HAS_ICONIC_BITMAP = 9,
    WCA_THEME_ATTRIBUTES = 10,
    WCA_NCRENDERING_EXILED = 11,
    WCA_NCADORNMENTINFO = 12,
    WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
    WCA_VIDEO_OVERLAY_ACTIVE = 14,
    WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
    WCA_DISALLOW_PEEK = 16,
    WCA_CLOAK = 17,
    WCA_CLOAKED = 18,
    WCA_ACCENT_POLICY = 19,
    WCA_FREEZE_REPRESENTATION = 20,
    WCA_EVER_UNCLOAKED = 21,
    WCA_VISUAL_OWNER = 22,
    WCA_HOLOGRAPHIC = 23,
    WCA_EXCLUDED_FROM_DDA = 24,
    WCA_PASSIVEUPDATEMODE = 25,
    WCA_USEDARKMODECOLORS = 26,
    WCA_LAST = 27,
};

struct WINDOWCOMPOSITIONATTRIBDATA {
    WINDOWCOMPOSITIONATTRIB attrib;
    PVOID pvData;
    SIZE_T cbData;
};

WindowsOSVersion GetWindowsOSVersion() noexcept;
void InitDarkModeSupport() noexcept;
bool IsDarkModeEnabled() noexcept;
void EnableDarkMode(HWND hWnd, bool enable) noexcept;
void RefreshNonClientArea(HWND hWnd) noexcept;
bool IsColorSchemeChange(LPARAM lParam) noexcept;
bool IsCompositionEnabled() noexcept;
void AdjustMaximizedClientRect(HWND hWnd, RECT& rect) noexcept;
void ApplyBorderlessStyle(HWND hWnd, bool borderless) noexcept;
void ApplyWindowStyle(HWND hWnd, bool darkMode) noexcept;

inline wchar_t* CStrWiden(const char* str) {
    if (str == nullptr) return nullptr;

    const auto len = (int)strlen(str);
    if (len == 0) return nullptr;

    const int wlen = MultiByteToWideChar(CP_UTF8, 0, str, len, nullptr, 0);
    if (wlen == 0) return nullptr;

    const auto wstr = new wchar_t[wlen + 1];
    if (const auto result = MultiByteToWideChar(CP_UTF8, 0, str, len, wstr, wlen);
        result == 0) {
        delete[] wstr;
        return nullptr;
    }

    wstr[wlen] = L'\0';
    return wstr;
}

inline char* CStrNarrow(const wchar_t* wstr) {
    if (wstr == nullptr) return nullptr;

    const auto wlen = (int)wcslen(wstr);
    if (wlen == 0) return nullptr;

    const int len = WideCharToMultiByte(CP_UTF8, 0, wstr, wlen, nullptr, 0, nullptr, nullptr);
    if (len == 0) return nullptr;

    const auto str = new char[len + 1];
    if (const auto result = WideCharToMultiByte(CP_UTF8, 0, wstr, wlen, str, len, nullptr, nullptr);
        result == 0) {
        delete[] str;
        return nullptr;
    }

    str[len] = '\0';
    return str;
}

}

#endif // !GLUINO_UTILS_H
