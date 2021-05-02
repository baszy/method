#include "Hotloader.hpp"

namespace method {

HotloaderIndex Hotloader::index_of(const Source & path) {
    // First we linearly search for the string and return it
    for (int i = 0; i < sources.size(); i++) {
        if (sources[i] == path) return i;
    }

    // If we dont find it, we just append it
    sources.push_back(path);
    return sources.size() - 1;
}

Source Hotloader::source_of(HotloaderIndex handle) {
    return sources.at(handle);
}

}
