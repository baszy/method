#pragma once

#include <cmath>
#include <cstring>

#include "Type/TVec3.hpp"

namespace method {

template <typename Type>
class TMat4 {
public:
    Type m[4][4];

    TMat4() {
        std::memset(this->m, 0, 4 * 4 * sizeof(Type));
        this->m[0][0] = 1;
        this->m[1][1] = 1;
        this->m[2][2] = 1;
        this->m[3][3] = 1;
    }

    TMat4(Type elements[4][4]) {
        std::memcpy(this->m, elements, 4 * 4 * sizeof(Type));
    }

    TMat4(const TMat4<Type> & other) {
        std::memcpy(this->m, other.m, 4 * 4 * sizeof(Type));
    }

    TMat4<Type> & operator = (const TMat4<Type> & other) {
        if (this != &other) {
            std::memcpy(this->m, other.m, 4 * 4 * sizeof(Type));
        }

        return * this;
    }

    const Type * get_data() const {
        return &this->m[0][0];
    }

};

template <typename Type>
TMat4<Type> operator * (const TMat4<Type> & lhs, const TMat4<Type> & rhs) {
    TMat4<Type> result;

    // TODO: Ripe for optimization
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            Type dot = 0;
            for (int k = 0; k < 4; k++) {
                dot += (rhs.m[x][k] * lhs.m[k][y]);
            }
            result.m[x][y] = dot;
        }
    }

    return result;
}

template <typename Type>
TMat4<Type> translate(const TVec3<Type> & translation) {
    TMat4<Type> result;

    result.m[3][0] = translation.x;
    result.m[3][1] = translation.y;
    result.m[3][2] = translation.z;

    return result;
}

template <typename Type>
TMat4<Type> rotate(TVec3<Type> axis, Type angle) {
    TMat4<Type> result;

    Type sina = sin(angle);
    Type cosa = cos(angle);
    Type omc = 1.0f - cosa;

    axis = normalize(axis);

    result.m[0][0] = axis.x * axis.x * omc + cosa;
    result.m[1][0] = axis.x * axis.y * omc - axis.z * sina;
    result.m[2][0] = axis.x * axis.z * omc + axis.y * sina;
    result.m[3][0] = 0;

    result.m[0][1] = axis.y * axis.x * omc + axis.z * sina;
    result.m[1][1] = axis.y * axis.y * omc + cosa;
    result.m[2][1] = axis.y * axis.z * omc - axis.x * sina;
    result.m[3][1] = 0;

    result.m[0][2] = axis.z * axis.x * omc - axis.y * sina;
    result.m[1][2] = axis.z * axis.y * omc + axis.x * sina;
    result.m[2][2] = axis.z * axis.z * omc + cosa;
    result.m[3][2] = 0;

    return result;
}

template <typename Type>
TMat4<Type> scale(const TVec3<Type> & scale) {
    TMat4<Type> result;

    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = scale.z;

    return result;
}

template <typename Type>
TMat4<Type> invert(const TMat4<Type> & source) {
    // TODO: implement
}

template <typename Type>
TMat4<Type> transpose(const TMat4<Type> & source) {
    TMat4<Type> result;

    result.m[0][0] = source.m[0][0];
    result.m[0][1] = source.m[1][0];
    result.m[0][2] = source.m[2][0];
    result.m[0][3] = source.m[3][0];

    result.m[1][0] = source.m[0][1];
    result.m[1][1] = source.m[1][1];
    result.m[1][2] = source.m[2][1];
    result.m[1][3] = source.m[3][1];

    result.m[2][0] = source.m[0][2];
    result.m[2][1] = source.m[1][2];
    result.m[2][2] = source.m[2][2];
    result.m[2][3] = source.m[3][2];

    result.m[3][0] = source.m[0][3];
    result.m[3][1] = source.m[1][3];
    result.m[3][2] = source.m[2][3];
    result.m[3][3] = source.m[3][3];

    return result;
}

template <typename Type>
TMat4<Type> look_at(TVec3<Type> position, TVec3<Type> at, TVec3<Type> up) {
    TMat4<Type> result;

    TVec3<Type> forward = normalize(at - position);
    TVec3<Type> side = normalize(cross(forward, normalize(up)));
    TVec3<Type> vertical = normalize(cross(side, forward));

    result.m[0][0] = side.x;
    result.m[1][0] = side.y;
    result.m[2][0] = side.z;

    result.m[0][1] = vertical.x;
    result.m[1][1] = vertical.y;
    result.m[2][1] = vertical.z;

    result.m[0][2] = -forward.x;
    result.m[1][2] = -forward.y;
    result.m[2][2] = -forward.z;

    result.m[3][0] = -dot(side, position);
    result.m[3][1] = -dot(vertical, position);
    result.m[3][2] = dot(forward, position);

    // Too slow
    // return result * translate(-position);

    return result;
}

template <typename Type>
TMat4<Type> orthographic(Type left, Type right, Type top, Type bottom,
    Type near, Type far) {

    TMat4<Type> result;

    result.m[0][0] = 2 / (right - left);
    result.m[1][1] = 2 / (top - bottom);
    result.m[2][2] = 2 / (near - far);

    result.m[3][0] = (left + right) / (left - right);
    result.m[3][1] = (bottom + top) / (bottom - top);
    result.m[3][2] = (far + near) / (near - far);

    return result;
}

template <typename Type>
TMat4<Type> perspective(Type fov, Type aspect, Type near, Type far) {
    TMat4<Type> result;

    Type h = tan(fov / 2);

    result.m[0][0] = 1 / (aspect * h);
    result.m[1][1] = 1 / h;
    result.m[2][2] = -(far + near) / (far - near);
    result.m[2][3] = -1;
    result.m[3][2] = -(2 * far * near) / (far - near);
    result.m[3][3] = 0;

    return result;
}

}
