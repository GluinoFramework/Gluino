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

constexpr MARGINS StyleExtendMargins = { -1, -1, -1, -1 };
static constexpr DWORD Win10MinimumBuildDarkMode = 18362;
std::once_flag flag_init_dark_mode_support;

WindowsOSVersion Gluino::GetWindowsOSVersion() noexcept
{
    const auto rtlGetNtVersionNumbers = (FnRtlGetNtVersionNumbers)GetProcAddress(
        GetModuleHandleW(L"ntdll.dll"), "RtlGetNtVersionNumbers");

    auto result = WindowsOSVersion::Unknown;

    if (rtlGetNtVersionNumbers == nullptr) {
        return result;
    }

    DWORD major, minor, build = 0;
    rtlGetNtVersionNumbers(&major, &minor, &build);
    build &= ~0xF0000000;

    if (major == 10 && minor == 0) result = WindowsOSVersion::Win10;
    if (major == 6 && (minor == 3 || minor == 2))  result = WindowsOSVersion::Win8;
    if (major == 6 && minor == 1)  result = WindowsOSVersion::Win7;

    return result;
}

void EnableDarkModeForApp() noexcept {
    if (SetPreferredAppMode) {
        SetPreferredAppMode(PreferredAppMode::AllowDark);
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
    if (const auto buildNumber = GetBuildNumber(); buildNumber < Win10MinimumBuildDarkMode) {
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
    if (const auto buildNumber = GetBuildNumber(); buildNumber < Win10MinimumBuildDarkMode) {
        return false;
    }

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
        constexpr MARGINS shadowState[2] = { StyleExtendMargins, {1, 1, 1, 1} };
        DwmExtendFrameIntoClientArea(hWnd, &shadowState[borderless]);
    }

    SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
    ShowWindow(hWnd, SW_SHOW);
}

void Gluino::ApplyWindowStyle(const HWND hWnd, const bool darkMode) noexcept {
    if (const auto buildNumber = GetBuildNumber(); buildNumber < Win10MinimumBuildDarkMode) {
        return;
    }

    DwmExtendFrameIntoClientArea(hWnd, &StyleExtendMargins);

    const BOOL mode = darkMode ? TRUE : FALSE;
    DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &mode, sizeof mode);

    constexpr DWM_SYSTEMBACKDROP_TYPE value = DWMSBT_MAINWINDOW;
    DwmSetWindowAttribute(hWnd, DWMWA_SYSTEMBACKDROP_TYPE, &value, sizeof value);
}
