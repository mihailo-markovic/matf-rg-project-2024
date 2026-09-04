
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

    void draw_lamp1();

    void update_camera();

    void update_light();

    void update_ferdinand();

    void light_setup();

    bool m_spotlight_on = true;

    glm::vec3 m_ferdinand_pos = glm::vec3(0.0f, 0.0f, -1.0f);
    float m_ferdinand_yaw = 0.0f;

};
}// MainController

#endif //MATF_RG_PROJECT_MAINCONTROLLER_HPP