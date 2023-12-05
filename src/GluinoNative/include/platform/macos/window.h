#pragma once

#ifndef GLUINO_WINDOW_H
#define GLUINO_WINDOW_H

#include "window_base.h"

namespace Gluino {

class Window final : public WindowBase {
    explicit Window(WindowOptions* options, WindowEvents* events);
    ~Window() override;

    void Show() override;
    void Hide() override;
    void Close() override;
    void Invoke(Delegate action) override;

    void GetBounds(Rect* bounds) override;

    autostr GetTitle() override;
    void SetTitle(autostr title) override;

    WindowBorderStyle GetBorderStyle() override;
    void SetBorderStyle(WindowBorderStyle style) override;

    WindowState GetWindowState() override;
    void SetWindowState(WindowState state) override;

    Size GetMinimumSize() override;
    void SetMinimumSize(Size& size) override;

    Size GetMaximumSize() override;
    void SetMaximumSize(Size& size) override;

    Size GetSize() override;
    void SetSize(Size& size) override;

    Point GetLocation() override;
    void SetLocation(Point& location) override;

    bool GetTopMost() override;
    void SetTopMost(bool topMost) override;

private:
    
};

}

#endif // !GLUINO_WINDOW_H
