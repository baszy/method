#pragma once

#include <string>

namespace method {

struct TGAImageDeveloperTag {
    short tag;
    int field_offset;
    int field_length;
};

class TGAImage {
public:
    // Header data
    char id_length;
    char color_map_type;
    char image_type;
    short color_map_start;
    short color_map_length;
    char color_map_bpp;
    short origin_x;
    short origin_y;
    short width;
    short height;
    char bpp;
    char descriptor;

    // Image and color map data
    char * id_data;
    char * color_map_data;
    char * image_data;

    // Developer area
/*  short tag_directory_length;
    TGAImageDeveloperTag * tag_directory_data; */
    
    // Extension area
/*  short extensions_length;
    char author_name[41];
    char author_comments_line0[81];
    char author_comments_line1[81];
    char author_comments_line2[81];
    char author_comments_line3[81];
    short date_month;
    short date_day;
    short date_year;
    short date_hour;
    short date_minute;
    short date_second;
    char job_name[41];
    short job_hour;
    short job_minute;
    short job_second;
    char software_name[41];
    short software_version;
    char software_letter;
    int key_color;
    short pixel_aspect_numerator;
    short pixel_aspect_denominator;
    short gamma_numerator;
    short gamma_denominator;
    int color_correction_start;
    int postage_stamp_start;
    int scan_table_start;
    char alpha_attributes;
    int * scan_table;
    char postage_stamp_width;
    char postage_stamp_height;
    char * postage_stamp_data;
    short color_correction_data[1024]; */

    // Footer data
    int extensions_start;
    int developer_start;
    char * signature;

    // We store these in signature instead
    // char reserved_character;
    // char null_terminator;

    TGAImage(const std::string & filename);
    ~TGAImage();

    /// \brief Prints out the parameters of a given TGA file
    void print_info();
};

}
