#include "TgaLoader.hpp"

#include <cstring>
#include <iostream>

#include "Console.hpp"

using namespace std;

namespace method {

TextureData * load_tga(const File & file) {
    enum TgaData {
        TGA_DATA_NONE = 0,
        TGA_DATA_CMAP = 1,
        TGA_DATA_RGB  = 2,
        TGA_DATA_BW   = 3,
        TGA_DATA_CMAP_RLE = 9,
        TGA_DATA_RGB_RLE  = 10,
        TGA_DATA_BW_RLE   = 11
    };

    struct TgaHeader {
        U8  id_length;
        U8  cmap_type;
        U8  data_type;
        U16 cmap_origin;
        U16 cmap_length;
        U8  cmap_bpp;
        U16 x;
        U16 y;
        U16 width;
        U16 height;
        U8  bpp;
        U8  image_desc;
    } __attribute__ ((packed));

    const char * TGA_SIGNATURE = "TRUEVISION-XFILE.";
    const char * file_signature = (char *)file.get_data() + file.get_size() - 26 + 8;
    if (strncmp(TGA_SIGNATURE, file_signature, 17) != 0)
      log << file.get_path() << " has an incorrect TGA signature" << newl;

    TgaHeader header;
    memcpy(&header, file.get_data(), sizeof(header));

    switch (header.data_type) {
    case TGA_DATA_NONE:
        break;
    case TGA_DATA_CMAP:
        break;
    case TGA_DATA_RGB:
        break;
    case TGA_DATA_BW:
        break;
    case TGA_DATA_CMAP_RLE:
        break;
    case TGA_DATA_RGB_RLE:
        break;
    case TGA_DATA_BW_RLE:
        break;
    default:
        break;
    }

    U64 bytes = header.width * header.height * (header.bpp / 8);
    const void * image_data = file.get_data() + 18 + header.id_length + header.cmap_length;
    U8 * data = new U8[bytes];
    memcpy(data, image_data, bytes);

    TextureData * result = new TextureData();

    result->width = header.width;
    result->height = header.height;
    result->bpp = header.bpp;
    result->data = data;
    result->rgb = false;
    // TODO: hack stops non-diffuse maps from being broken by SRGB correction
    if (file.get_path().find("diffuse") != string::npos) result->srgb = true;

    return result;
}

}
