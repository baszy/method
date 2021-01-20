#include "ReadFile.hpp"

#include <fstream>
#include <stdexcept>

namespace method {

std::string ReadFile(std::string filename) {
    std::ifstream stream_in(filename.c_str(), std::ios::in | std::ios::binary);

    if (stream_in) {
        std::string result;

        stream_in.seekg(0, std::ios::end);
        result.resize(stream_in.tellg());
        stream_in.seekg(0, std::ios::beg);

        stream_in.read(&result[0], result.size());

        stream_in.close();
        return result;
    }

    // TODO: instead of throwing exception, deal with error locally
    throw std::runtime_error("File \"" + filename + "\" could not be opened");
}

}