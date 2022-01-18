#pragma once

#include <string>

#include "Defs.hpp"

namespace method {

class File {
private:
    std::string path;

public:
    File(std::string path);

    std::string get_path() const;

    virtual const void * get_data() const = 0;
    virtual size_t get_size() const = 0;
};

}
