#pragma once

#include "glad/glad.h"

#include "Framebuffer.hpp"
#include "GLShader.hpp"
#include "MeshManager.hpp"
#include "TextureManager.hpp"
#include "Scene.hpp"

namespace method {

class RenderSystem {
public:
    Framebuffer * render_target;
    Framebuffer sky_target;

    MeshManager * mesh_manager;
    TextureManager * texture_manager;

    GLShader sky_shader;
    GLShader quad_shader;
    GLShader axes_shader;
    GLuint sky_vao;
    GLuint quad_vao;
    GLuint axes_vao;

    GLTexture * noise;

    RenderSystem(Framebuffer & target, MeshManager & mm, TextureManager & tm);

    void draw(const Scene & scene);
};

}
