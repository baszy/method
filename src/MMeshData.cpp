#include "MMeshData.hpp"

namespace method {

MMeshSection::MMeshSection(unsigned int start, unsigned int size)
    : m_start(start)
    , m_size(size) {}

MMeshData::MMeshData() {}

MMeshData::MMeshData(const std::vector<vec3> & vertices,
    const std::vector<vec2> & uvs, const std::vector<vec3> & normals,
    const std::vector<ivec3> & indices)
    : m_size(indices.size())
    , m_vertices(vertices)
    , m_uvs(uvs)
    , m_normals(normals)
    , m_indices(indices) {

    m_sections.emplace_back(0, m_indices.size());
}

void MMeshData::add_section(const std::vector<vec3> & vertices,
    const std::vector<vec2> & uvs, const std::vector<vec3> & normals,
    const std::vector<ivec3> & indices) {

    unsigned int start = m_indices.size();
    unsigned int size = indices.size();

    // TODO: vectors of different lengths

    append_vector(&m_vertices, vertices);
    append_vector(&m_uvs, uvs);
    append_vector(&m_normals, normals);
    append_vector(&m_indices, indices);

    // May be uneccessary depending on how GL wants its IBOs
    // for (unsigned int i = start; i < start + size; i++) {
    //     m_indices[i].x += start;
    //     m_indices[i].y += start;
    //     m_indices[i].z += start;
    // }

    m_size += size;
    m_sections.emplace_back(start, size);
}

unsigned int MMeshData::get_size() const {
    return m_size;
}

const std::vector<vec3> * MMeshData::get_vertices() const {
    return &m_vertices;
}

const std::vector<vec2> * MMeshData::get_uvs() const {
    return &m_uvs;
}

const std::vector<vec3> * MMeshData::get_normals() const {
    return &m_normals;
}

const std::vector<ivec3> * MMeshData::get_indices() const {
    return &m_indices;
}

const std::vector<MMeshSection> * MMeshData::get_sections() const {
    return &m_sections;
}

}
