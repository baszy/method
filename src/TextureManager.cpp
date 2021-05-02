#include "TextureManager.hpp"

#include "TGALoader.hpp"
#include "Vector.hpp"

namespace method {

TextureManager::TextureManager(const std::string & path)
    : path_base(path) {}

void TextureManager::reload(HotloaderIndex handle) {
    TextureData * data = load_tga(path_base + source_of(handle));

    GLTexture * tex = new GLTexture();

    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    // TODO: This has to be changed for diffuse and normal maps

    switch (data->bpp) {
        case 32:
        glTexImage2D(GL_TEXTURE_2D, 0, data->srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8,
            data->width, data->height, 0, GL_BGRA, GL_UNSIGNED_BYTE,
            data->image_data);
        break;
    case 24:
        glTexImage2D(GL_TEXTURE_2D, 0, data->srgb ? GL_SRGB8 : GL_RGB8,
            data->width, data->height, 0, GL_BGR, GL_UNSIGNED_BYTE,
            data->image_data);
        break;
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    texture_datas.push_back(data);
    texture_objects.push_back(tex);
}

TextureManager::~TextureManager() {
    for (int i = 0; i < texture_datas.size(); i++) {
        delete texture_datas[i];
    }
    for (int i = 0; i < texture_objects.size(); i++) {
        delete texture_objects[i];
    }
}

const GLTexture * TextureManager::get(HotloaderIndex handle) {
    if (!is_loaded(handle)) reload(handle);
    return texture_objects.at(handle);
}

bool TextureManager::is_loaded(HotloaderIndex handle) const {
    return handle < texture_datas.size()
        && handle < texture_objects.size();
}

}
