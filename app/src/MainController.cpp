#include <GUIController.hpp>
#include <MainController.hpp>

#include <engine/core/Controller.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (!gui_controller->is_enabled()) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();

        camera->rotate_camera(position.dx, position.dy);
    }
}

void MainController::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    engine::graphics::OpenGL::enable_depth_testing();
    m_point_shadow = graphics->init_point_shadow(1.0f, 25.0f);
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_ESCAPE).is_down()) { return false; }
    return true;
}

void MainController::draw_ferdinand() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *ferdinand = resources->model("ferdinand");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_ferdinand_pos);
    model = glm::rotate(model, glm::radians(m_ferdinand_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f));
    shader->set_mat4("model", model);
    ferdinand->draw(shader);
}

void MainController::draw_floor() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();


    engine::resources::Model *floor = resources->model("floor");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    // glm::mat4 model = glm::mat4(1.0f);
    // model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
    // shader->set_mat4("model", model);
    // floor->draw(shader);


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));

    graphics->draw_parallax_map(
            resources->shader("parallax"),
            resources->model("floor"),
            model,
            0.02f);
}

void MainController::draw_lamp() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *lamp = resources->model("lamp");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    shader->set_mat4("model", model);
    lamp->draw(shader);
}

void MainController::draw_lamp1() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *lamp = resources->model("lamp1");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, 5.0f));
    model = glm::scale(model, glm::vec3(0.07f, 0.1f, 0.07f));
    shader->set_mat4("model", model);
    lamp->draw(shader);
}

void MainController::draw_hangar() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *hangar = resources->model("hangar");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 5.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.002f));

    shader->set_mat4("model", model);
    hangar->draw(shader);
}

void MainController::update_camera() {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (gui_controller->is_enabled()) { return; }


    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    float dt = platform->dt();
    if (platform->key(engine::platform::KEY_W).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt); }
    if (platform->key(engine::platform::KEY_A).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt); }
    if (platform->key(engine::platform::KEY_S).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt); }
    if (platform->key(engine::platform::KEY_D).is_down()) { camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt); }
}

void MainController::update_light() {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (gui_controller->is_enabled()) { return; }

    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_O).state() == engine::platform::Key::State::JustPressed) { m_spotlight_on = !m_spotlight_on; }
}

void MainController::update_ferdinand() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    float dt = platform->dt();
    float back_speed = 0.5f;
    float rot_speed = 30.0f;

    if (platform->key(engine::platform::KEY_I).is_down()) {
        m_ferdinand_pos.x += glm::sin(glm::radians(m_ferdinand_yaw)) * dt;
        m_ferdinand_pos.z += glm::cos(glm::radians(m_ferdinand_yaw)) * dt;
    }
    if (platform->key(engine::platform::KEY_K).is_down()) {
        m_ferdinand_pos.x -= glm::sin(glm::radians(m_ferdinand_yaw)) * back_speed * dt;
        m_ferdinand_pos.z -= glm::cos(glm::radians(m_ferdinand_yaw)) * back_speed * dt;
    }
    if (platform->key(engine::platform::KEY_J).is_down()) { m_ferdinand_yaw += rot_speed * dt; }
    if (platform->key(engine::platform::KEY_L).is_down()) { m_ferdinand_yaw -= rot_speed * dt; }
}

void MainController::update_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    float dt = platform->dt();


    bool t_down = platform->key(engine::platform::KEY_T).is_down();

    if (t_down && !m_prev_t_down && m_event_state == EventState::IDLE) {
        m_event_state = EventState::WAITING;
        m_event_timer = 0.0f;
    }
    m_prev_t_down = t_down;

    const float M_SECONDS = 3.0f;
    const float MOVE_SECONDS = 3.0f;
    const float N_SECONDS = 2.0f;
    const float ROT_SECONDS = 6.0f;

    if (m_event_state == EventState::WAITING && m_event_timer >= M_SECONDS) {
        m_event_state = EventState::MOVING;
        m_event_timer = 0.0f;
    } else if (m_event_state == EventState::MOVING && m_event_timer >= MOVE_SECONDS) {
        m_event_state = EventState::WAITING_ROT;
        m_event_timer = 0.0f;
    } else if (m_event_state == EventState::WAITING_ROT && m_event_timer >= N_SECONDS) {
        m_event_state = EventState::ROTATING;
        m_event_timer = 0.0f;
        m_event_rot_start = m_ferdinand_yaw;
    } else if (m_event_state == EventState::ROTATING && m_event_timer >= ROT_SECONDS) {
        m_ferdinand_yaw = m_event_rot_start + 360.0f;
        m_event_state = EventState::IDLE;
        m_event_timer = 0.0f;
    }
    m_prev_t_down = t_down;

    m_event_timer += dt;


    if (m_event_state == EventState::MOVING) {
        float speed = 1.0f;
        m_ferdinand_pos.x += glm::sin(glm::radians(m_ferdinand_yaw)) * speed * dt;
        m_ferdinand_pos.z += glm::cos(glm::radians(m_ferdinand_yaw)) * speed * dt;
    } else if (m_event_state == EventState::ROTATING) {
        float rot_speed = 360.0f / ROT_SECONDS;
        m_ferdinand_yaw += rot_speed * dt;
    }
}

