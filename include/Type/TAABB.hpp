#pragma once

#include "TVec3.hpp"

namespace method {

// TODO: Use
template <typename Type>
class TAABB {
public:
    TVec3<Type> position;
    // std::array<TVec3<Type>, 8> vertices;

    TAABB(TVec3<Type> position)
        : position(position) {}
};

};
