#pragma once

#include <string>
#include <unordered_map>

#include "glad/glad.h"

#include "TextureData.hpp"

namespace method {

class Texture {
public:
    GLuint id;
    TextureData * data;
};

class TextureManager{
private:
    std::string base_path;
    std::unordered_map<std::string, Texture> textures;

public:
    TextureManager(const std::string & path);
    ~TextureManager();

    TextureManager(const TextureManager & other)               = delete;
    TextureManager(TextureManager && other)                    = delete;
    TextureManager & operator = (const TextureManager & other) = delete;
    TextureManager & operator = (TextureManager && other)      = delete;

    void reload(const std::string & handle);
    const Texture & get(const std::string & handle);
};

}
