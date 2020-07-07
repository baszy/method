#pragma once

#include <cmath>

namespace method {

template <typename Type>
class MVec2 {
public:
    // TODO: contiguous memory
    Type x, y;

    MVec2()
        : MVec2(0, 0) {}

    MVec2(Type x, Type y)
        : x(x), y(y) {}

};

template <typename Type>
MVec2<Type> operator - (const MVec2<Type> & source) {
    MVec2<Type> result;

    result.x = -source.x;
    result.y = -source.y;

    return result;
}

template <typename Type>
MVec2<Type> operator + (const MVec2<Type> & lhs, const MVec2<Type> & rhs) {
    MVec2<Type> result;

    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;

    return result;
}

template <typename Type>
MVec2<Type> operator - (const MVec2<Type> & lhs, const MVec2<Type> & rhs) {
    MVec2<Type> result;

    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;

    return result;
}

template <typename Type>
Type magnitude(const MVec2<Type> & source) {
    return sqrt((source.x * source.x) + (source.y * source.y));
}

template <typename Type>
MVec2<Type> normalize(MVec2<Type> source) {
    MVec2<Type> result;

    Type m = magnitude(source);
    result.x = source.x / m;
    result.y = source.y / m;

    return result;
}

template <typename Type>
Type dot(const MVec2<Type> & lhs, const MVec2<Type> & rhs) {
    return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

}
