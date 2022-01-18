#include "MappedFile.hpp"

#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

namespace method {

MappedFile::MappedFile(string path, MappedFileMode mode)
    : File(path) {

    int flags = (mode == MappedFileMode::READ_WRITE) ? O_RDWR : O_RDONLY;

    file = open(path.c_str(), flags);

    if (file == -1)
        cout << "File could not be opened" << endl;

    struct stat file_stats;

    if (fstat(file, &file_stats) == -1) {
        if (close(file) == -1)
            cout << "File could not be closed" << endl;
        cout << "Status of file could not be read" << endl;
    }

    data_length = file_stats.st_size;
    data = mmap(NULL, data_length, PROT_READ, MAP_PRIVATE, file, 0);

    if (data == MAP_FAILED) {
        if (close(file) == -1)
            cout << "File could not be closed" << endl;
        cout << "File could not be memory mapped" << endl;
    }
}

MappedFile::~MappedFile() {
    munmap(data, data_length);
}

const void * MappedFile::get_data() const {
    return data;
}

size_t MappedFile::get_size() const {
    return data_length;
}

}
