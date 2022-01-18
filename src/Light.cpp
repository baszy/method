#include "Light.hpp"

namespace method {

DirectionLight::DirectionLight(Vec3 direction, Vec3 color)
    : direction(direction), color(color) {}

PointLight::PointLight(Vec3 position, Vec3 color)
    : position(position), color(color) {}

SpotLight::SpotLight(Vec3 position, Vec3 direction, float angle, Vec3 color)
    : position(position), direction(direction), angle(angle), color(color) {}

};
