#include "RenderSystem.hpp"

#include <sstream>

#include "glad/glad.h"

#include "Camera.hpp"
#include "GLVAO.hpp"
#include "Vector.hpp"

#include "GLTexture.hpp"

namespace method {

RenderSystem::RenderSystem(RenderTarget * target)
    : m_target(target) {

    IVec2 size = target->get_viewport();
    glViewport(0, 0, size.x, size.y);

    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_PROGRAM_POINT_SIZE);

    // glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}

RenderSystem::~RenderSystem() {}

void RenderSystem::begin() {
    Vec4 sky_color(0.7f, 0.8f, 0.95f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(sky_color.x, sky_color.y, sky_color.z, sky_color.w);
}

void RenderSystem::draw(const Scene & scene) {
    for (Prop * prop : scene.props) {
        Material * mat = prop->get_material();
        GLShader * sh = mat->shader;
        sh->use();

        for (Camera * camera : scene.cameras) {
            sh->set_uniform("projection", camera->get_projection_matrix());
            sh->set_uniform("view", camera->get_view_matrix());
            sh->set_uniform("camera_position", camera->get_position());
        }
    
        for (Light * light : scene.lights) {
            sh->set_uniform("light_position", light->position);
            sh->set_uniform("light_color", light->color);
        }

        sh->set_uniform("model", prop->get_model_matrix());
        sh->set_uniform("object_color", mat->diffuse_color);
        sh->set_uniform("specular_exponent", mat->specular_exponent);

        if (mat->diffuse_map) {
            sh->set_uniform("diffuse_texture", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mat->diffuse_map->id);
        }
        
        if (mat->normal_map) {
            sh->set_uniform("normal_texture", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mat->normal_map->id);
        }

        if (mat->specular_map) {
            sh->set_uniform("specular_texture", 2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, mat->specular_map->id);
        }

        glBindVertexArray(prop->get_vao()->get_id());
        glDrawElements(GL_TRIANGLES, prop->get_data()->get_indices()->size()
            * 3, GL_UNSIGNED_INT, 0);
    }
}


void RenderSystem::end() {
    m_target->swap();
}

std::string RenderSystem::get_info() const {
    std::stringstream ss;
    ss << "OpenGL vendor: " << glGetString(GL_VENDOR) <<
        " \nOpenGL context version: " << glGetString(GL_VERSION) <<
        " \nOpenGL renderer: " << glGetString(GL_RENDERER) <<
        " \nGLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
    return ss.str();
}

RenderTarget * RenderSystem::get_render_target() {
    return m_target;
}

}
