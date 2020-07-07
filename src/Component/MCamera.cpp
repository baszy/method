#include "Component/MCamera.hpp"

namespace method {
MCamera::MCamera(float fov, float aspect, float near, float far)
    : m_projection(perspective(fov, aspect, near, far)) {}

mat4 MCamera::get_projection() {
    return m_projection;
}

}