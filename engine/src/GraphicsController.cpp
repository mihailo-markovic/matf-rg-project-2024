
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/Skybox.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace engine::graphics {

void GraphicsController::initialize() {
    const int opengl_initialized = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    RG_GUARANTEE(opengl_initialized, "OpenGL failed to init!");

    auto platform = engine::core::Controller::get<platform::PlatformController>();
    auto handle = platform->window()->handle_();
    m_perspective_params.FOV = glm::radians(m_camera.Zoom);
    m_perspective_params.Width = static_cast<float>(platform->window()->width());
    m_perspective_params.Height = static_cast<float>(platform->window()->height());
    m_perspective_params.Near = 0.1f;
    m_perspective_params.Far = 100.f;
    m_ortho_params.Bottom = 0.0f;
    m_ortho_params.Top = static_cast<float>(platform->window()->height());
    m_ortho_params.Left = 0.0f;
    m_ortho_params.Right = static_cast<float>(platform->window()->width());
    m_ortho_params.Near = 0.1f;
    m_ortho_params.Far = 100.0f;

    platform->register_platform_event_observer(std::make_unique<GraphicsPlatformEventObserver>(this));
    CHECKED_GL_CALL(glViewport, 0, 0, platform->window()->width(), platform->window()->height());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    RG_GUARANTEE(ImGui_ImplGlfw_InitForOpenGL(handle, true), "ImGUI failed to initialize for OpenGL");
    RG_GUARANTEE(ImGui_ImplOpenGL3_Init("#version 330 core"), "ImGUI failed to initialize for OpenGL");
}

void GraphicsController::terminate() {
    if (ImGui::GetCurrentContext()) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

void GraphicsPlatformEventObserver::on_window_resize(int width, int height) {
    m_graphics->perspective_params().Width = static_cast<float>(width);
    m_graphics->perspective_params().Height = static_cast<float>(height);
    m_graphics->orthographic_params().Right = static_cast<float>(width);
    m_graphics->orthographic_params().Top = static_cast<float>(height);
    CHECKED_GL_CALL(glViewport, 0, 0, width, height);
}

std::string_view GraphicsController::name() const { return "GraphicsController"; }

void GraphicsController::begin_gui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GraphicsController::end_gui() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GraphicsController::draw_skybox(const resources::Shader *shader, const resources::Skybox *skybox) {
    glm::mat4 view = glm::mat4(glm::mat3(m_camera.view_matrix()));
    shader->use();
    shader->set_mat4("view", view);
    shader->set_mat4("projection", projection_matrix<>());
    CHECKED_GL_CALL(glDepthFunc, GL_LEQUAL);
    CHECKED_GL_CALL(glBindVertexArray, skybox->vao());
    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, skybox->texture());
    CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 36);
    CHECKED_GL_CALL(glBindVertexArray, 0);
    CHECKED_GL_CALL(glDepthFunc, GL_LESS);// set depth function back to default
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_CUBE_MAP, 0);
}

void GraphicsController::draw_parallax_map(const resources::Shader *shader,
                                           resources::Model *model,
                                           const glm::mat4 &model_matrix,
                                           float height_scale) {
    shader->use();
    shader->set_mat4("projection", projection_matrix());
    shader->set_mat4("view", m_camera.view_matrix());
    shader->set_mat4("model", model_matrix);
    shader->set_vec3("viewPos", m_camera.Position);
    shader->set_float("heightScale", height_scale);
    shader->set_vec3("dirLight_dir", glm::vec3(1.0f, -1.0f, 1.0f));
    shader->set_vec3("spotLight_pos", glm::vec3(0.7f, 3.0f, -1.0f));
    shader->set_vec3("spotLight_dir", glm::vec3(0.0f, -1.0f, 0.0f));
    shader->set_vec3("pointLight_pos", glm::vec3(1.0f, 1.5f, 5.0f));
    model->draw(shader);
}

PointShadow GraphicsController::init_point_shadow(float near, float far) {
    uint32_t fbo, depth_cubemap;
    OpenGL::init_point_shadow(1024, 1024, fbo, depth_cubemap);
    return PointShadow(fbo, depth_cubemap, near, far);
}

void GraphicsController::begin_point_shadow_render(const PointShadow &shadow, const resources::Shader *depth_shader, const glm::vec3 &light_pos) {
    auto platform = engine::core::Controller::get<platform::PlatformController>();

    float near = shadow.m_near_plane;
    float far = shadow.m_far_plane;

    glm::mat4 shadow_proj = glm::perspective(glm::radians(90.0f), 1.0f, near, far);
    std::vector<glm::mat4> shadow_transforms = {
            shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),
            shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
            shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
            shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
            shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),
            shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)),
    };

    CHECKED_GL_CALL(glViewport, 0, 0, 1024, 1024);
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, shadow.m_fbo);
    CHECKED_GL_CALL(glClear, GL_DEPTH_BUFFER_BIT);

    depth_shader->use();
    for (int i = 0; i < 6; ++i) { depth_shader->set_mat4("shadowMatrices[" + std::to_string(i) + "]", shadow_transforms[i]); }
    depth_shader->set_vec3("lightPos", light_pos);
    depth_shader->set_float("far_plane", far);
}

void GraphicsController::end_point_shadow_render() {
    auto platform = engine::core::Controller::get<platform::PlatformController>();
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    CHECKED_GL_CALL(glViewport, 0, 0,
                    platform->window()->width(),
                    platform->window()->height());
}
}// namespace engine::graphics
