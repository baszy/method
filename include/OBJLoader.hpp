#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "MeshData.hpp"

#define METHOD_OBJ_INDEX_BIAS IVec3(1, 1, 1)

namespace method {

// Anyway this function assumes you own the pointer and will free it later
MeshData * load_obj(const std::string & filename);

}
