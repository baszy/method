#pragma once

#include "glad/glad.h"

#include "Framebuffer.hpp"
#include "Shader.hpp"
#include "MeshManager.hpp"
#include "Scene.hpp"
#include "TextureManager.hpp"

namespace method {

const int DEBUG_MAX_LINES = 100;

class RenderSystem {
public:
    MeshManager * mesh_manager;
    TextureManager * texture_manager;

    Shader cubemap_shader;
    GLuint cubemap_vao;

    Vec3 * debug_vertices;
    int debug_lines;
    Shader debug_shader;
    GLuint debug_vao;
    GLuint debug_vbo;

    Shader frustum_shader;
    GLuint frustum_vao;

    Framebuffer shadow_fb;
    Mat4 shadow_projection;
    Shader shadow_shader;

    RenderSystem(MeshManager & mm, TextureManager & tm);
    ~RenderSystem();

    void draw(Framebuffer * fb, const Scene & scene);
    void debug_ray(Vec3 start, Vec3 end, Vec3 color);

    void print_gl_info() const;
};

}
