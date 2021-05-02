#include "MappedFile.hpp"

#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace method {

void MappedFile::close_file() {
    if (close(this->file) == -1)
        std::cout << "File could not be closed" << std::endl;
}

void MappedFile::clean_throw(std::string message) {
    this->close_file();
    std::cout << message << std::endl;
}

void MappedFile::map_file() {
    this->data = static_cast<char *>
        (mmap(NULL, this->data_length, PROT_READ, MAP_PRIVATE, this->file, 0u));

    if (this->data == MAP_FAILED)
        this->clean_throw("File could not be mapped to memory");
}

void MappedFile::open_file(std::string filename, MappedFileMode mode) {
    int flags = (mode == MappedFileMode::READ_WRITE) ? O_RDWR : O_RDONLY;

    this->file = open(filename.c_str(), flags);

    // TODO: instead of throwing exception, deal with error locally
    if (file == -1)
        this->clean_throw("File could not be opened");

    struct stat file_stats;

    // TODO: instead of throwing exception, deal with error locally
    if (fstat(file, &file_stats) == -1)
        this->clean_throw("Status of file could not be read");

    this->data_length = file_stats.st_size;
}

void MappedFile::unmap_file() {
    munmap(this->data, this->data_length);
}

MappedFile::MappedFile() {}

MappedFile::MappedFile(std::string filename, MappedFileMode mode) {
    this->open_file(filename, mode);
    this->map_file();
}

MappedFile::~MappedFile() {
    this->unmap_file();
}

}
