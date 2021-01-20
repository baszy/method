#pragma once

#include <cmath>

#include "Type/TMat4.hpp"

namespace method {

template <typename Type>
class TQuat {
public:
    Type x, y, z, w;

    TQuat()
        : TQuat(0, 0, 0, 1) {}

    TQuat(Type x, Type y, Type z, Type w)
        : x(x), y(y), z(z), w(w) {}

    TQuat(TVec3<Type> angles) {
        // TODO: static_cast<Type>(2)
        angles *= 0.5;

        // TODO: sin() and cos() specialization for vectors
        Type cx = cos(angles.x);
        Type sx = sin(angles.x);
        Type cy = cos(angles.y);
        Type sy = sin(angles.y);
        Type cz = cos(angles.z);
        Type sz = sin(angles.z);

        x = sz * cy * cx - cz * sy * sx;
        y = cz * sy * cx + sz * cy * sx;
        z = cz * cy * sx - sz * sy * cx;
        w = cz * cy * cx + sz * sy * sx;
    }

    TQuat(TVec3<Type> axis, Type angle) {
        axis *= sin(angle / 2);
        return TQuat(axis.x, axis.y, axis.z, cos(angle / 2));
    }

    TMat4<Type> get_matrix() const {
        TMat4<Type> result;
        
        Type xx = x * x;
        Type xy = x * y;
        Type xz = x * z;
        Type yy = y * y;
        Type yz = y * z;
        Type zz = z * z;
        Type wx = w * x;
        Type wy = w * y;
        Type wz = w * z;

        result.m[0][0] = 1 - 2 * (yy + zz);
        result.m[1][0] = 2 * (xy + wz);
        result.m[2][0] = 2 * (xz - wy);
        
        result.m[0][1] = 2 * (xy - wz);
        result.m[1][1] = 1 - 2 * (xx + zz);
        result.m[2][1] = 2 * (yz + wx);

        result.m[0][2] = 2 * (xz + wy);
        result.m[1][2] = 2 * (yz - wx);
        result.m[2][2] = 1 - 2 * (xx + yy);

        return result;
    }

};

}
