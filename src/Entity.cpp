#include "Entity.hpp"

namespace method {

void Entity::recalculate() {
    m_model = scale(m_scale) * rotate(m_axis, m_angle) * translate(m_position);
}

Entity::Entity()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_axis(0.0f, 1.0f, 0.0f)
    , m_angle(0.0f)
    , m_scale(1.0f, 1.0f, 1.0f) {}

mat4 Entity::get_model_matrix() const {
    return m_model;
}

Vec3 Entity::get_position() const {
    return m_position;
}

void Entity::set_position(Vec3 position) {
    m_position = position;
    recalculate();
}

void Entity::set_rotation(Vec3 axis, float angle) {
    m_axis = axis;
    m_angle = angle;
    recalculate();
}

void Entity::set_scale(Vec3 scale) {
    m_scale = scale;
    recalculate();
}

}
