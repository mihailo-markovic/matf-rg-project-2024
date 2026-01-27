//
// Created by grafika on 1/21/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include <engine/core/App.hpp>
#include <engine/core/Controller.hpp>
#include <engine/platform/PlatformEventObserver.hpp>

namespace app {
class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

class MainController : public engine::core::Controller {
    void initialize() override;

    bool loop() override;

    void update() override;

    void begin_draw() override;

    void draw() override;

    void end_draw() override;

    void draw_Ferdinand();

    void update_camera();

public:
    std::string_view name() const override { return "app::MainController"; }

};
}// MainController

#endif //MATF_RG_PROJECT_MAINCONTROLLER_HPP