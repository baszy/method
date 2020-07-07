#include "MMappedFile.hpp"

#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace method {

void MMappedFile::close_file() {
    if (close(this->file) == -1)
        throw std::runtime_error("File could not be closed");
}

void MMappedFile::clean_throw(std::string message) {
    this->close_file();
    throw std::runtime_error(message);
}

void MMappedFile::map_file() {
    this->data = static_cast<char *>
        (mmap(NULL, this->size, PROT_READ, MAP_PRIVATE, this->file, 0u));

    if (this->data == MAP_FAILED)
        this->clean_throw("File could not be mapped to memory");
}

void MMappedFile::open_file(std::string filename, map_mode mode) {
    int flags = (mode == map_mode::READ_WRITE) ? O_RDWR : O_RDONLY;

    this->file = open(filename.c_str(), flags);

    // TODO: instead of throwing exception, deal with error locally
    if (file == -1)
        this->clean_throw("File could not be opened");

    struct stat file_stats;

    // TODO: instead of throwing exception, deal with error locally
    if (fstat(file, &file_stats) == -1)
        this->clean_throw("Status of file could not be read");

    this->size = file_stats.st_size;
}

void MMappedFile::unmap_file() {
    munmap(this->data, this->size);
}

MMappedFile::MMappedFile() {}

MMappedFile::MMappedFile(std::string filename, map_mode mode) {
    this->open_file(filename, mode);
    this->map_file();
}

MMappedFile::~MMappedFile() {
    this->unmap_file();
}

const char * MMappedFile::get_data() const {
    return this->data;
}

unsigned int MMappedFile::get_size() const {
    return this->size;
}

}