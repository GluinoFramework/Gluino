#pragma once

#ifndef GLUINO_APP_H
#define GLUINO_APP_H

#include "window_base.h"

namespace Gluino {

class AppBase {
public:
	virtual ~AppBase() = default;

	virtual WindowBase* SpawnWindow(WindowOptions* options, WindowEvents* events) = 0;
	virtual void DespawnWindow(WindowBase* window) = 0;

	virtual void Run() = 0;
	virtual void Exit() = 0;
};

}

#endif // !GLUINO_APP_H
