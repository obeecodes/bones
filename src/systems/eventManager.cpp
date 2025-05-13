#include "../../include/systems/eventManager.h"

EventManager* EventManager::instance = nullptr;

EventManager* EventManager::Instance(){
    if(!instance){
        instance = new EventManager();
    }
    return instance;
}

EventManager::EventManager(){}

void EventManager::subscribe(EventType type, void(*handler)(const Event&)){
    this->handlers[static_cast<size_t>(type)].push_back(handler);
}

void EventManager::publish(const Event& event){
    events.push(event);
}

// processes all published events
void EventManager::processEvents() {
    while (!events.empty()) {
        const Event& event = events.front();

        size_t typeIndex = static_cast<size_t>(event.type);
        for (auto& handler : handlers[typeIndex]) {
            handler(event);
        }

        events.pop();
    }
}