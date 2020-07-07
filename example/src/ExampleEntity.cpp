#include <iostream>

#include "MMeshData.hpp"
#include "MOBJLoader.hpp"

using namespace method;

int main() {

    MMeshData t = MOBJLoader::parse("example/resource/cube.obj");
    t.add_section({vec3(0,0,0)},{vec2(0,0)},{vec3(0,0,0)},{ivec3(0,0,0)});

    return 0;
}
