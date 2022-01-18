#pragma once

namespace method {

// TODO: Use
template <typename Type>
class AABB {
public:
    TVec3<Type> position;
    // std::array<TVec3<Type>, 8> vertices;

    AABB(TVec3<Type> position)
        : position(position) {}
};

};
