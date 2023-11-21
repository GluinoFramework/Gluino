#pragma once

#ifndef GLUINO_APP_H
#define GLUINO_APP_H

#include "common.h"

namespace Gluino {

class App {
public:
	explicit App(const autostr name) {
		_name = CopyStr(name);
	}

	virtual ~App() {
		delete[] _name;
	}

	virtual void Run() = 0;
	virtual void Exit() = 0;

protected:
	autostr _name;
};

}

#endif // !GLUINO_APP_H
