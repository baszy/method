#pragma once

#include "File.hpp"
#include "MeshData.hpp"

#define METHOD_OBJ_INDEX_BIAS IVec3(1, 1, 1)

namespace method {

MeshData * load_obj(const File & file);

}
