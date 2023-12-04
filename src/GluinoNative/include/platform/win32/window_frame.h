#pragma once

#ifndef GLUINO_WINDOW_FRAME_H
#define GLUINO_WINDOW_FRAME_H

#include "common.h"
#include "window.h"

#include <vector>

namespace Gluino {

class WindowFrame {
public:
	explicit WindowFrame(HWND hWndWindow);

	void Attach();
	void Detach() const;
	void Update() const;

private:
	HWND _hWndWindow;
	std::vector<HWND> _hWndEdges;

	Rect GetEdgeRect(WindowEdge edge) const;

	LRESULT CALLBACK WndFrameEdgeProc(HWND hWnd, WindowEdge edge, UINT msg, WPARAM wParam, LPARAM lParam) const;
	static LRESULT CALLBACK WndFrameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

}

#endif // !GLUINO_WINDOW_FRAME_H
