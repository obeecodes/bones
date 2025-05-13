#include "../../include/systems/event.h"

Event::Event(EventType type, uint32_t entityA, uint32_t entityB, EventData data)
    : type(type), entityA(entityA), entityB(entityB), data(data){}

Event::Event(EventType type, uint32_t entityA, uint32_t entityB)
    : type(type), entityA(entityA), entityB(entityB){}

Event::Event(EventType type, uint32_t entityA, EventData data)
    : type(type), entityA(entityA), data(data){}

Event::Event(EventType type, uint32_t entityA)
    : type(type), entityA(entityA){}