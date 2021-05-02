#include "TGALoader.hpp"

#include <cstring>

#include "MappedFile.hpp"

namespace method {

TextureData * load_tga(const std::string & path) {
    MappedFile file(path, MappedFileMode::READ_ONLY);

    TextureData * result = new TextureData();

    int counter = 0;

    // TODO: lambdas are slow I think especially because we are calling them
    // a trillion times
    auto next_char = [&]() {
        char result = *(reinterpret_cast<const char *>(file.data + counter));
        counter += 1;
        return result;
    };
    auto next_short = [&]() {
        short result = *(reinterpret_cast<const short *>(file.data + counter));
        counter += 2;
        return result;
    };
    auto next_int = [&]() {
        int result = *(reinterpret_cast<const int *>(file.data + counter));
        counter += 4;
        return result;
    };
    auto next_char_array = [&](unsigned int length) {
        char * result = new char[length];
        memcpy(result, file.data + counter, length);
        counter += length;
        return result;
    };

    int id_length        = next_char();
    counter += 4;
    int color_map_length = next_short();
    counter += 5;
    result->width        = next_short();
    result->height       = next_short();
    result->bpp          = next_char();
    counter += 1;
    unsigned int bytes = result->width * result->height * (result->bpp / 8);
    counter += id_length + color_map_length;
    result->image_data = next_char_array(bytes);

    counter = file.data_length - 26;
    counter += 8;
    const char * signature = next_char_array(18);

    if (strcmp("TRUEVISION-XFILE.", signature) != 0) {
        std::printf("%s has an incorrect TGA signature\n", path.c_str());
    }

    // TODO: hack stops non-diffuse maps from being broken by SRGB correction
    if (path.find("diffuse") != std::string::npos) result->srgb = true;

    return result;
}


}
