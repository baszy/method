#include "MeshData.hpp"

#include <cstring>

#include "Matrix.hpp"

namespace method {

Basis::Basis()
    : tangent(0),
      bitangent(0),
      normal(0) {}

Basis::Basis(Vec3 normal)
    : tangent(0),
      bitangent(0),
      normal(normal) {}

Basis::Basis(Vec3 tangent, Vec3 bitangent, Vec3 normal)
    : tangent(tangent),
      bitangent(bitangent),
      normal(normal) {}

MeshData::MeshData(unsigned int vertex_count, unsigned int face_count)
    : num_vertices(vertex_count),
      num_faces(face_count) {

    // TODO: We could just make this one contiguous allocation
    positions = new Vec3[num_vertices];
    uvs = new Vec2[num_vertices];
    bases = new Basis[num_vertices];

    faces = new IVec3[num_faces];
}

MeshData::~MeshData() {
    delete [] positions;
    delete [] uvs;
    delete [] bases;

    delete [] faces;
}

// TODO: We just trust that the caller will provide an array of the same
// length that they specified earlier
void MeshData::set_positions(Vec3 data[]) {
    memcpy(positions, data, num_vertices * sizeof(Vec3));
}

void MeshData::set_uvs(Vec2 data[]) {
    memcpy(uvs, data, num_vertices * sizeof(Vec2));
}

void MeshData::set_bases(Basis data[]) {
    memcpy(bases, data, num_vertices * sizeof(Basis));
}

void MeshData::set_faces(IVec3 data[]) {
    memcpy(faces, data, num_faces * sizeof(IVec3));
}

void MeshData::steal_positions(Vec3 data[]) {
    positions = data;
}

void MeshData::steal_uvs(Vec2 data[]) {
    uvs = data;
}

void MeshData::steal_bases(Basis data[]) {
    bases = data;
}

void MeshData::steal_faces(IVec3 data[]) {
    faces = data;
}

void MeshData::regenerate_basis(bool preserve_normals) {
    for (int i = 0; i < num_faces; i++) {
        Vec3 vertex[3] = {positions[faces[i].x],
                            positions[faces[i].y],
                            positions[faces[i].z]};
        Vec2 uv[3] = {uvs[faces[i].x],
                       uvs[faces[i].y],
                       uvs[faces[i].z]};

        Vec3 pos_edge_1 = vertex[1] - vertex[0];
        Vec3 pos_edge_2 = vertex[2] - vertex[0];
        Vec2 uv_edge_1 = uv[1] - uv[0];
        Vec2 uv_edge_2 = uv[2] - uv[0];

        float duv_elements[2][2] = {{uv_edge_1.x, uv_edge_2.x},
                                    {uv_edge_1.y, uv_edge_2.y}};
        Mat2 duv = invert(Mat2(duv_elements));

        Vec3 tangent = Vec3(duv.m[0][0] * pos_edge_1.x + duv.m[0][1] * pos_edge_2.x,
                            duv.m[0][0] * pos_edge_1.y + duv.m[0][1] * pos_edge_2.y,
                            duv.m[0][0] * pos_edge_1.z + duv.m[0][1] * pos_edge_2.z);
        Vec3 bitangent = Vec3(duv.m[1][0] * pos_edge_1.x + duv.m[1][1] * pos_edge_2.x,
                              duv.m[1][0] * pos_edge_1.y + duv.m[1][1] * pos_edge_2.y,
                              duv.m[1][0] * pos_edge_1.z + duv.m[1][1] * pos_edge_2.z);

        // Affects all constituent positions
        bases[faces[i].x].tangent += tangent;
        bases[faces[i].x].bitangent += bitangent;

        bases[faces[i].y].tangent += tangent;
        bases[faces[i].y].bitangent += bitangent;

        bases[faces[i].z].tangent += tangent;
        bases[faces[i].z].bitangent += bitangent;

        if (!preserve_normals) {
            Vec3 normal = normalize(cross(pos_edge_1, pos_edge_2));

            bases[faces[i].x].normal += normal;
            bases[faces[i].y].normal += normal;
            bases[faces[i].z].normal += normal;
        }
    }
}

}
