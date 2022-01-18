#pragma once

#include <string>

#include "Shader.hpp"
#include "TextureManager.hpp"
#include "Vector.hpp"

namespace method {

class Material {
public:
    Shader * shader = nullptr;

    float specular_exponent = 32;
    std::string diffuse_handle;
    std::string normal_handle;
    std::string specular_handle;

    Material(std::string diffuse_handle,
             std::string normal_handle,
             std::string specular_handle);
};

}
