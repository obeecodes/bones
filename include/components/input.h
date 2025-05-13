#pragma once 
#include <unordered_map>
#include <vector>
#include <utility>
#include "vec2.h"

enum class KeyState : uint8_t {
    None,
    Pressed,
    Released,
    Held
};

struct Input {
    
    std::unordered_map<int, KeyState> keyStates;

    std::vector<bool> mouseButtonStates;
    Vec2 mousePosition;
    Vec2 mouseDelta;
    float scrollDelta;

    Input();
};