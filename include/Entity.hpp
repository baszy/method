#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"

namespace method {

class Entity {
private:
    inline void regenerate_model();

public:
    // Parameters
    Vec3  position;
    Vec3  rot_axis;
    float rot_angle;
    float unif_scale;

    // Matrices generated from parameters
    Mat4 model;

    Entity();

    void set_position(Vec3 position);
    void set_rotation(Vec3 rot_axis, float rot_angle);
    void set_scale(float unif_scale);
};

}
