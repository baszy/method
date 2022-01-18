#include "Gui.hpp"

namespace method {

namespace {

Vec3 quad_vertices[6] = {
    Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f),
    Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 0.0f)
};

const char * vertex_source = METHOD_GLSL(
    layout (location = 0) in vec3 position;

    uniform mat4 projection;
    uniform mat4 transform;

    out vec2 texture_coord;

    void main()
    {
        texture_coord = position.xy;
        gl_Position = projection * transform * vec4(position, 1.0);
    }
);

const char * fragment_source = METHOD_GLSL(
    in vec2 texture_coord;

    uniform sampler2D sprite;

    out vec4 frag_color;

    void main()
    {
        frag_color = vec4(texture(sprite, texture_coord).rgb, 1.0);
    }
);

}

Sprite::Sprite(Texture t, Mat4 m)
    : texture(t)
    , transform(m) {}

Gui::Gui(Framebuffer * target)
    : render_target(target)
    , quad_shader() {

    // TODO: Should we enable SRGB in each renderer, or once at context creation?
    glEnable(GL_FRAMEBUFFER_SRGB);

    quad_shader.set_source(vertex_source, ShaderType::VERTEX);
    quad_shader.set_source(fragment_source, ShaderType::FRAGMENT);
    quad_shader.compile();

    // VAO for the quad
    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(0);

    // TODO: useless?
    glBindVertexArray(0);

    projection = orthographic(0.0f, (float)render_target->size.x,
        (float)render_target->size.y, 0.0f, -1.0f, 1.0f);
}

void Gui::blit_texture(const Sprite & sprite) {
    glBindFramebuffer(GL_FRAMEBUFFER, render_target->fbo_id);
    glViewport(0, 0, render_target->size.x, render_target->size.y);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    quad_shader.use();
    quad_shader.set_uniform("projection", projection);
    quad_shader.set_uniform("transform", sprite.transform);

    quad_shader.set_uniform("sprite", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite.texture.id);

    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

}
