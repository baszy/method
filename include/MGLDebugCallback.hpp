#pragma once

#include "glad/glad.h"

namespace method {

void GLAPIENTRY MGLDebugCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar * message,
    const void * userParam);

}