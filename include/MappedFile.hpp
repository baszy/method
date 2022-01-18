#pragma once

#include <string>

#include "File.hpp"

namespace method {

enum MappedFileMode {
    READ_ONLY,
    READ_WRITE
};

class MappedFile : public File {
private:
    int file;

    size_t data_length;
    void * data;

public:
    MappedFile(std::string path, MappedFileMode mode);
    ~MappedFile();

    const void * get_data() const;
    size_t get_size() const;
};

}
