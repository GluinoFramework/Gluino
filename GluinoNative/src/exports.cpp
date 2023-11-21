#include "window.h"
#include "window_options.h"

#ifdef _WIN32
#include "app_win32.h"
#include "window_win32.h"
#endif

using namespace Gluino;

extern "C" {
#ifdef _WIN32
	EXPORT App* Gluino_App_Create(const autostr name) { return new AppWin32(name); }
	EXPORT void Gluino_App_Register(AppWin32* app, const HINSTANCE hInstance) { app->Register(hInstance); }



	EXPORT Window* Gluino_Window_Create(WindowOptions* options) { return new WindowWin32(options); }
#endif

	EXPORT void Gluino_App_Destroy(const App* app) { delete app; }
	EXPORT void Gluino_App_Run(App* app) { app->Run(); }
	EXPORT void Gluino_App_Exit(App* app) { app->Exit(); }



	EXPORT void Gluino_Window_Destroy(const Window* window) { delete window; }


	EXPORT void Gluino_Window_Show(Window* window) { window->Show(); }
	EXPORT void Gluino_Window_Hide(Window* window) { window->Hide(); }
	EXPORT void Gluino_Window_Close(Window* window) { window->Close(); }


	EXPORT void Gluino_Window_SetTitle(Window* window, const autostr title) { window->SetTitle(title); }
}