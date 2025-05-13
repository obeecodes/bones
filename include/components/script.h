#pragma once

#include <cstdint>

struct Script {
    const char* script;
    const char* table;

    Script();
    Script(const char* script, const char* table);
};