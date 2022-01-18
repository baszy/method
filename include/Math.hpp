#pragma once

#include <cmath>

namespace method {

template <typename Type>
Type radians(Type degrees) {
    return degrees / 180 * M_PI;
}

template <typename Type>
Type degrees(Type radians) {
    return radians / M_PI * 180;
}

template <typename Type>
Type clamp(Type input, Type min, Type max) {
    if (input < min) return min;
    else if (input > max) return max;
    return input;
}

}
