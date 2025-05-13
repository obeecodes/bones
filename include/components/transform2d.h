#pragma once

#include "../components/vec2.h"

struct Transform2D{
    Vec2 position;
    int rotation;
    Vec2 scale;
    Transform2D();
    Transform2D(Vec2 position, int rotation = 0, Vec2 scale = Vec2{1, 1});
};