//
// Created by grafika on 1/21/26.
//

#include "../include/MainController.hpp"

#include <engine/core/Controller.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/platform/PlatformEventObserver.hpp>
#include <spdlog/spdlog.h>

namespace app {
void MainController::initialize() {}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_ESCAPE).is_down()) { return false; }
    return true;
}
}// MainController