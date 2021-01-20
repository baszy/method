#pragma once

#include "GLShader.hpp"
#include "GLTexture.hpp"
#include "Vector.hpp"

namespace method {

struct Material {
    GLShader * shader;

    Vec3 ambient_color = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 diffuse_color = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 specular_color = Vec3(0.0f, 0.0f, 0.0f);
    float specular_exponent;

    GLTexture * diffuse_map;
    GLTexture * normal_map;
    GLTexture * specular_map;
};

}
