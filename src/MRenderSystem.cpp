#include "MRenderSystem.hpp"

namespace method {

MRenderSystem::MRenderSystem(MRenderTarget * target)
    : m_target(target) {}

MRenderSystem::~MRenderSystem() {}

void MRenderSystem::begin(MShader * shader) {
    m_shader = shader;
    m_shader->use();
}

void MRenderSystem::draw(MScene * world) {}

void MRenderSystem::end() {
    m_target->swap();
}

std::string MRenderSystem::get_info() const {
    return METHOD_DEFAULT_RENDER_INFO;
}

MRenderTarget * MRenderSystem::get_render_target() {
    return m_target;
}

}