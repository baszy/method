#include "TextureManager.hpp"

#include "MappedFile.hpp"
#include "PngLoader.hpp"
#include "TgaLoader.hpp"
#include "Vector.hpp"

#include <iostream>

using namespace std;

namespace method {

TextureManager::TextureManager(const string & path)
    : base_path(path) {}

TextureManager::~TextureManager() {
    for (auto p : textures) {
        delete p.second.data;
    }
}

void TextureManager::reload(const string & handle) {
    MappedFile file(base_path + handle, MappedFileMode::READ_ONLY);
    TextureData * data = load_tga(file);

    auto insertion = textures.emplace(handle, Texture());
    Texture & texture = insertion.first->second;

    texture.data = data;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);

    // TODO: This has to be changed for diffuse and normal maps

    switch (data->bpp) {
    case 32:
        glTexImage2D(GL_TEXTURE_2D, 0, data->srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8,
            data->width, data->height, 0, data->rgb ? GL_RGBA : GL_BGRA,
            GL_UNSIGNED_BYTE, data->data);
        break;
    case 24:
        glTexImage2D(GL_TEXTURE_2D, 0, data->srgb ? GL_SRGB8 : GL_RGB8,
            data->width, data->height, 0, data->rgb ? GL_RGB : GL_BGR,
            GL_UNSIGNED_BYTE, data->data);
        break;
    }

    glGenerateMipmap(GL_TEXTURE_2D);
}

const Texture & TextureManager::get(const string & handle) {
    if (!textures.count(handle))
        reload(handle);

    return textures.at(handle);
}

}
