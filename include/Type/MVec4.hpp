#pragma once

#include <cmath>

namespace method {

template <typename Type>
class MVec4 {
public:
    // TODO: contiguous memory
    Type x, y, z, w;

    MVec4()
        : MVec4(0, 0, 0, 0) {}

    MVec4(Type x, Type y, Type z, Type w)
        : x(x), y(y), z(z), w(w) {}

};

template <typename Type>
MVec4<Type> operator - (const MVec4<Type> & source) {
    MVec4<Type> result;

    result.x = -source.x;
    result.y = -source.y;
    result.z = -source.z;
    result.w = -source.w;

    return result;
}

template <typename Type>
MVec4<Type> operator + (const MVec4<Type> & lhs, const MVec4<Type> & rhs) {
    MVec4<Type> result;

    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;
    result.w = lhs.w + rhs.w;

    return result;
}

template <typename Type>
MVec4<Type> operator - (const MVec4<Type> & lhs, const MVec4<Type> & rhs) {
    MVec4<Type> result;

    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    result.w = lhs.w - rhs.w;

    return result;
}

template <typename Type>
Type magnitude(const MVec4<Type> & source) {
    return sqrt((source.x * source.x) + (source.y * source.y)
        + (source.z * source.z) + (source.w * source.w));
}

template <typename Type>
MVec4<Type> normalize(const MVec4<Type> & source) {
    MVec4<Type> result;

    Type m = magnitude(source);
    result.x = source.x / m;
    result.y = source.y / m;
    result.z = source.z / m;
    result.w = source.w / m;

    return result;
}

template <typename Type>
Type dot(const MVec4<Type> & lhs, const MVec4<Type> & rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y)
        + (lhs.z * rhs.z) + (lhs.w * rhs.w);
}

}
