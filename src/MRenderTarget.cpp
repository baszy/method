#include "MRenderTarget.hpp"

namespace method {

MRenderTarget::MRenderTarget()
    : m_dimensions(ivec2(640, 480)) {}

MRenderTarget::MRenderTarget(const ivec2 & dimensions)
    : m_dimensions(dimensions) {}

MRenderTarget::~MRenderTarget() {}

ivec2 MRenderTarget::get_dimensions() const {
    return this->m_dimensions;
}

void MRenderTarget::swap() {}

void MRenderTarget::use() {}

}