#include "app_win32.h"

using namespace Gluino;

void AppWin32::Register(HINSTANCE hInstance) {
	_hInstance = hInstance;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof WNDCLASSEX;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = WS_EX_NOPARENTNOTIFY;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = _name;

	RegisterClassEx(&wcex);

	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}

void AppWin32::Run()
{
}

void AppWin32::Exit()
{
}

LRESULT AppWin32::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_CREATE:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
