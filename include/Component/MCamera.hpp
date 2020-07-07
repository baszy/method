#pragma once

#include "MComponent.hpp"
#include "MMatrix.hpp"

namespace method {

class MCamera : public MComponent {
private:
    mat4 m_projection;

public:
    MCamera(float fov, float aspect, float near, float far);

    mat4 get_projection();

};

}