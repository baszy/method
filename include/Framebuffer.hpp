#pragma once

#include "glad/glad.h"

#include "Vector.hpp"

namespace method {

class Framebuffer {
public:
    bool cubemap;

    GLuint fbo_id;
    GLuint color_buffer_id;
    GLuint rbo_id;

    IVec2 size;

    Framebuffer(const IVec2 & dimensions, bool is_cubemap);
};

}
