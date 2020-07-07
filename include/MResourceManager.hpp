#pragma once

#include <string>
#include <unordered_map>

namespace method {

class MResourceHandle {
private:
    unsigned int m_index;
};

template <typename Type>
class MResourceManager {
private:
    std::unordered_map<MResourceHandle, Type> m_resources;

public:
    load(std::string filename);
    set_loader();

};

}
