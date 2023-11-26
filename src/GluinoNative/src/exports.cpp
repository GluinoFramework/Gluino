#include "window.h"
#include "window_options.h"

#ifdef _WIN32
#include "app_win32.h"
#endif

using namespace Gluino;

extern "C" {
#ifdef _WIN32
	EXPORT App* Gluino_App_Create(const HINSTANCE hInstance) { return new AppWin32(hInstance); }

	//EXPORT Window* Gluino_Window_Create(WindowOptions* options) { return new WindowWin32(options); }
#endif

	EXPORT void Gluino_App_Destroy(const App* app) { delete app; }
	EXPORT Window* Gluino_App_SpawnWindow(App* app, WindowOptions* options, WindowEvents* events) { return app->SpawnWindow(options, events); }
	EXPORT void Gluino_App_DespawnWindow(App* app, Window* window) { app->DespawnWindow(window); }
	EXPORT void Gluino_App_Run(App* app) { app->Run(); }
	EXPORT void Gluino_App_Exit(App* app) { app->Exit(); }



	//EXPORT void Gluino_Window_Destroy(const Window* window) { delete window; }


	EXPORT void Gluino_Window_Show(Window* window) { window->Show(); }
	EXPORT void Gluino_Window_Hide(Window* window) { window->Hide(); }
	EXPORT void Gluino_Window_Close(Window* window) { window->Close(); }
	EXPORT void Gluino_Window_Invoke(Window* window, const Delegate action) { window->Invoke(action); }

	EXPORT void Gluino_Window_GetBounds(Window* window, Rect* bounds) { window->GetBounds(bounds); }

	EXPORT void Gluino_Window_SetTitle(Window* window, const autostr title) { window->SetTitle(title); }
}