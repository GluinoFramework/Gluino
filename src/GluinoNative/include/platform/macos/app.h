#pragma once

#ifndef GLUINO_APP_H
#define GLUINO_APP_H

#include "app_base.h"
#include "window_base.h"
#include "window_options.h"

namespace Gluino {

class App final : public AppBase {
public:
    ~App() override = default;

    WindowBase* SpawnWindow(WindowOptions* options, WindowEvents* events) override;
    void DespawnWindow(WindowBase* window) override;
    void Run() override;
    void Exit() override;
};

}

#endif // !GLUINO_APP_H
