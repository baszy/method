#include "GLDebugCallback.hpp"

#include <iostream>
#include <string>

namespace method {

void GLAPIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar * message,
    const void * userParam) {

    std::string str_severity = "Unknown severity";
    switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            str_severity = "GL_DEBUG_SEVERITY_NOTIFICATION";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            str_severity = "GL_DEBUG_SEVERITY_LOW";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            str_severity = "GL_DEBUG_SEVERITY_MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            str_severity = "GL_DEBUG_SEVERITY_HIGH";
            break;
    }

    std::string str_type = "Unknown type";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            str_type = "GL_DEBUG_TYPE_ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            str_type = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            str_type = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            str_type = "GL_DEBUG_TYPE_PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            str_type = "GL_DEBUG_TYPE_PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_MARKER:
            str_type = "GL_DEBUG_TYPE_MARKER";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            str_type = "GL_DEBUG_TYPE_PUSH_GROUP";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            str_type = "GL_DEBUG_TYPE_POP_GROUP";
            break;
        case GL_DEBUG_TYPE_OTHER:
            str_type = "GL_DEBUG_TYPE_OTHER";
            break;
    }

    // TOOD: use std::fprintf()
    std::printf("[OPENGL][%s][%s] %s\n", str_type.c_str(),
        str_severity.c_str(), message);
}

}
