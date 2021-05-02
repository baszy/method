#pragma once

#include "Vector.hpp"

namespace method {

struct Basis {
    // TODO: The normal could be omitted and recalculated on the gpu to save
    // space. Omitting tangent or bitangent may cause problems with shear
    Vec3 tangent;
    Vec3 bitangent;
    Vec3 normal;
};

class MeshData {
public:
    unsigned int num_vertices;
    Vec3 * vertices;
    Vec2 * uvs;
    Basis * bases;

    unsigned int num_faces;
    IVec3 * indices;

    MeshData(unsigned int vertex_count, unsigned int face_count);
    ~MeshData();

    void set_vertices(Vec3 data[]);
    void set_uvs(Vec2 data[]);
    void set_bases(Basis data[]);

    void set_indices(IVec3 data[]);
};

/**
 * \brief Recalculates the orthonormal basis vectors of the tangent space
 *        (i.e. tangent, bitangent, and normal vectors) for each vertex.
 *
 * \param mesh The mesh to be modified.
 */
void regenerate_basis(MeshData * mesh);

}
