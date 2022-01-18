#pragma once

#include <string>
#include <unordered_map>

#include "glad/glad.h"

#include "MeshData.hpp"

namespace method {

class Mesh {
public:
    GLuint vao;
    GLuint * vbo;
    GLuint vbo_count;
    GLuint ebo;
    MeshData * data;
};

class MeshManager {
private:
    std::string base_path;
    std::unordered_map<std::string, Mesh> meshes;

public:
    MeshManager(const std::string & base_path);
    ~MeshManager();

    MeshManager(const MeshManager & other)               = delete;
    MeshManager(MeshManager && other)                    = delete;
    MeshManager & operator = (const MeshManager & other) = delete;
    MeshManager & operator = (MeshManager && other)      = delete;

    void reload(std::string handle);
    const Mesh & get(const std::string & handle);
};

}
