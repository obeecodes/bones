#pragma once

#include "event.h"
#include <functional>
#include <queue>

constexpr size_t EVENT_TYPE_COUNT = static_cast<size_t>(EventType::SIZE);

class EventManager{
public:
    static EventManager* Instance();
    void subscribe(EventType type, void(*handler)(const Event&));
    void publish(const Event& event);
    void processEvents();

private:
    static EventManager* instance;
    std::array<std::vector<void(*)(const Event&)>, EVENT_TYPE_COUNT> handlers;
    std::queue<Event> events;
    EventManager();
}; 

namespace Events {

    inline void publishMove(uint32_t entity, const Vec2& delta) {
        EventManager::Instance()->publish(Event(EventType::MOVE, entity, delta));
    }
    
    inline void publishDamage(uint32_t target, uint32_t source, uint32_t damage) {
        EventManager::Instance()->publish(Event(EventType::DAMAGE, target, source, damage));
    }
    
}
