#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"

namespace method {

class Entity {
protected:
    Vec3 m_position;
    Vec3 m_axis;
    float m_angle;
    Vec3 m_scale;
    mat4 m_model;

    void recalculate();

public:
    Entity();

    mat4 get_model_matrix() const;
    Vec3 get_position() const;
    void set_position(Vec3 position);
    void set_rotation(Vec3 axis, float angle);
    void set_scale(Vec3 scale);

};

}
