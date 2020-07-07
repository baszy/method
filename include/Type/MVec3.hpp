#pragma once

#include <cmath>

namespace method {

template <typename Type>
class MVec3 {
public:
    // TODO: contiguous memory
    Type x, y, z;

    MVec3()
        : MVec3(0, 0, 0) {}

    MVec3(Type x, Type y, Type z)
        : x(x), y(y), z(z) {}

};

template <typename Type>
MVec3<Type> operator - (const MVec3<Type> & source) {
    MVec3<Type> result;

    result.x = -source.x;
    result.y = -source.y;
    result.z = -source.z;

    return result;
}

template <typename Type>
MVec3<Type> operator + (const MVec3<Type> & lhs, const MVec3<Type> & rhs) {
    MVec3<Type> result;

    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;

    return result;
}

template <typename Type>
MVec3<Type> operator - (const MVec3<Type> & lhs, const MVec3<Type> & rhs) {
    MVec3<Type> result;

    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;

    return result;
}

template <typename Type>
Type magnitude(const MVec3<Type> & source) {
    return sqrt((source.x * source.x) + (source.y * source.y)
        + (source.z * source.z));
}

template <typename Type>
MVec3<Type> cross(const MVec3<Type> & lhs, const MVec3<Type> & rhs) {
    MVec3<Type> result;

    result.x = (lhs.y * rhs.z) - (rhs.y * lhs.z);
    result.y = (lhs.z * rhs.x) - (rhs.z * lhs.x);
    result.z = (lhs.x * rhs.y) - (rhs.x * lhs.y);

    return result;
}

template <typename Type>
MVec3<Type> normalize(const MVec3<Type> & source) {
    MVec3<Type> result;

    Type m = magnitude(source);
    result.x = source.x / m;
    result.y = source.y / m;
    result.z = source.z / m;

    return result;
}

template <typename Type>
Type dot(const MVec3<Type> & lhs, const MVec3<Type> & rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

}
