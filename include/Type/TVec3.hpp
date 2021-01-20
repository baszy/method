#pragma once

#include <cmath>

namespace method {

template <typename Type>
class TVec3 {
public:
    // TODO: contiguous memory
    Type x, y, z;

    TVec3()
        : TVec3(0, 0, 0) {}

    TVec3(Type x, Type y, Type z)
        : x(x), y(y), z(z) {}

};

template <typename Type>
TVec3<Type> operator - (const TVec3<Type> & source) {
    TVec3<Type> result;

    result.x = -source.x;
    result.y = -source.y;
    result.z = -source.z;

    return result;
}

template <typename Type>
TVec3<Type> operator + (const TVec3<Type> & lhs, const TVec3<Type> & rhs) {
    TVec3<Type> result;

    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;

    return result;
}

template <typename Type>
TVec3<Type> operator - (const TVec3<Type> & lhs, const TVec3<Type> & rhs) {
    TVec3<Type> result;

    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;

    return result;
}

template <typename Type>
TVec3<Type> operator * (Type lhs, const TVec3<Type> & rhs) {
    TVec3<Type> result;

    result.x = lhs * rhs.x;
    result.y = lhs * rhs.y;
    result.z = lhs * rhs.z;

    return result;
}

template <typename Type>
Type magnitude(const TVec3<Type> & source) {
    return sqrt((source.x * source.x) + (source.y * source.y)
        + (source.z * source.z));
}

template <typename Type>
TVec3<Type> cross(const TVec3<Type> & lhs, const TVec3<Type> & rhs) {
    TVec3<Type> result;

    result.x = (lhs.y * rhs.z) - (rhs.y * lhs.z);
    result.y = (lhs.z * rhs.x) - (rhs.z * lhs.x);
    result.z = (lhs.x * rhs.y) - (rhs.x * lhs.y);

    return result;
}

template <typename Type>
TVec3<Type> normalize(const TVec3<Type> & source) {
    TVec3<Type> result;

    Type m = magnitude(source);
    result.x = source.x / m;
    result.y = source.y / m;
    result.z = source.z / m;

    return result;
}

template <typename Type>
Type dot(const TVec3<Type> & lhs, const TVec3<Type> & rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

}
