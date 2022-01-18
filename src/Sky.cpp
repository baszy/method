#include "Sky.hpp"

#include "Noise.hpp"

namespace method {

namespace {

const Vec3 vertices[6] = {
    Vec3(1.0f, 1.0f, 1.0f), Vec3(-1.0f, 1.0f, 1.0f), Vec3(-1.0f, -1.0f, 1.0f),
    Vec3(1.0f, 1.0f, 1.0f), Vec3(-1.0f, -1.0f, 1.0f), Vec3(1.0f, -1.0f, 1.0f)
};

}

Sky::Sky(Shader & shader, DirectionLight & sun)
    : target(IVec2(500, 500), FbType::CUBEMAP),
      shader(shader),
      sun(sun) {

    // VAO for the quad used to generate atmosphere cubemap
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vec3), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(0);

    noise = white_noise(500, 500, 216);
}

void Sky::draw(const Camera & camera) {
    // Sky texture generation
    target.use();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_ALWAYS);

    shader.use();

    shader.set_uniform("camera_position", camera.position);

    shader.set_uniform("sun_direction", normalize(sun.direction));
    shader.set_uniform("sun_color", sun.color);

    shader.set_uniform("noise", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noise.id);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

const DirectionLight & Sky::get_sun() const {
    return sun;
}

const Framebuffer & Sky::get_framebuffer() const {
    return target;
}

}
