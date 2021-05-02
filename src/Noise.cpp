#include "Noise.hpp"

namespace method {

GLTexture * white_noise(int width, int height, int band) {
    GLTexture * tex = new GLTexture();

    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    size_t length = width * height * 3;
    char * data = new char[length];
    for (size_t i = 0; i < length; i++) {
        data[i] = rand() % (256 - band) + band; 
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    return tex;
}


}
