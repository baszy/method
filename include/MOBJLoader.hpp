#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "MMeshData.hpp"

namespace method {

class MOBJLoader : MResourceLoader {
private:
    static const ivec3 s_index_bias;

    static vec3 process_v(const std::string & line);
    static vec2 process_vt(const std::string & line);
    static vec3 process_vn(const std::string & line);
    static void process_f(const std::string & line, ivec3 * face, ivec3 * uv,
        ivec3 * normal);

public:
    // TODO: why why why is this a pointer
    MMeshData * load(const std::string & filename) override;

};

}
