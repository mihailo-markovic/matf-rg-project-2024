#include "../include/MainController.hpp"
#include "../include/GUIController.hpp"
#include "../../engine/libs/glad/include/glad/glad.h"
#include "../../engine/test/app/include/app/GUIController.hpp"

#include <engine/core/Controller.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/platform/PlatformEventObserver.hpp>
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
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    engine::graphics::OpenGL::enable_depth_testing();
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_ESCAPE).is_down()) { return false; }
    return true;
}

void MainController::draw_Ferdinand() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *ferdinand = resources->model("ferdinand");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(0.2f));
    shader->set_mat4("model", model);
    ferdinand->draw(shader);
}

void MainController::draw_floor() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    engine::resources::Model *floor = resources->model("floor1");
    engine::resources::Shader *shader = resources->shader("basic");

    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(7.0f, 1.0f, 7.0f));
    shader->set_mat4("model", model);

    floor->draw(shader);
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
    model = glm::scale(model, glm::vec3(0.03f));
    shader->set_mat4("model", model);
    lamp->draw(shader);
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

void MainController::light_setup() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();

    engine::resources::Shader *shader = resources->shader("basic");
    shader->use();

    shader->set_vec3("dirLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
    shader->set_vec3("dirLight.ambient", glm::vec3(0.2));
    shader->set_vec3("dirLight.diffuse", glm::vec3(0.4f));
    shader->set_vec3("dirLight.specular", glm::vec3(1.0f));

    shader->set_vec3("spotLight.position", glm::vec3(0.7f, 3.0f, -1.0f));
    shader->set_vec3("spotLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
    shader->set_vec3("spotLight.ambient", glm::vec3(0.2));
    shader->set_vec3("spotLight.diffuse", glm::vec3(1.0f));
    shader->set_vec3("spotLight.specular", glm::vec3(1.0f));
    shader->set_float("spotLight.cutOff", glm::cos(glm::radians(15.0f)));
    shader->set_float("spotLight.outerCutOff", glm::cos(glm::radians(40.0f)));
    shader->set_float("spotLight.constant", 1.0f);
    shader->set_float("spotLight.linear", 0.045f);
    shader->set_float("spotLight.quadratic", 0.0075f);


    shader->set_float("material_shininess", 30.0f);

    shader->set_vec3("viewPos", camera->Position);

}

void MainController::update() { update_camera(); }

void MainController::begin_draw() { engine::graphics::OpenGL::clear_buffers(); }

void MainController::draw() {

    light_setup();
    draw_floor();
    draw_Ferdinand();
    draw_lamp();
}

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}
}

// MainController