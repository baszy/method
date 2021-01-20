#include "Camera.hpp"

namespace method {

Camera::Camera(float fov, float aspect, float near, float far)
    : m_projection(perspective(fov, aspect, near, far)) {}

mat4 Camera::get_projection_matrix() const {
    return m_projection;
}

mat4 Camera::get_view_matrix() const {
    return m_view;
}

void Camera::set_look(const Vec3 & target) {
    m_view = look_at(m_position, target, Vec3(0.0f, 1.0f, 0.0f)); 
}

}
