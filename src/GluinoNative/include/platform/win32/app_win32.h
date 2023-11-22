#pragma once

#ifndef GLUINO_APP_WIN32_H
#define GLUINO_APP_WIN32_H

#include "app.h"

#include <Windows.h>

namespace Gluino {

class AppWin32 final : public App {
public:
	~AppWin32() override = default;

	void Register(HINSTANCE hInstance, autostr className);

	void Run() override;
	void Exit() override;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE _hInstance{};
	wchar_t* _className{};
};

}

#endif // !GLUINO_APP_H