#pragma once

#include "GLShader.hpp"
#include "TextureManager.hpp"
#include "Vector.hpp"

namespace method {

struct Material {
    GLShader * shader;

    Vec3 ambient_color = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 diffuse_color = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 specular_color = Vec3(0.0f, 0.0f, 0.0f);
    float specular_exponent;

    HotloaderIndex diffuse_map_handle;
    HotloaderIndex normal_map_handle;
    HotloaderIndex displacement_map_handle;
};

}
