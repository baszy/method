#pragma once

#include "Entity.hpp"
#include "Matrix.hpp"

namespace method {

class Camera : public Entity {
private:
    mat4 m_projection;
    mat4 m_view;

public:
    Camera(float fov, float aspect, float near, float far);

    mat4 get_projection_matrix() const;
    mat4 get_view_matrix() const;
    void set_look(const Vec3 & target);

};

}
