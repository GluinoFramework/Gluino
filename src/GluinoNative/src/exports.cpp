#include "window_base.h"
#include "window_options.h"

#ifdef _WIN32
#include "app.h"
#endif

using namespace Gluino;

extern "C" {
#ifdef _WIN32
	EXPORT AppBase* Gluino_App_Create(const HINSTANCE hInstance, const autostr appId) { return new App(hInstance, appId); }
#endif

	EXPORT void Gluino_App_Destroy(const AppBase* app) { delete app; }
	EXPORT WindowBase* Gluino_App_SpawnWindow(AppBase* app, WindowOptions* options, WindowEvents* events) { return app->SpawnWindow(options, events); }
	EXPORT void Gluino_App_DespawnWindow(AppBase* app, WindowBase* window) { app->DespawnWindow(window); }
	EXPORT void Gluino_App_Run(AppBase* app) { app->Run(); }
	EXPORT void Gluino_App_Exit(AppBase* app) { app->Exit(); }

	EXPORT void Gluino_Window_Show(WindowBase* window) { window->Show(); }
	EXPORT void Gluino_Window_Hide(WindowBase* window) { window->Hide(); }
	EXPORT void Gluino_Window_Close(WindowBase* window) { window->Close(); }
	EXPORT void Gluino_Window_Center(WindowBase* window) { window->Center(); }
	EXPORT void Gluino_Window_DragMove(WindowBase* window) { window->DragMove(); }
	EXPORT void Gluino_Window_Invoke(WindowBase* window, const Delegate action) { window->Invoke(action); }

	EXPORT void Gluino_Window_GetBounds(WindowBase* window, Rect* bounds) { window->GetBounds(bounds); }

	EXPORT autostr Gluino_Window_GetTitle(WindowBase* window) { return window->GetTitle(); }
	EXPORT void Gluino_Window_SetTitle(WindowBase* window, const autostr title) { window->SetTitle(title); }

	EXPORT WindowBorderStyle Gluino_Window_GetBorderStyle(WindowBase* window) { return window->GetBorderStyle(); }
	EXPORT void Gluino_Window_SetBorderStyle(WindowBase* window, const WindowBorderStyle style) { window->SetBorderStyle(style); }

	EXPORT WindowState Gluino_Window_GetWindowState(WindowBase* window) { return window->GetWindowState(); }
	EXPORT void Gluino_Window_SetWindowState(WindowBase* window, const WindowState state) { window->SetWindowState(state); }

	EXPORT Size Gluino_Window_GetMinimumSize(WindowBase* window) { return window->GetMinimumSize(); }
	EXPORT void Gluino_Window_SetMinimumSize(WindowBase* window, Size size) { window->SetMinimumSize(size); }

	EXPORT Size Gluino_Window_GetMaximumSize(WindowBase* window) { return window->GetMaximumSize(); }
	EXPORT void Gluino_Window_SetMaximumSize(WindowBase* window, Size size) { window->SetMaximumSize(size); }

	EXPORT Size Gluino_Window_GetSize(WindowBase* window) { return window->GetSize(); }
	EXPORT void Gluino_Window_SetSize(WindowBase* window, Size size) { window->SetSize(size); }

	EXPORT Point Gluino_Window_GetLocation(WindowBase* window) { return window->GetLocation(); }
	EXPORT void Gluino_Window_SetLocation(WindowBase* window, Point location) { window->SetLocation(location); }

	EXPORT bool Gluino_Window_GetTopMost(WindowBase* window) { return window->GetTopMost(); }
	EXPORT void Gluino_Window_SetTopMost(WindowBase* window, const bool topMost) { window->SetTopMost(topMost); }
}