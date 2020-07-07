#pragma once

#include <string>

namespace method {

class MMappedFile {
public:
    enum class map_mode : unsigned char {
        READ_ONLY = 0b01,
        READ_WRITE = 0b10
    };

private:
    int file;
    char * data;
    unsigned int size;

    void close_file();
    void clean_throw(std::string message);
    void map_file();
    void open_file(std::string filename, map_mode mode);
    void unmap_file();

public:
    MMappedFile();
    MMappedFile(std::string filename, map_mode mode);
    ~MMappedFile();

    const char * get_data() const;
    unsigned int get_size() const;

};

}