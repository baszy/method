#include "GlDebugCallback.hpp"

#include <iostream>
#include <string>

using namespace std;

namespace method {

void GLAPIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar * message,
    const void * userParam) {

    string msg_severity = "Unknown severity";
    switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            msg_severity = "Notification";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            msg_severity = "Low";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            msg_severity = "Medium";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            msg_severity = "High";
            break;
    }

    string msg_type = "Unknown type";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            msg_type = "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            msg_type = "Deprecated";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            msg_type = "UB";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            msg_type = "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            msg_type = "Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            msg_type = "Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            msg_type = "Push group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            msg_type = "Pop group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            msg_type = "Other";
            break;
    }

    printf("[GL][%s/%s] %s\n", msg_severity.c_str(), msg_type.c_str(), message);
}

}
