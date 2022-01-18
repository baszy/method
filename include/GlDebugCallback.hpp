#pragma once

#include "glad/glad.h"

namespace method {

void GLAPIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar * message,
    const void * userParam);

}
