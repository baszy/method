#include "MShader.hpp"

#include "MReadFile.hpp"

namespace method {

MShader::MShader() {
    for (int i = 0; i < METHOD_SHADER_NUM_TYPES; i++)
        m_sources[i] = nullptr;
}

MShader::~MShader() {
    for (int i = 0; i < METHOD_SHADER_NUM_TYPES; i++)
        delete m_sources[i];
}

void MShader::load(std::string path, MShaderType type) {
    m_paths[type] = path;

    delete m_sources[type];

    // TODO: deal with allocation error
    // TODO: pointless copy
    m_sources[type] = new std::string(MReadFile(path));
}

}