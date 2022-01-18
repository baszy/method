#pragma once

#include "Vector.hpp"

namespace method {

struct LineDef {
    Vec2 start;
    Vec2 end;

    LineDef(Vec2 a, Vec2 b);
};

float dist(Vec2 a, Vec2 b);

float dist(LineDef l, Vec2 p);

float length(LineDef l);

}
