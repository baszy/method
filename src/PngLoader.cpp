#include "PngLoader.hpp"

#include <cstring>
#include <cmath>
#include <string>

#include <png.h>

#include "Console.hpp"
#include "Defs.hpp"
#include "MappedFile.hpp"
#include "Zlib.hpp"

using namespace std;

namespace method {

TextureData * load_png(const string & path) {
    FILE * file = fopen(path.c_str(), "rb");
    if (!file)
        log << "load_png(\"" << path << "\"): could not open file" << newl;

    size_t number = 8;
    U8 header[8] = {0};
    fread(header, 1, number, file);
    if (png_sig_cmp(header, 0, number))
        log << "load_png(\"" << path << "\"): not a valid png file" << newl;

    png_structp libpng = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!libpng)
        log << "load_png(\"" << path << "\"): could not initialize libpng" << newl;

    png_infop start_info = png_create_info_struct(libpng);
    if (!start_info) {
        png_destroy_read_struct(&libpng, nullptr, nullptr);
        log << "load_png(\"" << path << "\"): could not create start_info" << newl;
    }

    png_infop end_info = png_create_info_struct(libpng);
    if (!end_info) {
        png_destroy_read_struct(&libpng, &start_info, nullptr);
        log << "load_png(\"" << path << "\"): could not create end_info" << newl;
    }

    if (setjmp(png_jmpbuf(libpng))) {
        png_destroy_read_struct(&libpng, &start_info, &end_info);
        fclose(file);
        log << "load_png(\"" << path << "\"): big error, setjmp hit" << newl;
    }

    png_init_io(libpng, file);
    png_set_sig_bytes(libpng, number);
    png_read_info(libpng, start_info);

    U32 width, height;
    I32 bpc;
    I32 color_type, interlace_type, compression_type, filter_type;
    png_get_IHDR(libpng, start_info, &width, &height, &bpc,
        &color_type, &interlace_type, &compression_type, &filter_type);

    U8 channels = png_get_channels(libpng, start_info);
    U8 bpp = channels * bpc;
    U32 row_bytes = png_get_rowbytes(libpng, start_info);

    U64 bytes = width * height * bpp;
    U64 rows = bytes / row_bytes;
    U8 * data  = new U8[bytes];
    U8 ** row = new U8 * [rows];
    for (U64 i = 0; i < rows; i++)
        row[i] = data + i * row_bytes;

    png_read_image(libpng, row);

    fclose(file);

    TextureData * result = new TextureData();

    result->width = width;
    result->height = height;
    result->bpp = bpp;
    result->data = data;
    result->rgb = true;
    // TODO: hack stops non-diffuse maps from being broken by SRGB correction
    if (path.find("diffuse") != string::npos) result->srgb = true;
    else result->srgb = false;

    // Keep it the right way up for OpenGL. :)
    result->flip();

    return result;

    // TODO: Fix the normal PngLoader implementation.

