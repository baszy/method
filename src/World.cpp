#include "World.hpp"

namespace method {

LineDef::LineDef(Vec2 a, Vec2 b) : start(a), end(b) {}

float dist(Vec2 a, Vec2 b) {
    return magnitude(a - b);
}

float dist(LineDef l, Vec2 p) {
    Vec2 a = l.end - l.start;
    float mag = magnitude(a);
    a = (1.0f / mag) * a;
    Vec2 b = p - l.start;

    float d = dot(a, b);

    if (d < 0) return dist(l.start, p);
    else if (d > mag) return dist(l.end, p);

    return magnitude(b - (d * a));
}

float length(LineDef l) {
    return dist(l.start, l.end);
}

}
