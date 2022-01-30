#pragma once

#include "Math.hpp"
#include "TVec2.hpp"

namespace method {

template <typename Type>
class TVec3 {
public:
    // TODO: contiguous memory
    Type x, y, z;

    TVec3()
        : TVec3(0, 0, 0) {}

    explicit TVec3(Type all)
        : TVec3(all, all, all) {}

    TVec3(Type x, Type y, Type z)
        : x(x), y(y), z(z) {}

    // Dimension constructors
    TVec3(TVec2<Type> xy, Type z)
        : x(xy.x), y(xy.y), z(z) {}
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
TVec3<Type> operator += (TVec3<Type> & lhs, const TVec3<Type> & rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
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
TVec3<Type> operator -= (TVec3<Type> & lhs, const TVec3<Type> & rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
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
TVec3<Type> operator *= (TVec3<Type> & lhs, Type rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
}

template <typename Type>
TVec3<Type> operator / (const TVec3<Type> & lhs, Type rhs) {
    TVec3<Type> result;

    result.x = (1 / lhs) * lhs.x;
    result.y = (1 / lhs) * lhs.y;
    result.z = (1 / lhs) * lhs.z;

    return result;
}

template <typename Type>
TVec3<Type> operator /= (TVec3<Type> & lhs, Type rhs) {
    lhs.x *= (1 / rhs);
    lhs.y *= (1 / rhs);
    lhs.z *= (1 / rhs);
    return lhs;
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

    Type m = 1 / magnitude(source);
    result.x = source.x * m;
    result.y = source.y * m;
    result.z = source.z * m;

    return result;
}

template <typename Type>
Type dot(const TVec3<Type> & lhs, const TVec3<Type> & rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

template <typename Type>
bool operator == (const TVec3<Type> & lhs, const TVec3<Type> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

template <typename Type>
bool operator != (const TVec3<Type> & lhs, const TVec3<Type> & rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
}

}