void MainController::light_setup() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();


    for (auto shader_name: {"basic", "parallax"}) {
        engine::resources::Shader *shader = resources->shader(shader_name);
        shader->use();
        shader->set_vec3("dirLight.direction", glm::vec3(1.0f, -1.0f, 1.0f));
        shader->set_vec3("dirLight.ambient", glm::vec3(0.3f));
        shader->set_vec3("dirLight.diffuse", glm::vec3(0.6f));
        shader->set_vec3("dirLight.specular", glm::vec3(0.7f));
        shader->set_vec3("spotLight.position", glm::vec3(0.7f, 3.0f, -1.0f));
        shader->set_vec3("spotLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
        shader->set_vec3("pointLight.position", glm::vec3(1.0f, 1.5f, 5.0f));
        shader->set_vec3("pointLight.ambient", glm::vec3(0.1f));
        shader->set_vec3("pointLight.diffuse", glm::vec3(1.0f, 0.9f, 0.7f));
        shader->set_vec3("pointLight.specular", glm::vec3(1.0f));
        shader->set_float("pointLight.constant", 1.0f);
        shader->set_float("pointLight.linear", 0.09f);
        shader->set_float("pointLight.quadratic", 0.032f);
        if (m_spotlight_on) {
            shader->set_vec3("spotLight.ambient", glm::vec3(0.2f));
            shader->set_vec3("spotLight.diffuse", glm::vec3(1.0f));
            shader->set_vec3("spotLight.specular", glm::vec3(1.0f));
        } else {
            shader->set_vec3("spotLight.ambient", glm::vec3(0.0f));
            shader->set_vec3("spotLight.diffuse", glm::vec3(0.0f));
            shader->set_vec3("spotLight.specular", glm::vec3(0.0f));
        }
        shader->set_float("spotLight.cutOff", glm::cos(glm::radians(15.0f)));
        shader->set_float("spotLight.outerCutOff", glm::cos(glm::radians(40.0f)));
        shader->set_float("spotLight.constant", 1.0f);
        shader->set_float("spotLight.linear", 0.045f);
        shader->set_float("spotLight.quadratic", 0.0075f);
        shader->set_float("material_shininess", 30.0f);
        shader->set_vec3("viewPos", camera->Position);
    }
}

void MainController::draw_scene_depth(engine::resources::Shader *depth_shader) {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_ferdinand_pos);
    model = glm::rotate(model, glm::radians(m_ferdinand_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f));
    depth_shader->set_mat4("model", model);
    resources->model("ferdinand")->draw(depth_shader);

    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
    depth_shader->set_mat4("model", model);
    resources->model("floor")->draw(depth_shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(0.03f));
    depth_shader->set_mat4("model", model);
    resources->model("lamp")->draw(depth_shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, 5.0f));
    model = glm::scale(model, glm::vec3(0.07f, 0.1f, 0.07f));
    depth_shader->set_mat4("model", model);
    resources->model("lamp1")->draw(depth_shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 5.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.002f));
    depth_shader->set_mat4("model", model);
    resources->model("hangar")->draw(depth_shader);
}

void MainController::update() {
    update_camera();
    update_light();
    update_ferdinand();
    update_events();
}

void MainController::begin_draw() { engine::graphics::OpenGL::clear_buffers(); }

void MainController::draw() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    glm::vec3 point_light_pos = glm::vec3(1.0f, 1.5f, 5.0f);

    auto *depth_shader = resources->shader("point_shadow_depth");
    graphics->begin_point_shadow_render(m_point_shadow, depth_shader, point_light_pos);
    draw_scene_depth(depth_shader);
    graphics->end_point_shadow_render();

    light_setup();
    auto *basic_shader = resources->shader("basic");
    auto *parallax_shader = resources->shader("parallax");
    for (auto *shader: {basic_shader, parallax_shader}) {
        shader->use();
        shader->set_int("depthMap", 5);
        shader->set_float("far_plane", m_point_shadow.far_plane());
        shader->set_vec3("pointLightPos", point_light_pos);
    }
    graphics->bind_point_shadow_map(m_point_shadow, 5);

    draw_floor();
    draw_ferdinand();
    draw_lamp();
    draw_lamp1();
    draw_hangar();
}

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}
}// namespace app

// MainController