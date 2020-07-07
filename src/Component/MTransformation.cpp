#include "Component/MTransformation.hpp"

namespace method {

void MTransformation::recalculate() {
    m_model = scale(m_scale) * rotate(m_axis, m_angle) * translate(m_position);
}

MTransformation::MTransformation()
    : MTransformation(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 0.0f,
        vec3(1.0f, 1.0f, 1.0f)) {}

MTransformation::MTransformation(vec3 position, vec3 axis, float angle,
    vec3 scale)
    : MComponent()
    , m_position(position)
    , m_axis(axis)
    , m_angle(angle)
    , m_scale(scale) {

    recalculate();
}

void MTransformation::set_position(vec3 position) {
    m_position = position;
    recalculate();
}

void MTransformation::set_rotation(vec3 axis, float angle) {
    m_axis = axis;
    m_angle = angle;
    recalculate();
}

void MTransformation::set_scale(vec3 scale) {
    m_scale = scale;
    recalculate();
}

mat4 MTransformation::get_model_matrix() {
    return m_model;
}

}