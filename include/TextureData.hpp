#pragma once

#include "Vector.hpp"

namespace method {

struct TextureData {
    unsigned int width, height;
    unsigned int bpp;
    bool srgb;

    char * image_data;
};

}
