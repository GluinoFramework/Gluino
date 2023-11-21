#pragma once

#ifndef GLUINO_WINDOW_WIN32_H
#define GLUINO_WINDOW_WIN32_H

#include "window.h"

namespace Gluino {

class WindowWin32 final : public Window {
public:
	explicit WindowWin32(WindowOptions* options);
	~WindowWin32() override;

	

	void Show() override;
	void Hide() override;
	void Close() override;

	void SetTitle(autostr title) override;

private:
};

}

#endif // !GLUINO_WINDOW_WIN32_H

