#pragma once

#ifndef GLUINO_APP_H
#define GLUINO_APP_H

#include "common.h"

namespace Gluino {

class App {
public:
	virtual ~App() = default;

	virtual void Run() = 0;
	virtual void Exit() = 0;
};

}

#endif // !GLUINO_APP_H
