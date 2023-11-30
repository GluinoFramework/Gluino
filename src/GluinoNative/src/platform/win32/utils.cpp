#include "utils.h"

#include <dwmapi.h>
#include <mutex>

using namespace Gluino;

typedef void(WINAPI*FnRtlGetNtVersionNumbers)(LPDWORD, LPDWORD, LPDWORD);
typedef HRESULT(WINAPI*FnSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);
typedef BOOLEAN(WINAPI*FnShouldAppsUseDarkMode)();
typedef BOOLEAN(WINAPI*FnAllowDarkModeForWindow)(HWND, BOOLEAN);
typedef void(WINAPI*FnRefreshImmersiveColorPolicyState)();
typedef BOOLEAN(WINAPI*FnIsDarkModeAllowedForWindow)(HWND);
typedef BOOLEAN(WINAPI*FnGetIsImmersiveColorUsingHighContrast)(IMMERSIVEHCCACHEMODE);
typedef PreferredAppMode(WINAPI*FnSetPreferredAppMode)(PreferredAppMode);

FnSetWindowCompositionAttribute SetWindowCompositionAttribute = nullptr;
FnShouldAppsUseDarkMode ShouldAppsUseDarkMode = nullptr;
FnAllowDarkModeForWindow AllowDarkModeForWindow = nullptr;
FnRefreshImmersiveColorPolicyState RefreshImmersiveColorPolicyState = nullptr;
FnIsDarkModeAllowedForWindow IsDarkModeAllowedForWindow = nullptr;
FnGetIsImmersiveColorUsingHighContrast GetIsImmersiveColorUsingHighContrast = nullptr;
FnSetPreferredAppMode SetPreferredAppMode = nullptr;

static constexpr DWORD win10_minimum_build_dark_mode = 18362;
std::once_flag flag_init_dark_mode_support;

void EnableDarkModeForApp() noexcept {
    if (SetPreferredAppMode) {
        SetPreferredAppMode(AllowDark);
    }
}

DWORD GetBuildNumber() noexcept {
    const auto rtlGetNtVersionNumbers = (FnRtlGetNtVersionNumbers)GetProcAddress(
		GetModuleHandleW(L"ntdll.dll"), "RtlGetNtVersionNumbers");

    if (rtlGetNtVersionNumbers == nullptr) {
        return 0;
    }

    DWORD major, minor, build = 0;
    rtlGetNtVersionNumbers(&major, &minor, &build);
    build &= ~0xF0000000;
    return build;
}

bool IsHighContrast() noexcept {
    HIGHCONTRASTW highContrast;
    highContrast.cbSize = sizeof(highContrast);
    if (SystemParametersInfoW(SPI_GETHIGHCONTRAST,
        sizeof(highContrast),
        &highContrast,
        FALSE) == TRUE) {
        return (highContrast.dwFlags & HCF_HIGHCONTRASTON) > 0;
    }
    return false;
}

void InitDarkModeSupportOnce() noexcept {
    if (const auto buildNumber = GetBuildNumber(); buildNumber < win10_minimum_build_dark_mode) {
        return;
    }

    const HMODULE uxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (uxtheme == nullptr) {
        return;
    }

    RefreshImmersiveColorPolicyState = (FnRefreshImmersiveColorPolicyState)GetProcAddress(uxtheme, MAKEINTRESOURCEA(104));
    GetIsImmersiveColorUsingHighContrast = (FnGetIsImmersiveColorUsingHighContrast)GetProcAddress(uxtheme, MAKEINTRESOURCEA(106));
    ShouldAppsUseDarkMode = (FnShouldAppsUseDarkMode)GetProcAddress(uxtheme, MAKEINTRESOURCEA(132));
    AllowDarkModeForWindow = (FnAllowDarkModeForWindow)GetProcAddress(uxtheme, MAKEINTRESOURCEA(133));
    SetPreferredAppMode = (FnSetPreferredAppMode)GetProcAddress(uxtheme, MAKEINTRESOURCEA(135));
    IsDarkModeAllowedForWindow = (FnIsDarkModeAllowedForWindow)GetProcAddress(uxtheme, MAKEINTRESOURCEA(137));
    SetWindowCompositionAttribute = (FnSetWindowCompositionAttribute)GetProcAddress(GetModuleHandleW(L"user32.dll"), "SetWindowCompositionAttribute");

    if (RefreshImmersiveColorPolicyState &&
        ShouldAppsUseDarkMode &&
        AllowDarkModeForWindow &&
        SetPreferredAppMode &&
        IsDarkModeAllowedForWindow) {
        EnableDarkModeForApp();
        RefreshImmersiveColorPolicyState();
    }
}

void Gluino::InitDarkModeSupport() noexcept {
    std::call_once(flag_init_dark_mode_support, InitDarkModeSupportOnce);
}

