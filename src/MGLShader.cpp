#include "MGLShader.hpp"

#include <iostream>
#include <stdexcept>

namespace method {

const GLenum MGLShader::s_shader_map[METHOD_SHADER_NUM_TYPES] = {
    GL_VERTEX_SHADER,
    GL_TESS_CONTROL_SHADER,
    GL_TESS_EVALUATION_SHADER,
    GL_GEOMETRY_SHADER,
    GL_FRAGMENT_SHADER
};

void MGLShader::copy_binary_to_this(const MGLShader & other) {
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

MGLShader::MGLShader() {}

MGLShader::MGLShader(const MGLShader & other) {
    copy_binary_to_this(other);
}

MGLShader::~MGLShader() {
    glDeleteProgram(m_id);
}

MGLShader & MGLShader::operator = (const MGLShader & other) {
    if (this != &other)
        copy_binary_to_this(other);

    return * this;
}

void MGLShader::compile() {
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

    GLuint shaders[METHOD_SHADER_NUM_TYPES];

    for (int i = 0; i < METHOD_SHADER_NUM_TYPES; i++) {
        if (m_sources[i] == nullptr)
            continue;

        shaders[i] = glCreateShader(s_shader_map[i]);
        compile_shader(shaders[i], m_sources[i]->data());
    }

    m_id = glCreateProgram();
    for (int i = 0; i < METHOD_SHADER_NUM_TYPES; i++)
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
    for (int i = 0; i < METHOD_SHADER_NUM_TYPES; i++) {
        if (shaders[i])
            glDeleteShader(shaders[i]);
    }
}

void MGLShader::set_uniform(const std::string & location, const vec2 & value) {
    unsigned int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform2f(uniform, value.x, value.y);
}

void MGLShader::set_uniform(const std::string & location, const vec3 & value) {
    unsigned int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform3f(uniform, value.x, value.y, value.z);
}

void MGLShader::set_uniform(const std::string & location, const vec4 & value) {
    unsigned int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform4f(uniform, value.x, value.y, value.z, value.w);
}

void MGLShader::set_uniform(const std::string & location, const mat4 & value) {
    unsigned int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniformMatrix4fv(uniform, 1, GL_FALSE, value.get_data());
}

void MGLShader::use() const {
    glUseProgram(m_id);
}

}