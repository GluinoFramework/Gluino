#include "window_base.h"
#include "window_options.h"

#ifdef _WIN32
#include "app.h"
#endif

using namespace Gluino;

extern "C" {
#ifdef _WIN32
	EXPORT AppBase* Gluino_App_Create(const HINSTANCE hInstance) { return new App(hInstance); }

	//EXPORT WindowBase* Gluino_Window_Create(WindowOptions* options) { return new Window(options); }
#endif

	EXPORT void Gluino_App_Destroy(const AppBase* app) { delete app; }
	EXPORT WindowBase* Gluino_App_SpawnWindow(AppBase* app, WindowOptions* options, WindowEvents* events) { return app->SpawnWindow(options, events); }
	EXPORT void Gluino_App_DespawnWindow(AppBase* app, WindowBase* window) { app->DespawnWindow(window); }
	EXPORT void Gluino_App_Run(AppBase* app) { app->Run(); }
	EXPORT void Gluino_App_Exit(AppBase* app) { app->Exit(); }



	//EXPORT void Gluino_Window_Destroy(const WindowBase* window) { delete window; }


	EXPORT void Gluino_Window_Show(WindowBase* window) { window->Show(); }
	EXPORT void Gluino_Window_Hide(WindowBase* window) { window->Hide(); }
	EXPORT void Gluino_Window_Close(WindowBase* window) { window->Close(); }
	EXPORT void Gluino_Window_Invoke(WindowBase* window, const Delegate action) { window->Invoke(action); }

	EXPORT void Gluino_Window_GetBounds(WindowBase* window, Rect* bounds) { window->GetBounds(bounds); }

	EXPORT autostr Gluino_Window_GetTitle(WindowBase* window) { return window->GetTitle(); }
	EXPORT void Gluino_Window_SetTitle(WindowBase* window, const autostr title) { window->SetTitle(title); }

	EXPORT WindowStyle Gluino_Window_GetWindowStyle(WindowBase* window) { return window->GetWindowStyle(); }
	EXPORT void Gluino_Window_SetWindowStyle(WindowBase* window, const WindowStyle style) { window->SetWindowStyle(style); }

	EXPORT WindowState Gluino_Window_GetWindowState(WindowBase* window) { return window->GetWindowState(); }
	EXPORT void Gluino_Window_SetWindowState(WindowBase* window, const WindowState state) { window->SetWindowState(state); }
}