#include "../../include/components/collider.h"

Collider::Collider(Rectangle rect, Vec2 offset, ColliderType type)
    : rect(rect), offset(offset), type(type) {}

Collider::Collider(Rectangle rect, ColliderType type)
    : rect(rect), offset(0, 0), type(type) {}

Collider::Collider()
    : rect{0, 0, 0, 0}, offset(0, 0), type(ColliderType::KINEMATIC) {}