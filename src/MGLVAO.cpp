#include "MGLVAO.hpp"

#include "MVector.hpp"

namespace method {

MGLVAO::MGLVAO(const MMeshData * mesh)
    : m_num_vbos(0)
    , m_vao_id(0) {
    // TODO: no respect for mesh sections

    // Create VAO
    glGenVertexArrays(1, &m_vao_id);
    glBindVertexArray(m_vao_id);

    // VBO data
    vbo_data<vec3>(mesh->get_vertices());
    vbo_data<vec2>(mesh->get_uvs());
    vbo_data<vec3>(mesh->get_normals());

    // Element buffer object
    unsigned int ib;
    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->get_size() * sizeof(ivec3),
        mesh->get_indices()->data(), GL_STATIC_DRAW);

    // TODO: useless
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint MGLVAO::get_id() {
    return m_vao_id;
}

}
