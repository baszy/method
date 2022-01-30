#pragma once

#include <cstring>

#include "Math.hpp"
#include "TVec2.hpp"

namespace method {

template <typename Type>
class TMat2 {
public:
    Type m[2][2];

    TMat2() {
        std::memset(this->m, 0, 2 * 2 * sizeof(Type));
        this->m[0][0] = 1;
        this->m[1][1] = 1;
    }

    explicit TMat2(Type elements[2][2]) {
        std::memcpy(this->m, elements, 2 * 2 * sizeof(Type));
    }

    TMat2(const TMat2<Type> & other) {
        std::memcpy(this->m, other.m, 2 * 2 * sizeof(Type));
    }

    TMat2<Type> & operator = (const TMat2<Type> & other) {
        if (this != &other) {
            std::memcpy(this->m, other.m, 2 * 2 * sizeof(Type));
        }

        return *this;
    }

    const Type * get_data() const {
        return &this->m[0][0];
    }
};

template <typename Type>
Type determinant(const TMat2<Type> & source) {
    return source.m[0][0] * source.m[1][1]
         - source.m[0][1] * source.m[1][0];
}

template <typename Type>
TMat2<Type> inverse(const TMat2<Type> & source) {
    TMat2<Type> result;

    Type div = 1 / determinant(source);

    result.m[0][0] =  div * source.m[1][1];
    result.m[0][1] = -div * source.m[0][1];

    result.m[1][0] = -div * source.m[1][0];
    result.m[1][1] =  div * source.m[0][0];

    return result;
}

template <typename Type>
TMat2<Type> transpose(const TMat2<Type> & source) {
    TMat2<Type> result;

    result.m[0][0] = source.m[0][0];
    result.m[0][1] = source.m[1][0];

    result.m[1][0] = source.m[0][1];
    result.m[1][1] = source.m[1][1];

    return result;
}

template <typename Type>
TMat2<Type> operator * (const TMat2<Type> & lhs, const TMat2<Type> & rhs) {
    TMat2<Type> result;

    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            Type dot = 0;
            for (int k = 0; k < 2; k++) {
                dot += (rhs.m[x][k] * lhs.m[k][y]);
            }
            result.m[x][y] = dot;
        }
    }

    return result;
}

template <typename Type>
TVec2<Type> operator * (const TMat2<Type> & lhs, const TVec2<Type> & rhs) {
    TVec2<Type> result;

    result.x = dot(rhs, TVec2<Type>(lhs.m[0][0], lhs.m[0][1]));
    result.y = dot(rhs, TVec2<Type>(lhs.m[0][1], lhs.m[1][1]));

    return result;
}

}
