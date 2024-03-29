#pragma once

#include "Math.hpp"

namespace method {

template <typename Type>
class TVec2 {
public:
    // TODO: contiguous memory
    Type x, y;

    TVec2()
        : TVec2(0, 0) {}

    explicit TVec2(Type all)
        : TVec2(all, all) {}

    TVec2(Type x, Type y)
        : x(x), y(y) {}
};

template <typename Type>
TVec2<Type> operator - (const TVec2<Type> & source) {
    TVec2<Type> result;

    result.x = -source.x;
    result.y = -source.y;

    return result;
}

template <typename Type>
TVec2<Type> operator + (const TVec2<Type> & lhs, const TVec2<Type> & rhs) {
    TVec2<Type> result;

    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;

    return result;
}

template <typename Type>
TVec2<Type> operator += (TVec2<Type> & lhs, const TVec2<Type> & rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

template <typename Type>
TVec2<Type> operator - (const TVec2<Type> & lhs, const TVec2<Type> & rhs) {
    TVec2<Type> result;

    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;

    return result;
}

template <typename Type>
TVec2<Type> operator -= (TVec2<Type> & lhs, const TVec2<Type> & rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

template <typename Type>
TVec2<Type> operator * (Type lhs, const TVec2<Type> & rhs) {
    TVec2<Type> result;

    result.x = lhs * rhs.x;
    result.y = lhs * rhs.y;

    return result;
}

template <typename Type>
TVec2<Type> operator *= (TVec2<Type> & lhs, Type rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}

template <typename Type>
TVec2<Type> operator / (const TVec2<Type> & lhs, Type rhs) {
    TVec2<Type> result;

    result.x = (1 / lhs) * lhs.x;
    result.y = (1 / lhs) * lhs.y;

    return result;
}

template <typename Type>
TVec2<Type> operator /= (TVec2<Type> & lhs, Type rhs) {
    lhs.x *= (1 / rhs);
    lhs.y *= (1 / rhs);
    return lhs;
}

template <typename Type>
Type magnitude(const TVec2<Type> & source) {
    return sqrt((source.x * source.x) + (source.y * source.y));
}

template <typename Type>
TVec2<Type> normalize(TVec2<Type> source) {
    TVec2<Type> result;

    Type m = 1 / magnitude(source);
    result.x = source.x * m;
    result.y = source.y * m;

    return result;
}

template <typename Type>
Type dot(const TVec2<Type> & lhs, const TVec2<Type> & rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

template <typename Type>
bool operator == (const TVec2<Type> & lhs, const TVec2<Type> & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename Type>
bool operator != (const TVec2<Type> & lhs, const TVec2<Type> & rhs) {
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

}
