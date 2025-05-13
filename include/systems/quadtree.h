#pragma once

#include <vector>
#include "../components/collider.h"
#include "raylib.h"
#include <cstdint>

class Quadtree{
public:

    Quadtree(Rectangle region);
    Quadtree(uint32_t level, Rectangle region);
    void clear();
    void insert(Collider collider);
    void remove(Collider collider);
    void retrieve(std::vector<Collider>& nearComponents, const Rectangle& rect) const;

private:
    const uint32_t MAX_DEPTH = 10;
    const uint32_t MAX_OBJECTS = 8;
    uint32_t level;

    Rectangle region;
    std::vector<Collider> regionComponents;
    Quadtree* nodes[4];

    void split();

    uint32_t getIndex(const Rectangle& rect) const;


};