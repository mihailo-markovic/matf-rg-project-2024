/**
 * @file GraphicsController.hpp
 * @brief Defines the GraphicsController class that initializes OpenGL and ImGUI, and provides basic drawing functions.
*/

#ifndef GRAPHICSCONTROLLER_HPP
#define GRAPHICSCONTROLLER_HPP

#include <engine/core/Controller.hpp>
#include <engine/graphics/Camera.hpp>
#include <engine/platform/PlatformEventObserver.hpp>
#include <engine/resources/Model.hpp>
#include <engine/graphics/PointShadow.hpp>

struct ImGuiContext;

namespace engine::resources {
class Skybox;

class Shader;
}// namespace engine::resources

namespace engine::graphics {
/**
* @brief Parameters used to define a perspective projection matrix.
*/
struct PerspectiveMatrixParams {
    float FOV;
    float Width;
    float Height;
    float Near;
    float Far;
};

/**
* @brief Parameters used to define an orthographic projection matrix.
*/
struct OrthographicMatrixParams {
    float Left;
    float Right;
    float Bottom;
    float Top;
    float Near;
    float Far;
};

enum ProjectionType {
    Perspective,
    Orthographic
};

/**
* @class GraphicsController
* @brief Implements basic drawing methods that the @ref core::App implementation uses.
*
* This class should implement all the complex functions needed for drawing an entity in the scene.
* For example @ref GraphicsController::draw_skybox.
*/
class GraphicsController final : public core::Controller {
public:
    std::string_view name() const override;

    /**
    * @brief Calls internal methods for the beginning of gui drawing. Should be called in pair with @ref GraphicsController::end_gui.
    *
    * Example:
    * @code
    * auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    * graphics->begin_gui();
    * ImGui::Begin("Camera info");
    * const auto &c = ...;
    * ImGui::Text("Camera position: (%f, %f, %f)", c.Position.x, c.Position.y, c.Position.z);
    * ImGui::Text("(Yaw, Pitch): (%f, %f)", c.Yaw, c.Pitch);
    * ImGui::Text("Camera front: (%f, %f, %f)", c.Front.x, c.Front.y, c.Front.z);
    * ImGui::End();
    * graphics->end_gui();
    * @endcode
    */
    void begin_gui();

    /**
    * @brief Calls internal method for the ending of gui drawing. Should be called in pair with @ref GraphicsController::begin_gui.
    */
    void end_gui();

    /**
    * @brief Draws a @ref resources::Skybox with the @ref resources::Shader.
    */
    void draw_skybox(const resources::Shader *shader, const resources::Skybox *skybox);

    void draw_parallax_map(const resources::Shader *shader,
                           resources::Model *model,
                           const glm::mat4 &model_matrix,
                           float height_scale,
                           const glm::vec3 &dir_light_dir,
                           const glm::vec3 &spot_light_pos);

    /**
    * @brief Initializes point shadow mapping for a point light.
    * @param near Near plane of the shadow frustum.
    * @param far Far plane of the shadow frustum.
    * @returns Initialized PointShadow object.
    */
    PointShadow init_point_shadow(float near = 1.0f, float far = 25.0f);

    /**
    * @brief Renders the scene into the depth cubemap from the point light's perspective.
    * @param shadow The PointShadow object.
    * @param depth_shader The depth shader (must be point_shadow_depth.glsl).
    * @param light_pos Position of the point light.
    */
    void begin_point_shadow_render(const PointShadow &shadow,
                                   const resources::Shader *depth_shader,
                                   const glm::vec3 &light_pos);

    /**
    * @brief Ends the shadow render pass and restores the viewport.
    */
    void end_point_shadow_render();

    Camera *camera() { return &m_camera; }

    /**
    * @brief Compute the projection matrix.
    * @returns Return perspective projection by default.
    */
    template<ProjectionType type = Perspective>
    glm::mat4 projection_matrix() const {
        if constexpr (type == Perspective) {
            return glm::perspective(m_perspective_params.FOV,
                                    m_perspective_params.Width / m_perspective_params.Height,
                                    m_perspective_params.Near, m_perspective_params.Far);
        } else {
            return glm::ortho(m_ortho_params.Left, m_ortho_params.Right, m_ortho_params.Bottom, m_ortho_params.Top,
                              m_ortho_params.Near, m_ortho_params.Far);
        }
    }

    /**
    * @brief Compute the projection matrix.
    * @returns Return perspective projection by default.
    */
    glm::mat4 projection_matrix(ProjectionType type = Perspective) const {
        switch (type) {
            case Perspective: return projection_matrix<Perspective>();
            case Orthographic: return projection_matrix<Orthographic>();
            default: RG_SHOULD_NOT_REACH_HERE("Unsupported type");
        }
    }

    /**
    * @brief Use this function to change the perspective projection matrix parameters.
    * Projection matrix is always computed when the @ref GraphicsController::projection_matrix is called.
    * @returns @ref PerspectiveMatrixParams
    */
    PerspectiveMatrixParams &perspective_params() { return m_perspective_params; }

    /**
    * @brief Get the current @ref PerspectiveMatrixParams values.
    * @returns @ref PerspectiveMatrixParams
    */
    const PerspectiveMatrixParams &perspective_params() const { return m_perspective_params; }

    /**
    * @brief Use this function to change the orthographic projection matrix parameters.
    * Projection matrix is always computed
    * when @ref GraphicsController::projection_matrix is called.
    * @returns @ref PerspectiveMatrixParams
    */
    OrthographicMatrixParams &orthographic_params() { return m_ortho_params; }

    /**
    * @brief Get the current @ref OrthographicMatrixParams values.
    * @returns @ref PerspectiveMatrixParams
    */
    const OrthographicMatrixParams &orthographic_params() const { return m_ortho_params; }

private:
    /**
    * @brief Initializes OpenGL, ImGUI, and projection matrix params;
    */
    void initialize() override;

    void terminate();

    PerspectiveMatrixParams m_perspective_params{};
    OrthographicMatrixParams m_ortho_params{};

    glm::mat4 m_projection_matrix{};
    Camera m_camera{};
    ImGuiContext *m_imgui_context{};
};

/**
* @class GraphicsPlatformEventObserver
* @brief Observers change in window size in order to update the projection matrix.
*/
class GraphicsPlatformEventObserver final : public platform::PlatformEventObserver {
public:
    explicit GraphicsPlatformEventObserver(GraphicsController *graphics)
    : m_graphics(graphics) {}

    void on_window_resize(int width, int height) override;

private:
    GraphicsController *m_graphics;
};
}// namespace engine::graphics
#endif//GRAPHICSCONTROLLER_HPP
