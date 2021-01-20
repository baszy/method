#include "TGAImage.hpp"

#include <cstring>

#include "MappedFile.hpp"

namespace method {

// TODO: quit it with the excessive pointers
TGAImage::TGAImage(const std::string & filename) {
    MappedFile file(filename, MappedFileMode::READ_ONLY);

    const char * data = file.data;
    unsigned int size = file.data_length;

    int counter = 0;

    // TODO: lambdas are slow i think especially because we are calling them
    // a trillion times
    auto next_char = [&](){
        char result = *(reinterpret_cast<const char *>(data + counter));
        counter += 1;
        return result;
    };
    auto next_short = [&](){
        short result = *(reinterpret_cast<const short *>(data + counter));
        counter += 2;
        return result;
    };
    auto next_int = [&](){
        int result = *(reinterpret_cast<const int *>(data + counter));
        counter += 4;
        return result;
    };
    auto next_char_array = [&](unsigned int length){
        char * result = new char [length];
        memcpy(result, data + counter, length);
        counter += length;
        return result;
    };

    // Anyway we read per-value because of alignment amits
    this->id_length            = next_char();
    this->color_map_type       = next_char();
    this->image_type           = next_char();
    this->color_map_start      = next_short();
    this->color_map_length     = next_short();
    this->color_map_bpp        = next_char();
    this->origin_x             = next_short();
    this->origin_y             = next_short();
    this->width                = next_short();
    this->height               = next_short();
    this->bpp                  = next_char();
    this->descriptor           = next_char();
    unsigned int image_length = this->width * this->height * (this->bpp / 8);
    this->id_data              = next_char_array(this->id_length);
    this->color_map_data       = next_char_array(this->color_map_length);
    this->image_data           = next_char_array(image_length);

    counter = size - 26;
    this->extensions_start = next_int();
    this->developer_start = next_int();
    this->signature = next_char_array(18);
 
    if (strcmp("TRUEVISION-XFILE.", this->signature) != 0) {
        std::printf("%s has an incorrect TGA signature\n", filename.c_str());
    }
}

TGAImage::~TGAImage() {
    delete [] this->id_data;
    delete [] this->color_map_data;
    delete [] this->image_data;
}

void TGAImage::print_info() {
    std::printf("Image ID size: %d bytes\n", this->id_length);
    char cmt = this->color_map_type;
    std::printf("Has colormap: %d (%s)\n", cmt, cmt ? "Yes" : "No");
    char it = this->image_type;
    std::printf("Image type: %d\n", it);
    std::printf("Image data %s\n", (it > 0) ? "present" : "not present");
    const char * types[4] = {"No data", "Colormapped", "Truecolor", "Grayscale"};
    std::printf("%s\n", types[it & 0b00000111]);
    std::printf("%s\n", (it & 0b00001000) ? "RLE" : "No RLE");
    std::printf("Colormap start index: %d\n", this->color_map_start); 
    std::printf("Colormap length: %d bytes\n", this->color_map_length); 
    std::printf("Colormap bits per pixel: %d\n", this->color_map_bpp); 
    std::printf("Image origin coordinates: (%d, %d)\n", this->origin_x, this->origin_y);
    std::printf("Image dimensions: %dx%d\n", this->width, this->height);
    std::printf("Image bits per pixel: %d\n", this->bpp);
    char desc = this->descriptor;
    std::printf("Alpha bits per pixel: %d\n", desc & 0b0001111);
    const char * origin[4] = {"Bottom left", "Bottom right", "Top left", "Top right"};
    std::printf("Image origin: %s\n", origin[(desc & 0b00110000) / 0b00010000]); 
    std::printf("Image extension area offset: %d\n", this->extensions_start);
    std::printf("Image developer area offset: %d\n", this->developer_start);
    std::printf("Image signature: %s\n", this->signature);
}

}
