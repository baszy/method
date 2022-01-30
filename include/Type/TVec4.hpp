#pragma once

#include "Math.hpp"
#include "TVec2.hpp"
#include "TVec3.hpp"

namespace method {

template <typename Type>
class TVec4 {
public:
    // TODO: contiguous memory
    Type x, y, z, w;

    TVec4()
        : TVec4(0, 0, 0, 0) {}

    explicit TVec4(Type all)
        : TVec4(all, all, all, all) {}

    TVec4(Type x, Type y, Type z, Type w)
        : x(x), y(y), z(z), w(w) {}

    // Dimension constructors
    TVec4(TVec2<Type> xy, Type z, Type w)
        : x(xy.x), y(xy.y), z(z), w(w) {}

    TVec4(TVec3<Type> xyz, Type w)
        : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
};

template <typename Type>
TVec4<Type> operator - (const TVec4<Type> & source) {
    TVec4<Type> result;

    result.x = -source.x;
    result.y = -source.y;
    result.z = -source.z;
    result.w = -source.w;

    return result;
}

template <typename Type>
TVec4<Type> operator + (const TVec4<Type> & lhs, const TVec4<Type> & rhs) {
    TVec4<Type> result;

    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;
    result.w = lhs.w + rhs.w;

    return result;
}

template <typename Type>
TVec4<Type> operator += (TVec4<Type> & lhs, const TVec4<Type> & rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}

template <typename Type>
TVec4<Type> operator - (const TVec4<Type> & lhs, const TVec4<Type> & rhs) {
    TVec4<Type> result;

    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    result.w = lhs.w - rhs.w;

    return result;
}

template <typename Type>
TVec4<Type> operator -= (TVec4<Type> & lhs, const TVec4<Type> & rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;
    return lhs;
}

template <typename Type>
TVec4<Type> operator * (Type lhs, const TVec4<Type> & rhs) {
    TVec4<Type> result;

    result.x = lhs * rhs.x;
    result.y = lhs * rhs.y;
    result.z = lhs * rhs.z;
    result.w = lhs * rhs.w;

    return result;
}

template <typename Type>
TVec4<Type> operator *= (TVec4<Type> & lhs, Type rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    lhs.w *= rhs;
    return lhs;
}

template <typename Type>
TVec4<Type> operator / (const TVec4<Type> & lhs, Type rhs) {
    TVec4<Type> result;

    result.x = (1 / lhs) * lhs.x;
    result.y = (1 / lhs) * lhs.y;
    result.z = (1 / lhs) * lhs.z;
    result.w = (1 / lhs) * lhs.w;

    return result;
}

template <typename Type>
TVec4<Type> operator /= (TVec4<Type> & lhs, Type rhs) {
    lhs.x *= (1 / rhs);
    lhs.y *= (1 / rhs);
    lhs.z *= (1 / rhs);
    lhs.w *= (1 / rhs);
    return lhs;
}

template <typename Type>
Type magnitude(const TVec4<Type> & source) {
    return sqrt((source.x * source.x) + (source.y * source.y)
        + (source.z * source.z) + (source.w * source.w));
}

template <typename Type>
TVec4<Type> normalize(const TVec4<Type> & source) {
    TVec4<Type> result;

    Type m = 1 / magnitude(source);
    result.x = source.x * m;
    result.y = source.y * m;
    result.z = source.z * m;
    result.w = source.w * m;

    return result;
}

template <typename Type>
Type dot(const TVec4<Type> & lhs, const TVec4<Type> & rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y)
        + (lhs.z * rhs.z) + (lhs.w * rhs.w);
}

template <typename Type>
bool operator == (const TVec4<Type> & lhs, const TVec4<Type> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

template <typename Type>
bool operator != (const TVec4<Type> & lhs, const TVec4<Type> & rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w;
}

}
