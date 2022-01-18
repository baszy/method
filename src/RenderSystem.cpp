#include "RenderSystem.hpp"

#include <iostream>

#include "Camera.hpp"
#include "Math.hpp"
#include "Vector.hpp"

using namespace std;

namespace method {

namespace {

const Vec3 cubemap_vertices[36] = {
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

const Vec3 debug_vertices[12] = {
    /* Position          */ /* Color             */
    Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f),
    Vec3(1.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f),

    Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f),
    Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f),

    Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f),
    Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f)
};

const Vec3 frustum_vertices[60] = {
    /* Position             */ /* Color             */

    // Near plane
    Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 0.0f),
    Vec3( 1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 0.0f),
    Vec3( 1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 0.0f),
    Vec3( 1.0f,  1.0f, -1.0f), Vec3(1.0f, 1.0f, 0.0f),
    Vec3( 1.0f,  1.0f, -1.0f), Vec3(1.0f, 1.0f, 0.0f),
    Vec3(-1.0f,  1.0f, -1.0f), Vec3(1.0f, 1.0f, 0.0f),
    Vec3(-1.0f,  1.0f, -1.0f), Vec3(1.0f, 1.0f, 0.0f),
    Vec3(-1.0f, -1.0f, -1.0f), Vec3(1.0f, 1.0f, 0.0f),

    // Far plane
    Vec3(-1.0f, -1.0f,  1.0f), Vec3(1.0f, 0.0f, 1.0f),
    Vec3( 1.0f, -1.0f,  1.0f), Vec3(1.0f, 0.0f, 1.0f),
    Vec3( 1.0f, -1.0f,  1.0f), Vec3(1.0f, 0.0f, 1.0f),
    Vec3( 1.0f,  1.0f,  1.0f), Vec3(1.0f, 0.0f, 1.0f),
    Vec3( 1.0f,  1.0f,  1.0f), Vec3(1.0f, 0.0f, 1.0f),
    Vec3(-1.0f,  1.0f,  1.0f), Vec3(1.0f, 0.0f, 1.0f),
    Vec3(-1.0f,  1.0f,  1.0f), Vec3(1.0f, 0.0f, 1.0f),
    Vec3(-1.0f, -1.0f,  1.0f), Vec3(1.0f, 0.0f, 1.0f),

    // Frustum edges
    Vec3(-1.0f, -1.0f,  1.0f), Vec3(0.0f, 1.0f, 1.0f),
    Vec3(-1.0f, -1.0f, -1.0f), Vec3(0.0f, 1.0f, 1.0f),
    Vec3( 1.0f, -1.0f,  1.0f), Vec3(0.0f, 1.0f, 1.0f),
    Vec3( 1.0f, -1.0f, -1.0f), Vec3(0.0f, 1.0f, 1.0f),
    Vec3(-1.0f,  1.0f,  1.0f), Vec3(0.0f, 1.0f, 1.0f),
    Vec3(-1.0f,  1.0f, -1.0f), Vec3(0.0f, 1.0f, 1.0f),
    Vec3( 1.0f,  1.0f,  1.0f), Vec3(0.0f, 1.0f, 1.0f),
    Vec3( 1.0f,  1.0f, -1.0f), Vec3(0.0f, 1.0f, 1.0f),

    // Axes
    Vec3( 0.0f,  0.0f,  0.0f), Vec3(1.0f, 0.0f, 0.0f),
    Vec3( 1.0f,  0.0f,  0.0f), Vec3(1.0f, 0.0f, 0.0f),
    Vec3( 0.0f,  0.0f,  0.0f), Vec3(0.0f, 1.0f, 0.0f),
    Vec3( 0.0f,  1.0f,  0.0f), Vec3(0.0f, 1.0f, 0.0f),
    Vec3( 0.0f,  0.0f,  0.0f), Vec3(0.0f, 0.0f, 1.0f),
    Vec3( 0.0f,  0.0f,  1.0f), Vec3(0.0f, 0.0f, 1.0f)
};

Vec3 quad_vertices[6] = {
    Vec3(1.0f, 1.0f, 0.0f), Vec3(-1.0f, 1.0f, 0.0f), Vec3(-1.0f, -1.0f, 0.0f),
    Vec3(1.0f, 1.0f, 0.0f), Vec3(-1.0f, -1.0f, 0.0f), Vec3(1.0f, -1.0f, 0.0f)
};

Vec2 quad_uvs[6] = {
    Vec2(1.0f, 1.0f), Vec2(0.0f, 1.0f), Vec2(0.0f, 0.0f),
    Vec2(1.0f, 1.0f), Vec2(0.0f, 0.0f), Vec2(1.0f, 0.0f)
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

    uniform samplerCube cubemap;

    out vec4 frag_color;

    void main() {
        frag_color = texture(cubemap, texture_coord);
    }
);

