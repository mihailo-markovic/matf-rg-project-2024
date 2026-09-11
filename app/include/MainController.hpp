
#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include <engine/core/App.hpp>
#include <engine/core/Controller.hpp>
#include <engine/platform/PlatformEventObserver.hpp>
#include <engine/resources/Mesh.hpp>
#include <engine/graphics/PointShadow.hpp>

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

    void update_events();

    void light_setup();

    void draw_scene_depth(engine::resources::Shader *depth_shader);

    bool m_spotlight_on = true;

    glm::vec3 m_ferdinand_pos = glm::vec3(0.0f, 0.0f, -1.0f);
    float m_ferdinand_yaw = 0.0f;

    enum class EventState {
        IDLE,
        WAITING,
        MOVING,
        WAITING_ROT,
        ROTATING
    };

    EventState m_event_state = EventState::IDLE;
    float m_event_timer = 0.0f;
    float m_event_rot_start = 0.0f;
    bool m_prev_t_down = false;
    bool m_prev_r_down = false;

    engine::graphics::PointShadow m_point_shadow;
};
}// MainController

#endif //MATF_RG_PROJECT_MAINCONTROLLER_HPP