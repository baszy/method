#include "GLShader.hpp"

#include <iostream>
#include <stdexcept>

#include "ReadFile.hpp"

namespace method {

const GLenum GLShader::s_shader_map[ShaderType::NUM_TYPES] = {
    GL_VERTEX_SHADER,
    GL_TESS_CONTROL_SHADER,
    GL_TESS_EVALUATION_SHADER,
    GL_GEOMETRY_SHADER,
    GL_FRAGMENT_SHADER
};

void GLShader::copy_binary_to_this(const GLShader & other) {
    glDeleteShader(m_id);
    m_id = glCreateProgram();

    int binary_length;
    glGetProgramiv(other.m_id, GL_PROGRAM_BINARY_LENGTH, &binary_length);

    int bytes_read;
    unsigned int format;
    char data[binary_length];

    // TODO: errors handled by debug callback
    glGetProgramBinary(other.m_id, binary_length, &bytes_read, &format, data);
    glProgramBinary(m_id, format, data, bytes_read);
}

GLShader::GLShader() {
    for (int i = 0; i < ShaderType::NUM_TYPES; i++) {
        m_sources[i] = nullptr;
    }
}

GLShader::GLShader(const GLShader & other) {
    copy_binary_to_this(other);
}

GLShader::~GLShader() {
    for (int i = 0; i < ShaderType::NUM_TYPES; i++) {
        delete m_sources[i];
    }
    glDeleteProgram(m_id);
}

GLShader & GLShader::operator = (const GLShader & other) {
    if (this != &other)
        copy_binary_to_this(other);

    return * this;
}

void GLShader::compile() {
    auto compile_shader = [](unsigned int shader, const char * const source) {
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        GLint success = 1;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            GLint size;
            std::string result;

            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
            result.resize(size);
            glGetShaderInfoLog(shader, size, nullptr, &result[0]);

            // TODO: instead of throwing exception, deal with error locally
            throw std::runtime_error("Shader failed to compile:\n" + result);
        }
    };

    GLuint shaders[ShaderType::NUM_TYPES];

    for (int i = 0; i < ShaderType::NUM_TYPES; i++) {
        if (m_sources[i] == nullptr)
            continue;

        shaders[i] = glCreateShader(s_shader_map[i]);
        compile_shader(shaders[i], m_sources[i]->data());
    }

    m_id = glCreateProgram();
    for (int i = 0; i < ShaderType::NUM_TYPES; i++)
        glAttachShader(m_id, shaders[i]);
    glLinkProgram(m_id);

    GLint success = 1;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        int size;
        std::string result;

        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &size);
        result.resize(size);
        glGetShaderInfoLog(m_id, size, nullptr, &result[0]);

        // TODO: instead of throwing exception, deal with error locally
        throw std::runtime_error("Shader failed to link:\n" + result);
    }

    // Delete shader objects
    for (int i = 0; i < ShaderType::NUM_TYPES; i++) {
        if (shaders[i])
            glDeleteShader(shaders[i]);
    }
}

void GLShader::load(std::string path, ShaderType type) {
    m_paths[type] = path;

    delete m_sources[type];

    // TODO: deal with allocation error
    // TODO: pointless copy
    m_sources[type] = new std::string(ReadFile(path));
}

void GLShader::set_uniform(const std::string & location, int value) {
    unsigned int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform1i(uniform, value);
}

void GLShader::set_uniform(const std::string & location, float value) {
    unsigned int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform1f(uniform, value);
}

void GLShader::set_uniform(const std::string & location, const Vec2 & value) {
    unsigned int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform2f(uniform, value.x, value.y);
}

void GLShader::set_uniform(const std::string & location, const Vec3 & value) {
    unsigned int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform3f(uniform, value.x, value.y, value.z);
}

void GLShader::set_uniform(const std::string & location, const Vec4 & value) {
    unsigned int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform4f(uniform, value.x, value.y, value.z, value.w);
}

void GLShader::set_uniform(const std::string & location, const mat4 & value) {
    unsigned int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniformMatrix4fv(uniform, 1, GL_FALSE, value.get_data());
}

void GLShader::use() const {
    glUseProgram(m_id);
}

}