    /*
    MappedFile file(path, MappedFileMode::READ_ONLY);

    U32 counter = 0;

    // TODO: lambdas are slow I think especially because we are calling them
    // a trillion times
    auto next_u8 = [&]() {
        U8 result = *(reinterpret_cast<const U8 *>(file.data + counter));

        counter += 1;

        return result;
    };

    auto next_u16 = [&]() {
        U16 result = *(reinterpret_cast<const U16 *>(file.data + counter));

        counter += 2;

        return result;
    };

    auto next_u32 = [&]() {
        U32 result = *(reinterpret_cast<const U32 *>(file.data + counter));

        counter += 4;

        return result;
    };

    auto next_bytes = [&](U32 length) {
        U8 * result = new U8[length];

        memcpy(result, file.data + counter, length);
        counter += length;

        return result;
    };

    auto next_string = [&](U32 length) {
        char * result = new char[length + 1];

        memcpy(result, file.data + counter, length);
        result[length] = 0;
        counter += length;

        return result;
    };

    static const U8 PNG_HEADER[8] = {0x89, 'P', 'N', 'G', 0xD, 0xA, 0x1A, 0xA};
    U8 * png_header = next_bytes(8);
    if (memcmp(png_header, PNG_HEADER, 8) != 0)
       log << path << " has an incorrect PNG signature" << newl;

    U32 png_width;
    U32 png_height;
    U8 png_color_type;
    U8 png_palette_length;
    U8 png_compression_type;
    U8 png_filter_type;
    U8 png_interlace_type;

    U8 * png_zlib_data;
    U32 png_zlib_data_length;

    U8 * png_ztxt_data;
    U32 png_ztxt_data_length;

    U8 channel_bytes = 1;
    U8 channel_num = 0;

    while (counter < file.data_length) {
        U32 chunk_length = swap_endian(next_u32());
        char * chunk_type = next_string(4);
        U8 * chunk_data = next_bytes(chunk_length);
        U8 * chunk_crc = next_bytes(4);

        if (strcmp(chunk_type, "IHDR") == 0) {
            png_width = swap_endian(*(U32 *)(chunk_data + 0));
            png_height = swap_endian(*(U32 *)(chunk_data + 4));
            png_compression_type = *(chunk_data + 10);
            png_filter_type = *(chunk_data + 11);
            png_interlace_type = *(chunk_data + 12);

            channel_bytes = *(chunk_data + 8) / 8;
            switch (*(chunk_data + 9)) {
                case 0: channel_num = 1; break;
                case 2: channel_num = 3; break;
                case 3: channel_num = 3; break;
                case 4: channel_num = 2; break;
                case 6: channel_num = 4; break;
            }
        } else if (strcmp(chunk_type, "PLTE") == 0) {
            if (chunk_length % 3 == 0) {
                png_palette_length = chunk_length / 3;
            }
        } else if (strcmp(chunk_type, "IDAT") == 0) {
            png_zlib_data = (U8 *)malloc(chunk_length);
            png_zlib_data_length = chunk_length;
            memcpy(png_zlib_data, chunk_data, chunk_length);
        } else if (strcmp(chunk_type, "IEND") == 0) {

        } else if (strcmp(chunk_type, "gAMA") == 0) {

        } else if (strcmp(chunk_type, "iCCP") == 0) {
            char * keyword = strdup((char *)chunk_data);
            U32 keyword_length = strlen(keyword);

            U8 compression = *(chunk_data + keyword_length + 1);

            U32 profile_length = chunk_length - keyword_length - 2;
            U8 * profile_data = (U8 *)malloc(profile_length);
            memcpy(profile_data, chunk_data, profile_length);
        } else if (strcmp(chunk_type, "sRGB") == 0) {

        } else if (strcmp(chunk_type, "zTXt") == 0) {
            char * keyword = strdup((char *)chunk_data);
            U32 keyword_length = strlen(keyword);

            chunk_data += keyword_length + 2;
            chunk_length -= keyword_length + 2;

            png_ztxt_data = (U8 *)malloc(chunk_length);
            png_ztxt_data_length = chunk_length;
            memcpy(png_ztxt_data, chunk_data, chunk_length);
        } else if (strcmp(chunk_type, "pHYs") == 0) {

        }
    }

    TextureData * result = new TextureData();

    log << "---- ---- ---- ---- ----  zTXt data: ---- ---- ---- ---- ----" << newl;
    zlib_inflate(png_ztxt_data, png_ztxt_data_length);

    log << "---- ---- ---- ---- ---- Image data: ---- ---- ---- ---- ----" << newl;
    result->width = png_width;
    result->height = png_height;
    result->bpp = channel_bytes * channel_num * 8;
    result->data = (U8 *)zlib_inflate(png_zlib_data, png_zlib_data_length);
    result->rgb = true;
    log << path << ':' << result->width << 'x' << result->height << newl;

    return result;

    */
}

}
