#pragma once

#include "Vector.hpp"

namespace method {

class DirectionLight {
public:
    Vec3 direction;
    Vec3 color;

    DirectionLight(Vec3 direction, Vec3 color);
};

class PointLight {
public:
    Vec3 position;
    Vec3 color;

    PointLight(Vec3 position, Vec3 color);
};

class SpotLight {
public:
    Vec3 position;
    Vec3 direction;
    float angle;
    Vec3 color;

    SpotLight(Vec3 position, Vec3 direction, float angle, Vec3 color);
};

}
