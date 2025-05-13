#pragma once

#include "raylib.h"
#include "vec2.h"

enum class ColliderType {
    STATIC,
    DYNAMIC,
    KINEMATIC,
    AUTOMATIC,
    TRIGGER
};

struct Collider {
    Rectangle rect;
    Vec2 offset;
    ColliderType type;

    Collider(Rectangle rect, Vec2 offset, ColliderType type = ColliderType::KINEMATIC);
    Collider(Rectangle rect, ColliderType type = ColliderType::KINEMATIC);
    Collider();
};