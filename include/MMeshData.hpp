#pragma once

#include <vector>

#include "MVector.hpp"

namespace method {

class MMeshSection {
private:
    unsigned int m_start;
    unsigned int m_size;

public:
    MMeshSection(unsigned int start, unsigned int size);

};

class MMeshData {
private:
    unsigned int m_size;

    std::vector<vec3> m_vertices;
    std::vector<vec2> m_uvs;
    std::vector<vec3> m_normals;
    std::vector<ivec3> m_indices;

    std::vector<MMeshSection> m_sections;

    template <typename Type>
    inline void append_vector(std::vector<Type> * first,
        const std::vector<Type> & other) {

        first->reserve(first->size() + other.size());
        first->insert(first->end(), other.begin(), other.end());
    }

public:
    MMeshData();
    MMeshData(const std::vector<vec3> & vertices,
              const std::vector<vec2> & uvs,
              const std::vector<vec3> & normals,
              const std::vector<ivec3> & indices);

    void add_section(const std::vector<vec3> & vertices,
                     const std::vector<vec2> & uvs,
                     const std::vector<vec3> & normals,
                     const std::vector<ivec3> & indices);

    unsigned int get_size() const;

    const std::vector<vec3> * get_vertices() const;
    const std::vector<vec2> * get_uvs() const;
    const std::vector<vec3> * get_normals() const;
    const std::vector<ivec3> * get_indices() const;

    const std::vector<MMeshSection> * get_sections() const;

};

}
