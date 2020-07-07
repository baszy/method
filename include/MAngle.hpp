#pragma once

#include <cmath>

namespace method {

template <typename T>
T radians(T degrees) {
    return degrees / 180 * M_PI;
}

template <typename T>
T degrees(T radians) {
    return radians / M_PI * 180;
}

}