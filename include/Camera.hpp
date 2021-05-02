#pragma once

#include "Entity.hpp"
#include "Matrix.hpp"

namespace method {

class Camera : public Entity {
public:
    Vec3 direction;
    Mat4 projection;
    Mat4 view;

    Camera(float fov, float aspect, float near, float far);

    void set_look(const Vec3 & target);
};

}
