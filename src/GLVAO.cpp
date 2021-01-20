#include "GLVAO.hpp"

#include "Vector.hpp"

namespace method {

GLVAO::GLVAO(const MeshData * mesh)
    : m_num_vbos(0)
    , m_vao_id(0) {
    // TODO: no respect for mesh sections

    // Create VAO
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);

    // VBO data
    vbo_data<Vec3>(mesh->get_vertices());
    vbo_data<Vec2>(mesh->get_uvs());
    vbo_data<Vec3>(mesh->get_normals());

    // Element buffer object
    unsigned int ib;
    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->get_size() * sizeof(IVec3),
        mesh->get_indices()->data(), GL_STATIC_DRAW);

    // TODO: useless
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint GLVAO::get_id() const {
    return m_vao_id;
}

}
