#pragma once

#include <string>

#include "glad/glad.h"

#include "Matrix.hpp"
#include "Vector.hpp"

#define METHOD_GLSL(str) (const char*)"#version 450 core\n" #str

namespace method {

enum ShaderType {
    VERTEX,
    TESS_CONTROL,
    TESS_EVAL,
    GEOMETRY,
    FRAGMENT,
    NUM_TYPES
};

class Shader {
private:
    GLuint m_id;
    std::string m_paths[ShaderType::NUM_TYPES];
    std::string * m_sources[ShaderType::NUM_TYPES];

    static const GLenum s_shader_map[ShaderType::NUM_TYPES];

    void copy_binary_to_this(const Shader & other);

public:
    Shader();
    Shader(const Shader & other);
    ~Shader();

    Shader & operator = (const Shader & other);

    void compile();
    void load(std::string path, ShaderType type);
    void set_source(const char * source, ShaderType type);
    void use() const;

    void set_uniform(const char * location, int value);
    void set_uniform(const char * location, float value);
    void set_uniform(const char * location, const Vec2 & value);
    void set_uniform(const char * location, const Vec3 & value);
    void set_uniform(const char * location, const Vec4 & value);
    void set_uniform(const char * location, const Mat4 & value);
    void set_uniform(const char * location,
                     const Vec3 values[], unsigned int values_length);
};

}
