#pragma once

#include "vec2.h"

struct Physics {
    Vec2 velocity;
    Vec2 acceleration;

    Physics();
    Physics(Vec2 velocity, Vec2 acceleration);
};
