#pragma once

#include <string>

#include "glad/glad.h"

#include "MMatrix.hpp"
#include "MShader.hpp"
#include "MVector.hpp"

namespace method {

class MGLShader : public MShader {
private:
    static const GLenum s_shader_map[METHOD_SHADER_NUM_TYPES];

    GLuint m_id;

    void copy_binary_to_this(const MGLShader & other);

public:
    MGLShader();
    MGLShader(const MGLShader & other);
    ~MGLShader();

    MGLShader & operator = (const MGLShader & other);

    void compile() override;
    void set_uniform(const std::string & location, const vec2 & value) override;
    void set_uniform(const std::string & location, const vec3 & value) override;
    void set_uniform(const std::string & location, const vec4 & value) override;
    void set_uniform(const std::string & location, const mat4 & value) override;
    void use() const override;

};

}