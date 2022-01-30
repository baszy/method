#pragma once

#include <cstring>

#include "Math.hpp"
#include "TMat2.hpp"
#include "TVec2.hpp"
#include "TVec3.hpp"

namespace method {

template <typename Type>
class TMat3 {
public:
    Type m[3][3];

    TMat3() {
        std::memset(this->m, 0, 3 * 3 * sizeof(Type));
        this->m[0][0] = 1;
        this->m[1][1] = 1;
        this->m[2][2] = 1;
    }

    explicit TMat3(Type elements[3][3]) {
        std::memcpy(this->m, elements, 3 * 3 * sizeof(Type));
    }

    TMat3(const TMat3<Type> & other) {
        std::memcpy(this->m, other.m, 3 * 3 * sizeof(Type));
    }

    TMat3<Type> & operator = (const TMat3<Type> & other) {
        if (this != &other) {
            std::memcpy(this->m, other.m, 3 * 3 * sizeof(Type));
        }

        return *this;
    }

    const Type * get_data() const {
        return &this->m[0][0];
    }
};

template <typename Type>
Type determinant(const TMat3<Type> & source) {
    return source.m[0][0] * (source.m[1][1] * source.m[2][2] - source.m[2][1] * source.m[1][2])
         - source.m[0][1] * (source.m[1][0] * source.m[2][2] - source.m[2][0] * source.m[1][2])
         + source.m[0][2] * (source.m[1][0] * source.m[2][1] - source.m[2][0] * source.m[1][1]);
}

template <typename Type>
TMat3<Type> inverse(const TMat3<Type> & source) {
    TMat3<Type> result;

    Type div = 1 / determinant(source);

    result.m[0][0] =  div * (source.m[1][1] * source.m[2][2] - source.m[1][2] * source.m[2][1]);
    result.m[0][1] = -div * (source.m[0][1] * source.m[2][2] - source.m[0][2] * source.m[2][1]);
    result.m[0][2] =  div * (source.m[0][1] * source.m[1][2] - source.m[0][2] * source.m[1][1]);

    result.m[1][0] = -div * (source.m[1][0] * source.m[2][2] - source.m[1][2] * source.m[2][0]);
    result.m[1][1] =  div * (source.m[0][0] * source.m[2][2] - source.m[0][2] * source.m[2][0]);
    result.m[1][2] = -div * (source.m[0][0] * source.m[1][2] - source.m[0][2] * source.m[1][0]);

    result.m[2][0] =  div * (source.m[1][0] * source.m[2][1] - source.m[1][1] * source.m[2][0]);
    result.m[2][1] = -div * (source.m[0][0] * source.m[2][1] - source.m[0][1] * source.m[2][0]);
    result.m[2][2] =  div * (source.m[0][0] * source.m[1][1] - source.m[0][1] * source.m[1][0]);

    return result;
}

template <typename Type>
TMat3<Type> transpose(const TMat3<Type> & source) {
    TMat3<Type> result;

    result.m[0][0] = source.m[0][0];
    result.m[0][1] = source.m[1][0];
    result.m[0][2] = source.m[2][0];

    result.m[1][0] = source.m[0][1];
    result.m[1][1] = source.m[1][1];
    result.m[1][2] = source.m[2][1];

    result.m[2][0] = source.m[0][2];
    result.m[2][1] = source.m[1][2];
    result.m[2][2] = source.m[2][2];

    return result;
}

template <typename Type>
TMat3<Type> rotate(Type angle) {
    TMat3<Type> result;

    Type sina = sin(angle);
    Type cosa = cos(angle);

    result.m[0][0] = cosa;
    result.m[1][0] = -sina;

    result.m[0][1] = sina;
    result.m[1][1] = cosa;

    return result;
}

template <typename Type>
TMat3<Type> scale(const TVec2<Type> & scale) {
    TMat3<Type> result;

    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;

    return result;
}

template <typename Type>
TMat3<Type> translate(const TVec2<Type> & translation) {
    TMat3<Type> result;

    result.m[0][2] = translation.x;
    result.m[1][2] = translation.y;

    return result;
}

template <typename Type>
TMat3<Type> operator * (const TMat3<Type> & lhs, const TMat3<Type> & rhs) {
    TMat3<Type> result;

    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            Type dot = 0;
            for (int k = 0; k < 3; k++) {
                dot += (rhs.m[x][k] * lhs.m[k][y]);
            }
            result.m[x][y] = dot;
        }
    }

    return result;
}

template <typename Type>
TVec3<Type> operator * (const TMat3<Type> & lhs, const TVec3<Type> & rhs) {
    TVec3<Type> result;

    result.x = dot(rhs, TVec3<Type>(lhs.m[0][0], lhs.m[0][1], lhs.m[0][2]));
    result.y = dot(rhs, TVec3<Type>(lhs.m[1][0], lhs.m[1][1], lhs.m[1][2]));
    result.z = dot(rhs, TVec3<Type>(lhs.m[2][0], lhs.m[2][1], lhs.m[2][2]));

    return result;
}

}
