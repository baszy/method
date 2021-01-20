#pragma once

#include <vector>

#include "Vector.hpp"

namespace method {

class MeshSection {
private:
    unsigned int m_start;
    unsigned int m_size;

public:
    MeshSection(unsigned int start, unsigned int size);

};

class MeshData {
private:
    unsigned int m_size;

    std::vector<Vec3> m_vertices;
    std::vector<Vec2> m_uvs;
    std::vector<Vec3> m_normals;
    std::vector<IVec3> m_indices;

    std::vector<MeshSection> m_sections;

    template <typename Type>
    inline void append_vector(std::vector<Type> * first,
        const std::vector<Type> & other) {

        first->reserve(first->size() + other.size());
        first->insert(first->end(), other.begin(), other.end());
    }

public:
    MeshData();
    MeshData(const std::vector<Vec3> & vertices,
              const std::vector<Vec2> & uvs,
              const std::vector<Vec3> & normals,
              const std::vector<IVec3> & indices);

    void add_section(const std::vector<Vec3> & vertices,
                     const std::vector<Vec2> & uvs,
                     const std::vector<Vec3> & normals,
                     const std::vector<IVec3> & indices);

    unsigned int get_size() const;

    const std::vector<Vec3> * get_vertices() const;
    const std::vector<Vec2> * get_uvs() const;
    const std::vector<Vec3> * get_normals() const;
    const std::vector<IVec3> * get_indices() const;

    const std::vector<MeshSection> * get_sections() const;

};

}