const char * debug_vertex_source = METHOD_GLSL(
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

const char * debug_fragment_source = METHOD_GLSL(
    in interp {
        vec3 position;
        vec3 color;
    } vertex;

    out vec4 frag_color;

    void main() {
        frag_color = vec4(vertex.color, 1.0f);
    }
);

const char * shadow_vertex_source = METHOD_GLSL(
    layout (location = 0) in vec3 position;

    uniform mat4 model;

    uniform mat4 light;

    void main() {
        gl_Position = light * model * vec4(position, 1.0f);
    }
);

const char * shadow_fragment_source = METHOD_GLSL(
    void main() {}
);

const char * frustum_vertex_source = METHOD_GLSL(
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;

    uniform mat4 projection;
    uniform mat4 view;

    uniform mat4 light;

    out interp {
        vec3 position;
        vec3 color;
    } vertex;

    void main() {
        vertex.position = position;
        vertex.color = color;
        // TODO: inverse
        gl_Position = projection * view * inverse(light) * vec4(position, 1.0f);
    }
);

const char * frustum_fragment_source = METHOD_GLSL(
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

RenderSystem::RenderSystem(MeshManager & mm, TextureManager & tm)
    : mesh_manager(&mm),
      texture_manager(&tm),
      cubemap_shader(),
      debug_lines(0),
      debug_shader(),
      frustum_shader(),
      shadow_fb(IVec2(2048, 2048), FbType::DEPTH),
      shadow_shader() {

    // TODO: Should this be moved to the Window where we draw framebuffers?
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // TODO: Should we enable SRGB in each renderer, or once at context creation?
    glEnable(GL_FRAMEBUFFER_SRGB);

    glEnable(GL_DITHER);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    cubemap_shader.set_source(vertex_source, ShaderType::VERTEX);
    cubemap_shader.set_source(fragment_source, ShaderType::FRAGMENT);
    cubemap_shader.compile();

    debug_shader.set_source(debug_vertex_source, ShaderType::VERTEX);
    debug_shader.set_source(debug_fragment_source, ShaderType::FRAGMENT);
    debug_shader.compile();

    frustum_shader.set_source(frustum_vertex_source, ShaderType::VERTEX);
    frustum_shader.set_source(frustum_fragment_source, ShaderType::FRAGMENT);
    frustum_shader.compile();

    shadow_shader.set_source(shadow_vertex_source, ShaderType::VERTEX);
    shadow_shader.set_source(shadow_fragment_source, ShaderType::FRAGMENT);
    shadow_shader.compile();

    // VAO for the skybox
    glGenVertexArrays(1, &cubemap_vao);
    glBindVertexArray(cubemap_vao);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(Vec3), cubemap_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(0);

    // VAO for debug rays
    glGenVertexArrays(1, &debug_vao);
    glBindVertexArray(debug_vao);

    glGenBuffers(1, &debug_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, debug_vbo);
    // Each line is represented by 4 Vec3 (TODO could be 3 with index buffer)
    debug_vertices = new Vec3[DEBUG_MAX_LINES * 4 * sizeof(Vec3)]();
    glBufferData(GL_ARRAY_BUFFER, DEBUG_MAX_LINES * 4 * sizeof(Vec3), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, sizeof(Vec3) / sizeof(float), GL_FLOAT, GL_FALSE,
        2 * sizeof(Vec3), (void *)(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, sizeof(Vec3) / sizeof(float), GL_FLOAT, GL_FALSE,
        2 * sizeof(Vec3), (void *)(1 * sizeof(Vec3)));
    glEnableVertexAttribArray(1);

    // VAO for the frustum cube
    glGenVertexArrays(1, &frustum_vao);
    glBindVertexArray(frustum_vao);

    vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 60 * sizeof(Vec3), frustum_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, sizeof(Vec3) / sizeof(float), GL_FLOAT, GL_FALSE,
        2 * sizeof(Vec3), (void *)(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, sizeof(Vec3) / sizeof(float), GL_FLOAT, GL_FALSE,
        2 * sizeof(Vec3), (void *)(1 * sizeof(Vec3)));
    glEnableVertexAttribArray(1);
}

RenderSystem::~RenderSystem() {
    delete [] debug_vertices;
}

void RenderSystem::draw(Framebuffer * fb, const Scene & scene) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Depth buffer rendering
    shadow_fb.use();
    glClear(GL_DEPTH_BUFFER_BIT);
    // glDisable(GL_CULL_FACE);

    // TODO: Should this be somewhere else
    // Calculate light frustum based on view frustum
    Vec4 * frustum = scene.camera->get_frustum(); // World space

    // Basis vectors of light projection/view
    Vec3 sun_forward = normalize(scene.sky->get_sun().direction);
    Vec3 sun_right   = normalize(cross(sun_forward, Vec3(0.0f, 1.0f, 0.0f)));
    Vec3 sun_up      = normalize(cross(sun_right, sun_forward));

    // Precalculated frustum
    float left   = -300.0f, right = 300.0f;
    float bottom = -300.0f, top   = 300.0f;
    float near   =    1.0f, far   = 600.0f;

    /*
    for (int i = 0; i < 8; i++) {
        Vec3 point = Vec3(frustum[i].x, frustum[i].y, frustum[i].z);
        float x = dot(point, sun_right);
        float y = dot(point, sun_up);
        float z = dot(point, sun_forward);

        if (x < left) left = x;
        else if (x > right) right = x;
        if (y < bottom) bottom = y;
        else if (y > top) top = y;
        if (z < near) near = z;
        else if (z > far) far = z;
    }
    */

    // TODO: 100.0 is arbitrary, test intersection with scene instead?
    Vec3 position = 230.0f * sun_forward;
    Vec3 direction = sun_forward;

    delete [] frustum;

    Mat4 light_pv = orthographic(left, right, bottom, top, near, far)
                  * look_at(position, direction, Vec3(0.0f, 1.0f, 0.0f));

    shadow_shader.use();
    shadow_shader.set_uniform("light", light_pv);

    for (Prop * prop : scene.props) {
        shadow_shader.set_uniform("model", prop->model);

        const Mesh & mesh = mesh_manager->get(prop->mesh);

        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, mesh.data->num_faces * 3, GL_UNSIGNED_INT, 0);
    }

    // Main scene rendering
    fb->use();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_CULL_FACE);

    for (Prop * prop : scene.props) {
        const Material & mat = prop->material;
        Shader * sh = mat.shader;
        sh->use();

        sh->set_uniform("projection", scene.camera->projection);
        sh->set_uniform("view", scene.camera->view);
        sh->set_uniform("model", prop->model);

        sh->set_uniform("light", light_pv);

        sh->set_uniform("camera_position", scene.camera->position);

        char location[30];
        // TODO: Switch to AOS? if convenient
        for (int i = 0; i < scene.point_lights_length; i++) {
            snprintf(location, 30, "point_lights[%d].position", i);
            sh->set_uniform(location, scene.point_lights_positions[i]);
            snprintf(location, 30, "point_lights[%d].color", i);
            sh->set_uniform(location, scene.point_lights_colors[i]);
        }
        sh->set_uniform("point_lights_count", scene.point_lights_length);

        sh->set_uniform("direction_light.direction", scene.sky->get_sun().direction);
        sh->set_uniform("direction_light.color", scene.sky->get_sun().color);
        sh->set_uniform("direction_light.depth_map", 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, shadow_fb.depth_buffer_id);

        sh->set_uniform("specular_exponent", mat.specular_exponent);

        sh->set_uniform("diffuse_texture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,
            texture_manager->get(mat.diffuse_handle).id);

        sh->set_uniform("normal_texture", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,
            texture_manager->get(mat.normal_handle).id);

        sh->set_uniform("specular_texture", 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,
            texture_manager->get(mat.specular_handle).id);

        const Mesh & mesh = mesh_manager->get(prop->mesh);

        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, mesh.data->num_faces * 3, GL_UNSIGNED_INT, 0);
    }

    // Skybox rendering
    glDepthFunc(GL_LEQUAL);

    cubemap_shader.use();

    cubemap_shader.set_uniform("projection", scene.camera->projection);
    cubemap_shader.set_uniform("view", scene.camera->view);

    cubemap_shader.set_uniform("cubemap", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, scene.sky->get_framebuffer().color_buffer_id);

    glBindVertexArray(cubemap_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Debug rays rendering
    debug_shader.use();

    debug_shader.set_uniform("projection", scene.camera->projection);
    debug_shader.set_uniform("view", scene.camera->view);

    glBindVertexArray(debug_vao);
    glDrawArrays(GL_LINES, 0, 2 * debug_lines);

    // light frustum rendering
    frustum_shader.use();

    frustum_shader.set_uniform("light", light_pv);
    frustum_shader.set_uniform("projection", scene.camera->projection);
    frustum_shader.set_uniform("view", scene.camera->view);

    glBindVertexArray(frustum_vao);
    glDrawArrays(GL_LINES, 0, 30);
}

// TODO: This can all be done on the gpu to save on glBufferSubData calls +
// make the system more flexible (rapid updates)
void RenderSystem::debug_ray(Vec3 start, Vec3 end, Vec3 color) {
    size_t offset = debug_lines * 4 * sizeof(Vec3);

    debug_vertices[offset + 0] = start;
    debug_vertices[offset + 1] = color;
    debug_vertices[offset + 2] = end;
    debug_vertices[offset + 3] = color;

    glBindVertexArray(debug_vao);
    glBindBuffer(GL_ARRAY_BUFFER, debug_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, 4 * sizeof(Vec3), debug_vertices + offset);
    debug_lines++;
}

void RenderSystem::print_gl_info() const {
    cout << "[RenderSystem][Notification/Other] GL Vendor: "   << glGetString(GL_VENDOR)   << endl;
    cout << "[RenderSystem][Notification/Other] GL Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "[RenderSystem][Notification/Other] GL Version: "  << glGetString(GL_VERSION)  << endl;
}

}
