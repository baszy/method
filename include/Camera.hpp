#pragma once

#include "Entity.hpp"
#include "Matrix.hpp"

namespace method {

class Camera : public Entity {
private:
    inline void regenerate_projection();
    inline void regenerate_view();

    static const Vec3 up;

public:
    // Parameters
    float fov;
    float aspect;
    float near;
    float far;
    Vec3 target;

    // Matrices generated from parameters
    Mat4 projection;
    Mat4 view;

    Camera(float fov, float aspect, float near, float far, const Vec3 & target);

    void set_params(float fov, float aspect, float near, float far);
    void set_look(const Vec3 & target);

    Vec4 * get_frustum() const;
};

}
