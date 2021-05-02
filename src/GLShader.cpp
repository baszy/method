#include "GLShader.hpp"

#include <iostream>

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

            // TODO: are we going to deal with this error?
            std::cout << "Shader failed to compile:" << std::endl << result;
        }
    };

    GLuint shaders[ShaderType::NUM_TYPES] = {0};

    for (int i = 0; i < ShaderType::NUM_TYPES; i++) {
        if (!m_sources[i])
            continue;

        shaders[i] = glCreateShader(s_shader_map[i]);
        compile_shader(shaders[i], m_sources[i]->data());
    }

    m_id = glCreateProgram();
    for (int i = 0; i < ShaderType::NUM_TYPES; i++) {
        if (shaders[i])
            glAttachShader(m_id, shaders[i]);
    }
    glLinkProgram(m_id);

    GLint success = 1;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        int size;
        std::string result;

        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &size);
        result.resize(size);
        glGetProgramInfoLog(m_id, size, nullptr, &result[0]);

        // TODO: are we going to deal with this error?
        std::cout << "Shader failed to link:" << std::endl << result;
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

    // TODO: pointless copy
    m_sources[type] = new std::string(ReadFile(path));
}

void GLShader::set_source(const char * source, ShaderType type) {
    m_paths[type] = "";

    delete m_sources[type];

    m_sources[type] = new std::string(source);
}

void GLShader::use() const {
    glUseProgram(m_id);
}

// TODO: cache the GetUniformLocations because transferring strings is slow
void GLShader::set_uniform(const std::string & location, int value) {
    int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform1i(uniform, value);
}

void GLShader::set_uniform(const std::string & location, float value) {
    int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform1f(uniform, value);
}

void GLShader::set_uniform(const std::string & location, const Vec2 & value) {
    int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform2f(uniform, value.x, value.y);
}

void GLShader::set_uniform(const std::string & location, const Vec3 & value) {
    int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform3f(uniform, value.x, value.y, value.z);
}

void GLShader::set_uniform(const std::string & location, const Vec4 & value) {
    int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform4f(uniform, value.x, value.y, value.z, value.w);
}

void GLShader::set_uniform(const std::string & location, const Mat4 & value) {
    int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniformMatrix4fv(uniform, 1, GL_FALSE, value.get_data());
}

void GLShader::set_uniform(const std::string & location,
                           const Vec3 values[], unsigned int len) {
    int uniform = glGetUniformLocation(m_id, location.c_str());
    glUniform3fv(uniform, len, (float *)values);
}

}
