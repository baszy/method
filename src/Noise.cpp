#include "Noise.hpp"

#include "TextureData.hpp"

#include <cstdlib>

namespace method {

Texture white_noise(int width, int height, int band) {
    Texture texture;

    // TODO: Not freed
    TextureData * data = new TextureData();
    data->width = width;
    data->height = height;
    data->bpp = 24;
    data->srgb = false;
    data->rgb = true;
    size_t bytes = width * height * 3;
    data->data = new U8[bytes];

    texture.data = data;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    for (size_t i = 0; i < bytes; i++) {
        data->data[i] = rand() % (256 - band) + band;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, data->data);

    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}


}
