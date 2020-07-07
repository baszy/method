#pragma once

#include <string>

namespace method {

class MResourceLoader {
public:
    virtual ~MResourceLoader();

    virtual load(std::string filename) = 0;

};

}
