#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"

#include "Hotloader.hpp"
#include "TextureData.hpp"

namespace method {

struct GLTexture {
    unsigned int id;
};

class TextureManager : public Hotloader {
private:
    std::vector<TextureData *> texture_datas;
    std::vector<GLTexture *> texture_objects;

    std::string path_base;

    void reload(HotloaderIndex handle);

public:
    TextureManager(const std::string & path);
    ~TextureManager();

    const GLTexture * get(HotloaderIndex handle);
    bool is_loaded(HotloaderIndex handle) const;
};

}
