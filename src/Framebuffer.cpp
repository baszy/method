#include "Framebuffer.hpp"

namespace method {

Framebuffer::Framebuffer(const IVec2 & dimensions, FbType type)
    : size(dimensions) {

    glGenFramebuffers(1, &fbo_id);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

    switch (type) {
    case FbType::NORMAL:
        glGenTextures(1, &color_buffer_id);
        glBindTexture(GL_TEXTURE_2D, color_buffer_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, size.x, size.y, 0,
            GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            color_buffer_id, 0);

        glGenRenderbuffers(1, &rbo_id);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_RENDERBUFFER, rbo_id);

        break;

    case FbType::DEPTH:
        glGenTextures(1, &depth_buffer_id);
        glBindTexture(GL_TEXTURE_2D, depth_buffer_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        static float border[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // For comparison sampling:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

        // TODO: GL_FLOAT?
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, 0,
            GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
            depth_buffer_id, 0);

        glReadBuffer(GL_NONE);
        glDrawBuffer(GL_NONE);

        break;

    case FbType::CUBEMAP:
        glGenTextures(1, &color_buffer_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, color_buffer_id);
        
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        for (int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB,
                size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        }

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_buffer_id, 0);

        break;
    }
}

void Framebuffer::use() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
    glViewport(0, 0, size.x, size.y);
}

}
