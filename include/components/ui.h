#pragma once
#include "raylib.h"
#include "lauxlib.h" 
#include <string>

struct UI {
    Rectangle bounds;
    Color color;
    int layer;
    bool hovered;
    bool pressed;
    int luaCallbackRef = LUA_NOREF;
    std::string label;
    Vector2 labelOffset = {0.0f, 0.0f};
    int fontSize;
    Color textColor;

    UI();
    UI(Rectangle bounds, Color color, int layer = 0, int luaCallbackRef = LUA_NOREF, const std::string& label = "", Vector2 labelOffset = {0.0f, 0.0f}, int fontSize = 10, Color textColor = WHITE);
};