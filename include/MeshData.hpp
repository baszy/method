#pragma once

#include "Vector.hpp"

namespace method {

class Basis {
public:
    // TODO: The normal could be omitted and recalculated on the gpu to save
    // space. Omitting tangent or bitangent may cause problems with shear.
    Vec3 tangent;
    Vec3 bitangent;
    Vec3 normal;

    Basis(); // Leaves everything as 0.
    Basis(Vec3 normal); // Leaves tangent and bitangent as 0.
    Basis(Vec3 tangent, Vec3 bitangent, Vec3 normal);
};

class MeshData {
public:
    unsigned int num_vertices;
    Vec3 * positions;
    Vec2 * uvs;
    Basis * bases;

    unsigned int num_faces;
    IVec3 * faces;

    MeshData(unsigned int vertex_count, unsigned int face_count);
    ~MeshData();

    void set_positions(Vec3 data[]);
    void set_uvs(Vec2 data[]);
    void set_bases(Basis data[]);

    void set_faces(IVec3 data[]);

    void steal_positions(Vec3 data[]);
    void steal_uvs(Vec2 data[]);
    void steal_bases(Basis data[]);

    void steal_faces(IVec3 data[]);

    /**
     * \brief Recalculates the orthonormal basis vectors of the tangent space
     *        (i.e. tangent, bitangent, and normal vectors) for each vertex.
     *
     * \param preserve_normals Maintains the original normals of the model
     */
    void regenerate_basis(bool preserve_normals);
};

}
