#include "Camera.hpp"

namespace method {

Camera::Camera(float fov, float aspect, float near, float far)
    : projection(perspective(fov, aspect, near, far)) {}

void Camera::set_look(const Vec3 & target) {
    direction = target;
    view = look_at(m_position, direction, Vec3(0.0f, 1.0f, 0.0f)); 
}

}
