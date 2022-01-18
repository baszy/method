#pragma once

#include <cmath>

#include <Type/TVec2.hpp>
#include <Type/TVec3.hpp>

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
TVec4<Type> operator - (const TVec4<Type> & lhs, const TVec4<Type> & rhs) {
    TVec4<Type> result;

    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    result.w = lhs.w - rhs.w;

    return result;
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

// Template specializations

bool operator == (const TVec4<bool> & lhs, const TVec4<bool> & rhs);
bool operator == (const TVec4<int> & lhs, const TVec4<int> & rhs);
bool operator == (const TVec4<unsigned int> & lhs, const TVec4<unsigned int> & rhs);

}
