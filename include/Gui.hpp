#pragma once

#include "glad/glad.h"

#include "Framebuffer.hpp"
#include "Shader.hpp"
#include "TextureManager.hpp"
#include "Matrix.hpp"

namespace method {

class Sprite {
public:
    Texture texture;
    Mat4 transform;

    Sprite(Texture t, Mat4 m);
};

class Gui {
public:
    Framebuffer * render_target;

    Shader quad_shader;
    GLuint quad_vao;
    Mat4 projection;

    Gui(Framebuffer * target);

    void blit_texture(const Sprite & sprite);
};

};
