#include "MGLRenderSystem.hpp"

#include <sstream>

#include "glad/glad.h"

#include "Component/MCamera.hpp"
#include "Component/MModel.hpp"
#include "Component/MTransformation.hpp"
#include "MGLWindow.hpp"
#include "MGLVAO.hpp"
#include "MVector.hpp"

namespace method {

MGLRenderSystem::MGLRenderSystem(MGLWindow * target)
    : MRenderSystem(target) {

    ivec2 size = target->get_viewport();
    glViewport(0, 0, size.x, size.y);

    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void MGLRenderSystem::begin(MShader * shader) {
    MRenderSystem::begin(shader);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MGLRenderSystem::draw(MScene * scene) {
    scene->for_each<MCamera, MTransformation>([&](std::shared_ptr<MCamera> c,
        std::shared_ptr<MTransformation> t) {

        m_shader->set_uniform("projection", c->get_projection());
        m_shader->set_uniform("view", t->get_model_matrix());
    });

    scene->for_each<MModel, MTransformation>([&](std::shared_ptr<MModel> m,
        std::shared_ptr<MTransformation> t) {

        m_shader->set_uniform("model", t->get_model_matrix());

        glBindVertexArray(m->get_vao()->get_id());
        glDrawElements(GL_TRIANGLES, m->get_data()->get_indices()->size() * 3,
            GL_UNSIGNED_INT, 0);
    });
}

std::string MGLRenderSystem::get_info() const {
    std::stringstream output;
    output << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl
           << "OpenGL context version: " << glGetString(GL_VERSION) << std::endl
           << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl
           << "[DEBUG] GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
    return output.str();
}

}
