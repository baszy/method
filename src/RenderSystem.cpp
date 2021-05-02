#include "RenderSystem.hpp"

#include <cstdlib>

#include "Camera.hpp"
#include "Noise.hpp"
#include "Vector.hpp"

namespace method {

namespace {

const Vec3 quad_vertices[6] = {
    Vec3(1.0f, 1.0f, 1.0f), Vec3(-1.0f, 1.0f, 1.0f), Vec3(-1.0f, -1.0f, 1.0f),
    Vec3(1.0f, 1.0f, 1.0f), Vec3(-1.0f, -1.0f, 1.0f), Vec3(1.0f, -1.0f, 1.0f)
};

const Vec3 sky_vertices[36] = {
    Vec3(-1.0f,  1.0f, -1.0f),
    Vec3(-1.0f, -1.0f, -1.0f),
    Vec3( 1.0f, -1.0f, -1.0f),
    Vec3( 1.0f, -1.0f, -1.0f),
    Vec3( 1.0f,  1.0f, -1.0f),
    Vec3(-1.0f,  1.0f, -1.0f),

    Vec3(-1.0f, -1.0f,  1.0f),
    Vec3(-1.0f, -1.0f, -1.0f),
    Vec3(-1.0f,  1.0f, -1.0f),
    Vec3(-1.0f,  1.0f, -1.0f),
    Vec3(-1.0f,  1.0f,  1.0f),
    Vec3(-1.0f, -1.0f,  1.0f),

    Vec3( 1.0f, -1.0f, -1.0f),
    Vec3( 1.0f, -1.0f,  1.0f),
    Vec3( 1.0f,  1.0f,  1.0f),
    Vec3( 1.0f,  1.0f,  1.0f),
    Vec3( 1.0f,  1.0f, -1.0f),
    Vec3( 1.0f, -1.0f, -1.0f),

    Vec3(-1.0f, -1.0f,  1.0f),
    Vec3(-1.0f,  1.0f,  1.0f),
    Vec3( 1.0f,  1.0f,  1.0f),
    Vec3( 1.0f,  1.0f,  1.0f),
    Vec3( 1.0f, -1.0f,  1.0f),
    Vec3(-1.0f, -1.0f,  1.0f),

    Vec3(-1.0f,  1.0f, -1.0f),
    Vec3( 1.0f,  1.0f, -1.0f),
    Vec3( 1.0f,  1.0f,  1.0f),
    Vec3( 1.0f,  1.0f,  1.0f),
    Vec3(-1.0f,  1.0f,  1.0f),
    Vec3(-1.0f,  1.0f, -1.0f),

    Vec3(-1.0f, -1.0f, -1.0f),
    Vec3(-1.0f, -1.0f,  1.0f),
    Vec3( 1.0f, -1.0f, -1.0f),
    Vec3( 1.0f, -1.0f, -1.0f),
    Vec3(-1.0f, -1.0f,  1.0f),
    Vec3( 1.0f, -1.0f,  1.0f)
};

const Vec3 axes_vertices[12] = {
    /* Position          */ /* Color             */
    Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f),
    Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f),

    Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f),
    Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f),

    Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f),
    Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f)
};

const char * vertex_source = METHOD_GLSL(
    layout (location = 0) in vec3 position;

    uniform mat4 projection;
    uniform mat4 view;

    out vec3 texture_coord;

    void main() {
        texture_coord = position;
        vec4 result = projection * mat4(mat3(view)) * vec4(position, 1.0f);
        gl_Position = result.xyww;
    }
);

const char * fragment_source = METHOD_GLSL(
    in vec3 texture_coord;

    uniform samplerCube sky_map;

    out vec4 frag_color;

    void main() {
        frag_color = texture(sky_map, texture_coord);
    }
);

const char * axes_vertex_source = METHOD_GLSL(
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;

    uniform mat4 projection;
    uniform mat4 view;

    out interp {
        vec3 position;
        vec3 color;
    } vertex;

    void main() {
        vertex.position = position;
        vertex.color = color;
        gl_Position = projection * view * vec4(position, 1.0f);
    }
);

const char * axes_fragment_source = METHOD_GLSL(
    in interp {
        vec3 position;
        vec3 color;
    } vertex;

    out vec4 frag_color;

    void main() {
        frag_color = vec4(vertex.color, 1.0f);
    }
);

}

