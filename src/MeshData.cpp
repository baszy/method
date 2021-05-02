#include "MeshData.hpp"

#include <cstring>

#include "Matrix.hpp"

namespace method {

MeshData::MeshData(unsigned int vertex_count, unsigned int face_count)
    : num_vertices(vertex_count)
    , num_faces(face_count) {

    // TODO: We could just make this one contiguous allocation
    vertices = new Vec3[num_vertices];
    uvs = new Vec2[num_vertices];
    bases = new Basis[num_vertices];

    indices = new IVec3[num_faces];
}

MeshData::~MeshData() {
    delete [] vertices;
    delete [] uvs;
    delete [] bases;

    delete [] indices;
}

// TODO: We just trust that the caller will provide an array of the same
// length that they specified earlier
void MeshData::set_vertices(Vec3 data[]) {
    memcpy(vertices, data, num_vertices * sizeof(Vec3));
}

void MeshData::set_uvs(Vec2 data[]) {
    memcpy(uvs, data, num_vertices * sizeof(Vec2));
}

void MeshData::set_bases(Basis data[]) {
    memcpy(bases, data, num_vertices * sizeof(Basis));
}

void MeshData::set_indices(IVec3 data[]) {
    memcpy(indices, data, num_faces * sizeof(IVec3));
}

void regenerate_basis(MeshData * mesh) {
    for (int i = 0; i < mesh->num_faces; i++) {
        Vec3 vertices[3] = {mesh->vertices[mesh->indices[i].x],
                            mesh->vertices[mesh->indices[i].y],
                            mesh->vertices[mesh->indices[i].z]};
        Vec2 uvs[3] = {mesh->uvs[mesh->indices[i].x],
                       mesh->uvs[mesh->indices[i].y],
                       mesh->uvs[mesh->indices[i].z]};

        Vec3 pos_edge_1 = vertices[1] - vertices[0];
        Vec3 pos_edge_2 = vertices[2] - vertices[0];
        Vec2 uv_edge_1 = uvs[1] - uvs[0];
        Vec2 uv_edge_2 = uvs[2] - uvs[0];

        float duv_elements[2][2] = {{uv_edge_1.x, uv_edge_2.x},
                                    {uv_edge_1.y, uv_edge_2.y}};
        Mat2 duv = invert(Mat2(duv_elements));

        Vec3 tangent = Vec3(duv.m[0][0] * pos_edge_1.x + duv.m[0][1] * pos_edge_2.x,
                            duv.m[0][0] * pos_edge_1.y + duv.m[0][1] * pos_edge_2.y,
                            duv.m[0][0] * pos_edge_1.z + duv.m[0][1] * pos_edge_2.z);
        Vec3 bitangent = Vec3(duv.m[1][0] * pos_edge_1.x + duv.m[1][1] * pos_edge_2.x,
                              duv.m[1][0] * pos_edge_1.y + duv.m[1][1] * pos_edge_2.y,
                              duv.m[1][0] * pos_edge_1.z + duv.m[1][1] * pos_edge_2.z);
        Vec3 normal = normalize(cross(pos_edge_1, pos_edge_2));

        // Affects all constituent vertices
        mesh->bases[mesh->indices[i].x].tangent += tangent;
        mesh->bases[mesh->indices[i].x].bitangent += bitangent;
        mesh->bases[mesh->indices[i].x].normal += normal;

        mesh->bases[mesh->indices[i].y].tangent += tangent;
        mesh->bases[mesh->indices[i].y].bitangent += bitangent;
        mesh->bases[mesh->indices[i].y].normal += normal;

        mesh->bases[mesh->indices[i].z].tangent += tangent;
        mesh->bases[mesh->indices[i].z].bitangent += bitangent;
        mesh->bases[mesh->indices[i].z].normal += normal;
    }
}

}
