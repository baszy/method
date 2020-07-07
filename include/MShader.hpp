#pragma once

#include <string>

#include "MMatrix.hpp"
#include "MVector.hpp"

namespace method {

enum MShaderType {
    METHOD_SHADER_VERTEX,
    METHOD_SHADER_TESS_CONTROL,
    METHOD_SHADER_TESS_EVAL,
    METHOD_SHADER_GEOMETRY,
    METHOD_SHADER_FRAGMENT,
    METHOD_SHADER_NUM_TYPES
};

class MShader {
protected:
    std::string m_paths[METHOD_SHADER_NUM_TYPES];
    std::string * m_sources[METHOD_SHADER_NUM_TYPES];

public:
    MShader();
    virtual ~MShader();

    virtual void compile() = 0;
    virtual void load(std::string path, MShaderType type);
    virtual void set_uniform(const std::string & location,
        const vec2 & value) = 0;
    virtual void set_uniform(const std::string & location,
        const vec3 & value) = 0;
    virtual void set_uniform(const std::string & location,
        const vec4 & value) = 0;
    virtual void set_uniform(const std::string & location,
        const mat4 & value) = 0;
    virtual void use() const = 0;

};

}