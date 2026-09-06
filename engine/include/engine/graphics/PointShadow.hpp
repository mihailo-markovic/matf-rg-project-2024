//
// Created by grafika on 9/4/26.
//

#ifndef MATF_RG_PROJECT_POINTSHADOW_HPP
#define MATF_RG_PROJECT_POINTSHADOW_HPP
#include <cstdint>

namespace engine::graphics {
/**
* @class PointShadow
* @brief Represents a point light shadow map using a depth cubemap.
* Use @ref GraphicsController::begin_point_shadow_render,
* @ref GraphicsController::end_point_shadow_render to generate the shadow map,
* and @ref GraphicsController::draw_with_point_shadows to render the scene with shadows.
*/
class PointShadow {
    friend class GraphicsController;

public:
    PointShadow() = default;

    PointShadow(uint32_t fbo, uint32_t depth_cubemap, float near, float far)
    : m_fbo(fbo)
  , m_depth_cubemap(depth_cubemap)
  , m_near_plane(near)
  , m_far_plane(far) {}

    uint32_t depth_cubemap() const { return m_depth_cubemap; }
    uint32_t fbo() const { return m_fbo; }
    float far_plane() const { return m_far_plane; }

    void destroy();

private:
    uint32_t m_fbo{0};
    uint32_t m_depth_cubemap{0};
    float m_near_plane{1.0f};
    float m_far_plane{25.0f};
};
}//namespace engine::graphics
#endif //MATF_RG_PROJECT_POINTSHADOW_HPP