bool Gluino::IsDarkModeEnabled() noexcept {
    if (ShouldAppsUseDarkMode == nullptr) {
        return false;
    }
    return (ShouldAppsUseDarkMode() == TRUE) && !IsHighContrast();
}

void Gluino::EnableDarkMode(const HWND hWnd, const bool enable) noexcept {
    if (AllowDarkModeForWindow == nullptr) {
        return;
    }
    AllowDarkModeForWindow(hWnd, enable ? TRUE : FALSE);
}

void Gluino::RefreshNonClientArea(const HWND hWnd) noexcept {
    if (IsDarkModeAllowedForWindow == nullptr ||
        ShouldAppsUseDarkMode == nullptr) {
        return;
    }

    BOOL dark = FALSE;
    if (IsDarkModeAllowedForWindow(hWnd) == TRUE &&
        ShouldAppsUseDarkMode() == TRUE && !IsHighContrast()) {
        dark = TRUE;
    }

    if (SetWindowCompositionAttribute != nullptr) {
        WINDOWCOMPOSITIONATTRIBDATA data = {
            WCA_USEDARKMODECOLORS, &dark, sizeof(dark) };
        SetWindowCompositionAttribute(hWnd, &data);
    }
}

bool Gluino::IsColorSchemeChange(const LPARAM lParam) noexcept {
    bool returnValue = false;
    if (lParam > 0 && CompareStringOrdinal(reinterpret_cast<LPCWCH>(lParam),
        -1,
        L"ImmersiveColorSet",
        -1,
        TRUE) == CSTR_EQUAL) {
        RefreshImmersiveColorPolicyState();
        returnValue = true;
    }

    if (GetIsImmersiveColorUsingHighContrast) {
        GetIsImmersiveColorUsingHighContrast(IHCM_REFRESH);
    }
    return returnValue;
}

bool Gluino::IsCompositionEnabled() noexcept
{
    BOOL compositionEnabled = FALSE;
    const bool success = DwmIsCompositionEnabled(&compositionEnabled) == S_OK;
    return compositionEnabled && success;
}

void Gluino::AdjustMaximizedClientRect(const HWND hWnd, RECT& rect) noexcept {
    WINDOWPLACEMENT wp;
    if (!GetWindowPlacement(hWnd, &wp)) {
    	return;
	}

    if (const auto maximized = wp.showCmd == SW_MAXIMIZE; !maximized) {
        return;
    }

    const auto monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONULL);
    if (!monitor) {
        return;
    }

    MONITORINFO monitorInfo{};
    monitorInfo.cbSize = sizeof(monitorInfo);
    if (!GetMonitorInfoW(monitor, &monitorInfo)) {
        return;
    }

    rect = monitorInfo.rcWork;
}

void Gluino::ApplyBorderlessStyle(const HWND hWnd, const bool borderless) noexcept {
    const auto compositionEnabled = IsCompositionEnabled();

    constexpr DWORD aeroBorderless = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
    constexpr DWORD basicBorderless = WS_POPUP | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
    const DWORD borderlessStyle = compositionEnabled ? aeroBorderless : basicBorderless;

    const DWORD newStyle = borderless ? borderlessStyle : WS_OVERLAPPEDWINDOW;
    const DWORD oldStyle = GetWindowLong(hWnd, GWL_STYLE);

    if (newStyle == oldStyle) {
        return;
    }

    SetWindowLongW(hWnd, GWL_STYLE, static_cast<LONG>(newStyle));

    if (compositionEnabled) {
        constexpr MARGINS shadowState[2] = { {0, 0, 0, 0}, {1, 1, 1, 1} };
        DwmExtendFrameIntoClientArea(hWnd, &shadowState[borderless]);
    }

    SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
    ShowWindow(hWnd, SW_SHOW);
}

void Gluino::ApplyWindowStyle(const HWND hWnd, const bool darkMode) noexcept {
    if (IsCompositionEnabled()) {
        constexpr MARGINS margins = { -1, -1, -1, -1 };
        auto hr = DwmExtendFrameIntoClientArea(hWnd, &margins);
        if (FAILED(hr)) return;

        const BOOL mode = darkMode ? TRUE : FALSE;
        hr = DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &mode, sizeof mode);
        if (FAILED(hr)) return;

        constexpr DWM_SYSTEMBACKDROP_TYPE value = DWMSBT_MAINWINDOW;
        hr = DwmSetWindowAttribute(hWnd, DWMWA_SYSTEMBACKDROP_TYPE, &value, sizeof value);
        if (FAILED(hr)) return;

        constexpr BOOL bkdbrush = TRUE;
        auto _ = DwmSetWindowAttribute(hWnd, DWMWA_USE_HOSTBACKDROPBRUSH, &bkdbrush, sizeof bkdbrush);
    }
    else {
        EnableDarkMode(hWnd, darkMode);
        if (IsDarkModeEnabled()) {
            RefreshNonClientArea(hWnd);
        }
    }
}
