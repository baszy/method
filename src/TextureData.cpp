#include "TextureData.hpp"

#include <cstring>
#include <iostream>

using namespace std;

namespace method {

TextureData::~TextureData() {
    delete [] data;
}

void TextureData::flip() {
    const U32 bytes_per_row = width * (bpp / 8);
    U8 * row1 = data + bytes_per_row * 0;
    U8 * row2 = data + bytes_per_row * (height - 1);

    U8 * temp = new U8 [bytes_per_row];

    for (U32 i = 0; i < height / 2; i++) {
        memcpy(temp, row1, bytes_per_row);
        memcpy(row1, row2, bytes_per_row);
        memcpy(row2, temp, bytes_per_row);

        row1 += bytes_per_row;
        row2 -= bytes_per_row;
    }

    delete [] temp;
}

}
