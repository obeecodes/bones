#include "../../include/components/physics.h"

Physics::Physics()
    : velocity(Vec2{}), acceleration(Vec2{}){}

Physics::Physics(Vec2 velocity, Vec2 acceleration)
    : velocity(velocity), acceleration(acceleration){}
