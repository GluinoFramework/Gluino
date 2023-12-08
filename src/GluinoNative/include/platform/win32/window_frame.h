#pragma once

#ifndef GLUINO_WINDOW_FRAME_H
#define GLUINO_WINDOW_FRAME_H

#include "common.h"

#include <Windows.h>
#include <vector>

namespace Gluino {

class WindowFrame {
public:
	explicit WindowFrame(HWND hWndWindow);

	void Attach();
	void Detach();
	void Update() const;

private:
	HWND _hWndWindow;
	std::vector<HWND> _hWndEdges;
	bool _isAttached = false;

	Rect GetEdgeRect(WindowEdge edge) const;

	LRESULT CALLBACK WndFrameEdgeProc(HWND hWnd, WindowEdge edge, UINT msg, WPARAM wParam, LPARAM lParam) const;
	static LRESULT CALLBACK WndFrameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

}

#endif // !GLUINO_WINDOW_FRAME_H
