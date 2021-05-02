#pragma once

#include <string>

#include "MeshData.hpp"

#define METHOD_OBJ_INDEX_BIAS IVec3(1, 1, 1)

namespace method {

MeshData * load_obj(const std::string & path);

}