RenderSystem::RenderSystem(Framebuffer & target, MeshManager & mm,
    TextureManager & tm)
    : render_target(&target)
    , sky_target(IVec2(500, 500), true)
    , mesh_manager(&mm)
    , texture_manager(&tm)
    , sky_shader()
    , quad_shader()
    , axes_shader() {

    // TODO: Should this be moved to the Window where we draw framebuffers?
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_FRAMEBUFFER_SRGB);

    glEnable(GL_DITHER);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    sky_shader.load("shader/atmosphere.vert", ShaderType::VERTEX);
    sky_shader.load("shader/atmosphere.geom", ShaderType::GEOMETRY);
    sky_shader.load("shader/atmosphere.frag", ShaderType::FRAGMENT);
    sky_shader.compile();

    quad_shader.set_source(vertex_source, ShaderType::VERTEX);
    quad_shader.set_source(fragment_source, ShaderType::FRAGMENT);
    quad_shader.compile();

    axes_shader.set_source(axes_vertex_source, ShaderType::VERTEX);
    axes_shader.set_source(axes_fragment_source, ShaderType::FRAGMENT);
    axes_shader.compile();

    // VAO for the skybox
    glGenVertexArrays(1, &sky_vao);
    glBindVertexArray(sky_vao);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(Vec3), sky_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(0);

    // VAO for the quad used to generate atmosphere cubemap
    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);

    vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vec3), quad_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(0);

    // VAO for the 3d axes
    glGenVertexArrays(1, &axes_vao);
    glBindVertexArray(axes_vao);

    vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(Vec3), axes_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, sizeof(Vec3) / sizeof(float), GL_FLOAT, GL_FALSE,
        2 * sizeof(Vec3), (void *)(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, sizeof(Vec3) / sizeof(float), GL_FLOAT, GL_FALSE,
        2 * sizeof(Vec3), (void *)(1 * sizeof(Vec3)));
    glEnableVertexAttribArray(1);

    // TODO: useless?
    glBindVertexArray(0);

    noise = white_noise(500, 500, 216);
}

void RenderSystem::draw(const Scene & scene) {
    glEnable(GL_DEPTH_TEST);

    // Sky texture generation
    glBindFramebuffer(GL_FRAMEBUFFER, sky_target.fbo_id);
    glViewport(0, 0, sky_target.size.x, sky_target.size.y);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_ALWAYS);

    sky_shader.use();

    sky_shader.set_uniform("camera_position", scene.camera->get_position());

    sky_shader.set_uniform("sun_direction", normalize(scene.sun.direction));
    sky_shader.set_uniform("sun_color", scene.sun.color);

    sky_shader.set_uniform("noise", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, noise->id);

    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Main scene rendering
    glBindFramebuffer(GL_FRAMEBUFFER, render_target->fbo_id);
    glViewport(0, 0, render_target->size.x, render_target->size.y);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthFunc(GL_LESS);

    for (Prop * prop : scene.props) {
        const Material * mat = prop->material;
        GLShader * sh = mat->shader;
        sh->use();

        sh->set_uniform("projection", scene.camera->projection);
        sh->set_uniform("view", scene.camera->view);
        sh->set_uniform("model", prop->get_model_matrix());

        sh->set_uniform("camera_position", scene.camera->get_position());

        sh->set_uniform("point_lights_length",
            scene.point_lights_length);
        sh->set_uniform("point_lights_positions",
            scene.point_lights_positions, scene.point_lights_length);
        sh->set_uniform("point_lights_colors",
            scene.point_lights_colors, scene.point_lights_length);

        sh->set_uniform("sun_direction", scene.sun.direction);
        sh->set_uniform("sun_color", scene.sun.color);

        sh->set_uniform("object_color", mat->diffuse_color);
        sh->set_uniform("specular_exponent", mat->specular_exponent);

        sh->set_uniform("diffuse_texture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,
            texture_manager->get(mat->diffuse_map_handle)->id);

        sh->set_uniform("normal_texture", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,
            texture_manager->get(mat->normal_map_handle)->id);

        sh->set_uniform("displacement_texture", 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,
            texture_manager->get(mat->displacement_map_handle)->id);

        const GLVAO * vao = mesh_manager->get(prop->mesh_handle);

        glBindVertexArray(vao->vao_id);
        glDrawElements(GL_TRIANGLES, vao->num_indices, GL_UNSIGNED_INT, 0);
    }

    glDepthFunc(GL_LEQUAL);

    quad_shader.use();

    quad_shader.set_uniform("projection", scene.camera->projection);
    quad_shader.set_uniform("view", scene.camera->view);

    quad_shader.set_uniform("sky_map", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sky_target.color_buffer_id);

    glBindVertexArray(sky_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    axes_shader.use();

    axes_shader.set_uniform("projection", scene.camera->projection);
    axes_shader.set_uniform("view", scene.camera->view);

    glBindVertexArray(axes_vao);
    glDrawArrays(GL_LINES, 0, 6);
}

}
