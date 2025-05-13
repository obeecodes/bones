#include "../../include/components/transform2d.h"

Transform2D::Transform2D()
    :   position(Vec2{}), rotation(0), scale(Vec2{1, 1}){}

Transform2D::Transform2D(Vec2 position, int rotation, Vec2 scale)
    :   position(position), rotation(rotation), scale(Vec2{1, 1}){}