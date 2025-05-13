#pragma once

#include <vector>
#include <unordered_set>
#include <cstdint>


template <typename T>
class ECS{
public:
    ECS();
    void createComponent(T component, uint32_t entityID);
    void destroyComponent(uint32_t entityID);

    std::vector<T>& getComponents();
    T getEntityComponent(uint32_t entityID);
    uint32_t& getInactiveIndex();
    uint32_t getEntityIndex(uint32_t entityID);
    uint32_t getEntityID(uint32_t index);
    bool hasComponent(uint32_t entityID);

private:
    std::vector<T> components;
    std::vector<uint32_t> denseComponents;
    std::vector<uint32_t> sparseComponents;
    uint32_t inactiveIndex;
    std::unordered_set<uint32_t> active;
};