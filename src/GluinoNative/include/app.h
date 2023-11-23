#pragma once

#ifndef GLUINO_APP_H
#define GLUINO_APP_H

#include "window.h"

namespace Gluino {

class App {
public:
	virtual ~App() = default;

	virtual Window* SpawnWindow(WindowOptions* options) = 0;
	virtual void DespawnWindow(Window* window) = 0;

	virtual void Run() = 0;
	virtual void Exit() = 0;
};

}

#endif // !GLUINO_APP_H
