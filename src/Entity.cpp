#include "Entity.hpp"

namespace method {

inline void Entity::regenerate_model() {
    model = translate(position) * scale(Vec3(unif_scale)) * rotate(rot_axis, rot_angle);
}

Entity::Entity()
    : position(0.0f, 0.0f, 0.0f)
    , rot_axis(0.0f, 1.0f, 0.0f)
    , rot_angle(0.0f)
    , unif_scale(1.0f) {}

void Entity::set_position(Vec3 position) {
    this->position = position;

    regenerate_model();
}

void Entity::set_rotation(Vec3 rot_axis, float rot_angle) {
    this->rot_axis = rot_axis;
    this->rot_angle = rot_angle;

    regenerate_model();
}

void Entity::set_scale(float unif_scale) {
    this->unif_scale = unif_scale;

    regenerate_model();
}

}
