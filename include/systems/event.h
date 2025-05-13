#pragma once

#include <variant>
#include <vector>
#include <utility>
#include "../components/vec2.h"
#include "../components/input.h"
#include "../components/health.h"
#include "raylib.h"

enum class EventType {

    WALL_COLLISION,
    COLLISION,
    INPUT,
    CREATE_COMPONENT,
    PLAYER_MOVEMENT,
    UPDATE_VELOCITY,
    UPDATE_PLAYER_HEALTH,
    MOVE_INTENT,
    MOVE_SUCCESS,
    MOVEMENT,
    SPRITE_UPDATE,
    SPRITE_PHYSICS_UPDATE,
    CAMERA_UPDATE,
    MOVE,
    ATTACK,
    DAMAGE,
    HEAL,
    DESTROY,
    SIZE
};

using EventData = std::variant<uint32_t, Vec2, Input, Health, std::pair<Rectangle, Rectangle> >;

struct Event{
    EventType type;
    uint32_t entityA;
    uint32_t entityB;

    EventData data;

    Event(EventType type, uint32_t entityA, uint32_t entityB, EventData data);
    Event(EventType type, uint32_t entityA, uint32_t entityB);
    Event(EventType type, uint32_t entityA, EventData data);
    Event(EventType type, uint32_t entityA);
};




