
#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include <engine/core/App.hpp>
#include <engine/core/Controller.hpp>
#include <engine/platform/PlatformEventObserver.hpp>
#include <engine/resources/Mesh.hpp>

namespace app {

class MainPlatformEventObserver final : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

class MainController : public engine::core::Controller {

public:
    std::string_view name() const override { return "app::MainController"; }

    void initialize() override;

    bool loop() override;

    void update() override;

    void begin_draw() override;

    void draw() override;

    void end_draw() override;

private:
    void draw_Ferdinand();

    void draw_floor();

    void draw_lamp();

    void update_camera();

    void light_setup();
};
}// MainController

#endif //MATF_RG_PROJECT_MAINCONTROLLER_HPP