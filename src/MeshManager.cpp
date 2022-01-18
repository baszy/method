#include "MeshManager.hpp"

#include "MappedFile.hpp"
#include "ObjLoader.hpp"
#include "Vector.hpp"

using namespace std;

namespace method {

MeshManager::MeshManager(const string & base_path)
    : base_path(base_path),
      meshes() {}

MeshManager::~MeshManager() {
    for (auto p : meshes) {
        delete [] p.second.vbo;
        delete    p.second.data;
    }
}

void MeshManager::reload(string handle) {
    MappedFile file(base_path + handle, MappedFileMode::READ_ONLY);
    MeshData * data = load_obj(file);
    data->regenerate_basis(true);

    auto insertion = meshes.emplace(handle, Mesh());
    Mesh & mesh = insertion.first->second;

    mesh.vbo_count = 3;
    mesh.vbo = new GLuint[3];
    mesh.data = data;

    // Create VAO
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);
    glGenBuffers(mesh.vbo_count, mesh.vbo);

    unsigned int cur_attrib = 0;

    // Vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, data->num_vertices * sizeof(Vec3),
        data->positions, GL_STATIC_DRAW);

    glVertexAttribPointer(cur_attrib, sizeof(Vec3) / sizeof(float),
        GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(cur_attrib);

    cur_attrib++;

    // Vertex UVs
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, data->num_vertices * sizeof(Vec2),
        data->uvs, GL_STATIC_DRAW);

    glVertexAttribPointer(cur_attrib, sizeof(Vec2) / sizeof(float),
        GL_FLOAT, GL_FALSE, 0, (void *)(0));
    glEnableVertexAttribArray(cur_attrib);

    cur_attrib++;

    // Vertex basis vectors
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, data->num_vertices * sizeof(Basis),
        data->bases, GL_STATIC_DRAW);

    glVertexAttribPointer(cur_attrib, sizeof(Vec3) / sizeof(float),
        GL_FLOAT, GL_FALSE, 3 * sizeof(Vec3), (void *)(0));
    glEnableVertexAttribArray(cur_attrib);

    cur_attrib++;

    glVertexAttribPointer(cur_attrib, sizeof(Vec3) / sizeof(float),
        GL_FLOAT, GL_FALSE, 3 * sizeof(Vec3), (void *)(1 * sizeof(Vec3)));
    glEnableVertexAttribArray(cur_attrib);

    cur_attrib++;

    glVertexAttribPointer(cur_attrib, sizeof(Vec3) / sizeof(float),
        GL_FLOAT, GL_FALSE, 3 * sizeof(Vec3), (void *)(2 * sizeof(Vec3)));
    glEnableVertexAttribArray(cur_attrib);

    cur_attrib++;

    // Element buffer
    glGenBuffers(1, &mesh.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->num_faces * sizeof(IVec3),
        data->faces, GL_STATIC_DRAW);
}

const Mesh & MeshManager::get(const string & handle) {
    if (!meshes.count(handle))
        reload(handle);

    return meshes.at(handle);
}

}
