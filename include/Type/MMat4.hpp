#pragma once

#include <cmath>
#include <cstring>

#include "Type/MVec3.hpp"

namespace method {

template <typename Type>
class MMat4 {
public:
    Type m[4][4];

    MMat4() {
        std::memset(this->m, 0, 16 * sizeof(Type));
        this->m[0][0] = 1;
        this->m[1][1] = 1;
        this->m[2][2] = 1;
        this->m[3][3] = 1;
    }

    MMat4(Type elements[4][4]) {
        std::memcpy(this->m, elements, 16 * sizeof(Type));
    }

    MMat4(const MMat4<Type> & other) {
        std::memcpy(this->m, other.m, 16 * sizeof(Type));
    }

    MMat4<Type> & operator = (const MMat4<Type> & other) {
        if (this != &other) {
            std::memcpy(this->m, other.m, 16 * sizeof(Type));
        }

        return * this;
    }

    const Type * get_data() const {
        return &this->m[0][0];
    }

};

template <typename Type>
MMat4<Type> operator * (const MMat4<Type> & lhs, const MMat4<Type> & rhs) {
    MMat4<Type> result;

    result.m[0][0] = ((lhs.m[0][0] * rhs.m[0][0]) + (lhs.m[0][1] * rhs.m[1][0])
        + (lhs.m[0][2] * rhs.m[2][0]) + (lhs.m[0][3] * rhs.m[3][0]));
    result.m[0][1] = ((lhs.m[0][0] * rhs.m[0][1]) + (lhs.m[0][1] * rhs.m[1][1])
        + (lhs.m[0][2] * rhs.m[2][1]) + (lhs.m[0][3] * rhs.m[3][1]));
    result.m[0][2] = ((lhs.m[0][0] * rhs.m[0][2]) + (lhs.m[0][1] * rhs.m[1][2])
        + (lhs.m[0][2] * rhs.m[2][2]) + (lhs.m[0][3] * rhs.m[3][2]));
    result.m[0][3] = ((lhs.m[0][0] * rhs.m[0][3]) + (lhs.m[0][1] * rhs.m[1][3])
        + (lhs.m[0][2] * rhs.m[2][3]) + (lhs.m[0][3] * rhs.m[3][3]));

    result.m[1][0] = ((lhs.m[1][0] * rhs.m[0][0]) + (lhs.m[1][1] * rhs.m[1][0])
        + (lhs.m[1][2] * rhs.m[2][0]) + (lhs.m[1][3] * rhs.m[3][0]));
    result.m[1][1] = ((lhs.m[1][0] * rhs.m[0][1]) + (lhs.m[1][1] * rhs.m[1][1])
        + (lhs.m[1][2] * rhs.m[2][1]) + (lhs.m[1][3] * rhs.m[3][1]));
    result.m[1][2] = ((lhs.m[1][0] * rhs.m[0][2]) + (lhs.m[1][1] * rhs.m[1][2])
        + (lhs.m[1][2] * rhs.m[2][2]) + (lhs.m[1][3] * rhs.m[3][2]));
    result.m[1][3] = ((lhs.m[1][0] * rhs.m[0][3]) + (lhs.m[1][1] * rhs.m[1][3])
        + (lhs.m[1][2] * rhs.m[2][3]) + (lhs.m[1][3] * rhs.m[3][3]));

    result.m[2][0] = ((lhs.m[2][0] * rhs.m[0][0]) + (lhs.m[2][1] * rhs.m[1][0])
        + (lhs.m[2][2] * rhs.m[2][0]) + (lhs.m[2][3] * rhs.m[3][0]));
    result.m[2][1] = ((lhs.m[2][0] * rhs.m[0][1]) + (lhs.m[2][1] * rhs.m[1][1])
        + (lhs.m[2][2] * rhs.m[2][1]) + (lhs.m[2][3] * rhs.m[3][1]));
    result.m[2][2] = ((lhs.m[2][0] * rhs.m[0][2]) + (lhs.m[2][1] * rhs.m[1][2])
        + (lhs.m[2][2] * rhs.m[2][2]) + (lhs.m[2][3] * rhs.m[3][2]));
    result.m[2][3] = ((lhs.m[2][0] * rhs.m[0][3]) + (lhs.m[2][1] * rhs.m[1][3])
        + (lhs.m[2][2] * rhs.m[2][3]) + (lhs.m[2][3] * rhs.m[3][3]));

    result.m[3][0] = ((lhs.m[3][0] * rhs.m[0][0]) + (lhs.m[3][1] * rhs.m[1][0])
        + (lhs.m[3][2] * rhs.m[2][0]) + (lhs.m[3][3] * rhs.m[3][0]));
    result.m[3][1] = ((lhs.m[3][0] * rhs.m[0][1]) + (lhs.m[3][1] * rhs.m[1][1])
        + (lhs.m[3][2] * rhs.m[2][1]) + (lhs.m[3][3] * rhs.m[3][1]));
    result.m[3][2] = ((lhs.m[3][0] * rhs.m[0][2]) + (lhs.m[3][1] * rhs.m[1][2])
        + (lhs.m[3][2] * rhs.m[2][2]) + (lhs.m[3][3] * rhs.m[3][2]));
    result.m[3][3] = ((lhs.m[3][0] * rhs.m[0][3]) + (lhs.m[3][1] * rhs.m[1][3])
        + (lhs.m[3][2] * rhs.m[2][3]) + (lhs.m[3][3] * rhs.m[3][3]));

    return result;
}

template <typename Type>
MMat4<Type> translate(const MVec3<Type> & translation) {
    MMat4<Type> result;

    result.m[3][0] = translation.x;
    result.m[3][1] = translation.y;
    result.m[3][2] = translation.z;

    return result;
}

template <typename Type>
MMat4<Type> rotate(MVec3<Type> axis, Type angle) {
    MMat4<Type> result;

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
MMat4<Type> scale(const MVec3<Type> & scale) {
    MMat4<Type> result;

    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = scale.z;

    return result;
}

template <typename Type>
MMat4<Type> invert(const MMat4<Type> & source) {
    // TODO: implement
}

template <typename Type>
MMat4<Type> transpose(const MMat4<Type> & source) {
    MMat4<Type> result;

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
MMat4<Type> look_at(MVec3<Type> position, MVec3<Type> at, MVec3<Type> up) {
    MMat4<Type> result;

    MVec3<Type> forward;
    forward = at - position;
    forward = normalize(forward);

    MVec3<Type> side;
    up = normalize(up);
    side = normalize(cross(forward, up));

    MVec3<Type> vertical;
    vertical = cross(side, forward);

    result.m[0][0] = side.x;
    result.m[1][0] = side.y;
    result.m[2][0] = side.z;

    result.m[0][1] = up.x;
    result.m[1][1] = up.y;
    result.m[2][1] = up.z;

    result.m[0][2] = -forward.x;
    result.m[1][2] = -forward.y;
    result.m[2][2] = -forward.z;

    result = result * translate(-position);

    return result;
}

template <typename Type>
MMat4<Type> orthographic(Type left, Type right, Type top, Type bottom,
    Type near, Type far) {

    MMat4<Type> result;

    result.m[0][0] = 2 / (right - left);
    result.m[1][1] = 2 / (top - bottom);
    result.m[2][2] = 2 / (near - far);

    result.m[3][0] = (left + right) / (left - right);
    result.m[3][1] = (bottom + top) / (bottom - top);
    result.m[3][2] = (far + near) / (near - far);

    return result;
}

template <typename Type>
MMat4<Type> perspective(Type fov, Type aspect, Type near, Type far) {
    MMat4<Type> result;

    Type h = tan(fov / 2);

    result.m[0][0] = 1 / (aspect * h);
    result.m[1][1] = 1 / h;
    result.m[2][2] = (far + near) / (near - far);
    result.m[2][3] = -1;
    result.m[3][2] = (2 * far * near) / (far - near);

    return result;
}

}