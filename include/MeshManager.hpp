#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"

#include "Hotloader.hpp"
#include "MeshData.hpp"

namespace method {

struct GLVAO {
    GLuint vao_id;
    GLuint num_vbos;
    GLuint num_indices;
};

class MeshManager : public Hotloader {
private:
    std::vector<MeshData *> mesh_datas;
    std::vector<GLVAO *> mesh_vaos;

    std::string path_base;

    void reload(HotloaderIndex handle);

public:
    MeshManager(const std::string & path);
    ~MeshManager();

    const GLVAO * get(HotloaderIndex handle);
    bool is_loaded(HotloaderIndex handle) const;
};

}
