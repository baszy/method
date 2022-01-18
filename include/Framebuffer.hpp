#pragma once

#include "glad/glad.h"

#include "Vector.hpp"

namespace method {

enum FbType {
    NORMAL,
    DEPTH,
    CUBEMAP
};

class Framebuffer {
public:
    GLuint fbo_id;
    GLuint color_buffer_id;
    GLuint depth_buffer_id;
    GLuint rbo_id;

    IVec2 size;

    Framebuffer(const IVec2 & dimensions, FbType type);

    void use();
};

}
