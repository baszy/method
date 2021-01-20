#pragma once

#include <string>

namespace method {

enum MappedFileMode {
    READ_ONLY,
    READ_WRITE
};

class MappedFile {
public:
    int file;

    char * data;
    unsigned int data_length;

    void close_file();
    void clean_throw(std::string message);
    void map_file();
    void open_file(std::string filename, MappedFileMode mode);
    void unmap_file();

    MappedFile();
    MappedFile(std::string filename, MappedFileMode mode);
    ~MappedFile();

};

}
