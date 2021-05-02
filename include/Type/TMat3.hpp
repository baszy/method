#pragma once

#include <cmath>
#include <cstring>

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

    TMat3(Type elements[3][3]) {
        std::memcpy(this->m, elements, 3 * 3 * sizeof(Type));
    }

    TMat3(const TMat3<Type> & other) {
        std::memcpy(this->m, other.m, 3 * 3 * sizeof(Type));
    }

    TMat3<Type> & operator = (const TMat3<Type> & other) {
        if (this != &other) {
            std::memcpy(this->m, other.m, 3 * 3 * sizeof(Type));
        }

        return * this;
    }

    const Type * get_data() const {
        return &this->m[0][0];
    }

};

template <typename Type>
TMat3<Type> operator * (const TMat3<Type> & lhs, const TMat3<Type> & rhs) {
    TMat3<Type> result;

    // TODO: Ripe for optimization
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
TMat3<Type> invert(const TMat3<Type> & source) {
    // TODO: implement
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

}
