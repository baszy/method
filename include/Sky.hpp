#pragma once

#include "Camera.hpp"
#include "Framebuffer.hpp"
#include "Shader.hpp"
#include "Light.hpp"
#include "MeshManager.hpp"
#include "TextureManager.hpp"

namespace method {

class Sky {
private:
    Framebuffer target;
    GLuint vao;

    Shader & shader;
    DirectionLight & sun;

    Texture noise;

public:
    Sky(Shader & shader, DirectionLight & sun);

    void draw(const Camera & camera);
    const DirectionLight & get_sun() const;
    const Framebuffer & get_framebuffer() const;
};

}
