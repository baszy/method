#include "MeshManager.hpp"

#include "OBJLoader.hpp"
#include "Vector.hpp"

#include <iostream>

namespace method {

MeshManager::MeshManager(const std::string & path)
    : path_base(path) {}

void MeshManager::reload(HotloaderIndex handle) {
    MeshData * data = load_obj(path_base + source_of(handle));
    regenerate_basis(data);

    GLVAO * vao = new GLVAO();

    // Create VAO
    glGenVertexArrays(1, &vao->vao_id);
    glBindVertexArray(vao->vao_id);

    GLuint vbo_ids[3];
    glGenBuffers(3, vbo_ids);
    unsigned int vbo_index = 0;

    // Vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[vbo_index]);
    glBufferData(GL_ARRAY_BUFFER, data->num_vertices * sizeof(Vec3),
        data->vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(vbo_index, sizeof(Vec3) / sizeof(float),
        GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(vbo_index);

    vbo_index++;

    // Vertex UVs
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[vbo_index]);
    glBufferData(GL_ARRAY_BUFFER, data->num_vertices * sizeof(Vec2),
        data->uvs, GL_STATIC_DRAW);

    glVertexAttribPointer(vbo_index, sizeof(Vec2) / sizeof(float),
        GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(vbo_index);

    vbo_index++;

    // Vertex basis vectors
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[vbo_index]);
    glBufferData(GL_ARRAY_BUFFER, data->num_vertices * sizeof(Basis),
        data->bases, GL_STATIC_DRAW);

    glVertexAttribPointer(vbo_index, sizeof(Vec3) / sizeof(float),
        GL_FLOAT, GL_FALSE, 3 * sizeof(Vec3), (void *)(0));
    glEnableVertexAttribArray(vbo_index);

    vbo_index++;

    glVertexAttribPointer(vbo_index, sizeof(Vec3) / sizeof(float),
        GL_FLOAT, GL_FALSE, 3 * sizeof(Vec3), (void *)(1 * sizeof(Vec3)));
    glEnableVertexAttribArray(vbo_index);

    vbo_index++;

    glVertexAttribPointer(vbo_index, sizeof(Vec3) / sizeof(float),
        GL_FLOAT, GL_FALSE, 3 * sizeof(Vec3), (void *)(2 * sizeof(Vec3)));
    glEnableVertexAttribArray(vbo_index);

    vbo_index++;

    // Element buffer
    unsigned int ib;
    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->num_faces * sizeof(IVec3),
        data->indices, GL_STATIC_DRAW);

    // TODO: useless?
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    vao->num_vbos = vbo_index;
    vao->num_indices = data->num_faces * 3; // 3 vertices per face :)

    mesh_datas.push_back(data);
    mesh_vaos.push_back(vao);
}

MeshManager::~MeshManager() {
    for (int i = 0; i < mesh_datas.size(); i++) {
        delete mesh_datas[i];
    }
    for (int i = 0; i < mesh_vaos.size(); i++) {
        delete mesh_vaos[i];
    }
}

const GLVAO * MeshManager::get(HotloaderIndex handle) {
    if (!is_loaded(handle)) reload(handle);
    return mesh_vaos.at(handle);
}

bool MeshManager::is_loaded(HotloaderIndex handle) const {
    return handle < mesh_datas.size()
        && handle < mesh_vaos.size();
}

}
