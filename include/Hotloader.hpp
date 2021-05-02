#pragma once

#include <string>
#include <vector>

namespace method {

typedef unsigned int HotloaderIndex;
typedef std::string Source;

class Hotloader {
public:
    std::vector<Source> sources;

    HotloaderIndex index_of(const Source & path);
    Source source_of(HotloaderIndex handle);
};

}
