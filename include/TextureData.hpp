#pragma once

#include "Defs.hpp"

namespace method {

class TextureData {
public:
    U32 width, height;
    U32 bpp;

    bool srgb;
    bool rgb;

    U8 * data;

    ~TextureData();

    /**
     * \brief Vertically flips the image data.
     */
    void flip();
};

}
