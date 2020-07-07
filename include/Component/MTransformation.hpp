#pragma once

#include "MComponent.hpp"
#include "MMatrix.hpp"
#include "MVector.hpp"

namespace method {

class MTransformation : public MComponent {
private:
    vec3 m_position;
    vec3 m_axis;
    float m_angle;
    vec3 m_scale;

    mat4 m_model;

    void recalculate();

public:
    MTransformation();
    MTransformation(vec3 position, vec3 axis, float angle, vec3 scale);

    void set_position(vec3 position);
    void set_rotation(vec3 axis, float angle);
    void set_scale(vec3 scale);

    mat4 get_model_matrix();

};

}