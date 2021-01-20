#include "Prop.hpp"

namespace method {

Prop::Prop(MeshData * data, Material * material)
    : m_material(material) 
    , m_data(data)
    , m_vao(data) {}

MeshData * Prop::get_data() const {
    return m_data;
}

Material * Prop::get_material() const {
    return m_material;
}

const GLVAO * Prop::get_vao() const {
    return &m_vao;
}

}
