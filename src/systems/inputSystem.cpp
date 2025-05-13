#include "../../include/systems/inputSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/spriteSystem.h"
#include "../../include/entities/entityManager.h"
#include "../../include/core/engine.h"
#include "../../include/systems/audioSystem.h"
#include "../../include/systems/eventManager.h"
#include "../../include/core/luastate.h"

InputSystem* InputSystem::instance = nullptr;

InputSystem::InputSystem()
    : inputComponentSystem(new ECS<Input>()),
      inputs(inputComponentSystem->getComponents()),
      inactiveIndex(inputComponentSystem->getInactiveIndex()),
      physics(PhysicsSystem::Instance()->getPhysics()),
      sprites(SpriteSystem::Instance()->getSprites()),
      eventManager(EventManager::Instance()),
      mousePosition{} {
        createSubscriptions();

    mouseButtonStates.resize(3, false); 
}

void InputSystem::createSubscriptions(){
    eventManager->subscribe(EventType::DESTROY, destroyEntity);
}

InputSystem* InputSystem::Instance() {
    if (!instance) instance = new InputSystem();
    return instance;
}

void InputSystem::init() {
    LuaState::Instance()->callScript("src/scripts/input.lua");

    // sets up keys in lua
    lua_State* L = LuaState::Instance()->getLuaState();
    lua_getglobal(L, "getKeys");

    if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
        std::cerr << "Lua error in getKeys(): " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return;
    }

    if (!lua_istable(L, -1)) {
        std::cerr << "Lua getKeys() did not return a table." << std::endl;
        lua_pop(L, 1);
        return;
    }

    // read and store keycodes
    size_t len = lua_rawlen(L, -1);
    trackedKeys.clear();
    for (size_t i = 1; i <= len; i++) {
        lua_rawgeti(L, -1, i);
        if (lua_isinteger(L, -1)) {
            trackedKeys.push_back(static_cast<int>(lua_tointeger(L, -1)));
        }
        lua_pop(L, 1); 
    }

    lua_pop(L, 1); 
}

void InputSystem::createInputComponent(Input input, uint32_t entityID) {
    inputComponentSystem->createComponent(input, entityID);
}

void InputSystem::update() {
    updateMouseButtonStates();
    updateKeyStates();
    for (uint32_t i = 0; i < inactiveIndex; i++) {

        Input& input = inputs[i];
        
        for (int key : trackedKeys) {
            input.keyStates[key] = keyStates[key];
        }

        input.mousePosition = mousePosition;
        input.mouseDelta    = mouseDelta;
        input.scrollDelta   = scrollDelta;
        
        input.mouseButtonStates = mouseButtonStates;
    }

}

void InputSystem::updateMouseButtonStates() {
    Vec2 currentMouse = Vec2{(float)GetMouseX(), (float)GetMouseY()};
    mouseDelta = currentMouse - mousePosition;
    mousePosition = currentMouse;
    scrollDelta = GetMouseWheelMove();
    
    mouseButtonStates[LEFT] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    mouseButtonStates[MIDDLE] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);
    mouseButtonStates[RIGHT] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
}

void InputSystem::updateKeyStates() {
    for (int key : trackedKeys) {
        
        bool isDown = IsKeyDown(key);
        bool wasDown = previousKeyStates[key];

        KeyState newState;

        if (isDown && !wasDown) {
            newState = KeyState::Pressed;
        } else if (!isDown && wasDown) {
            newState = KeyState::Released;
        } else if (isDown && wasDown) {
            newState = KeyState::Held;
        } else {
            newState = KeyState::None;
        }

        keyStates[key] = newState;
        previousKeyStates[key] = isDown;
    }
}


bool InputSystem::getMouseButtonState(int buttonNumber) {
    return mouseButtonStates[buttonNumber];
}

Vec2& InputSystem::getMousePosition() {
    return mousePosition;
}

void InputSystem::clean() {
    std::fill(mouseButtonStates.begin(), mouseButtonStates.end(), false);
}

std::vector<Input>& InputSystem::getInputs() {
    return inputs;
}

void InputSystem::setKeys(const std::vector<int>& keys){
    trackedKeys = keys;
}

void InputSystem::destroyEntity(const Event& event){
    InputSystem::Instance()->getInputComponentSystem()->destroyComponent(event.entityA);
}

ECS<Input>* InputSystem::getInputComponentSystem(){
    return this->inputComponentSystem;
}

