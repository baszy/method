#pragma once

#include <vector>

#include "Camera.hpp"
#include "Light.hpp"
#include "Prop.hpp"

namespace method {

struct Scene {
    std::vector<Camera *> cameras;
    std::vector<Light *> lights;
    std::vector<Prop *> props;
};

}
