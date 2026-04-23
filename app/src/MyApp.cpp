
#include "../include/MyApp.hpp"

#include "../../engine/test/app/include/app/GUIController.hpp"
#include "../include/GUIController.hpp"
#include <GUIController.hpp>
#include <MainController.hpp>
#include <spdlog/spdlog.h>

namespace app {
void MyApp::app_setup() {
    spdlog::info("app setup complited!");
    auto main_controller = register_controller<app::MainController>();
    auto gui_controller = register_controller<app::GUIController>();
    main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    gui_controller->after(main_controller);
}

}// app