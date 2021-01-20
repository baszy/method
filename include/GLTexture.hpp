#pragma once

#include "TGAImage.hpp"

namespace method {

class GLTexture {
public:
    unsigned int id;

    GLTexture(TGAImage * i);
    ~GLTexture();
};

}
