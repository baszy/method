#include "GLTexture.hpp"

#include "glad/glad.h"

namespace method {

// TODO: Only works with truecolor TGAImages
GLTexture::GLTexture(TGAImage * i) {
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch (i->bpp) {
    case 32:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, i->width, i->height, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, i->image_data);
        break;
    case 24:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i->width, i->height, 0,
            GL_BGR, GL_UNSIGNED_BYTE, i->image_data);
        break;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &this->id);
}

}
