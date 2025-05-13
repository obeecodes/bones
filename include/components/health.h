#pragma once

#include <cstdint>

struct Health {
    uint32_t max;
    uint32_t current;

    Health(uint32_t max = 20);
};