#pragma once 

#include "../../external/lua/include/lua.hpp"
#include "../components/vec2.h"
#include "../systems/eventManager.h"
#include "../../include/systems/audioSystem.h"
#include "../../include/systems/collisionSystem.h"
#include "../../include/systems/healthSystem.h"
#include "../../include/systems/inputSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/scriptSystem.h"
#include "../../include/systems/spriteSystem.h"
#include "../../include/systems/uiSystem.h"



class LuaState {
public:
    static LuaState* Instance();
    void pushEntityId(uint32_t entityID);
    void callScript(const char* file);
    void init();
    void callUICallback(int luaCallbackRef, uint32_t entityID);
    void createKeyboardKeys();
    lua_State* getLuaState();

    // lua expects c style function pointer 
    static int lua_createEntity(lua_State* L);
    static int lua_createAudioComponent(lua_State* L);
    static int lua_createColliderComponent(lua_State* L);
    static int lua_createHealthComponent(lua_State* L);
    static int lua_createInputComponent(lua_State* L);
    static int lua_createPhysicsComponent(lua_State* L);
    static int lua_createTransformComponent(lua_State* L);
    static int lua_createScriptComponent(lua_State* L);
    static int lua_createSpriteComponent(lua_State* L);
    static int lua_createUIComponent(lua_State* L);
    static int lua_addTexture(lua_State* L);
    static int lua_addSound(lua_State* L);
    static int lua_playSound(lua_State* L);
    static int lua_moveIntent(lua_State* L);
    static int lua_setKeys(lua_State* L);
    static int lua_isKeyDown(lua_State* L);
    static int lua_isKeyPressed(lua_State* L);
    static int lua_getMapSize(lua_State* L);
    static int lua_callScript(lua_State* L);
    static int lua_getPosition(lua_State* L);
    static int lua_getHealth(lua_State* L);
    static int lua_getVelocity(lua_State* L);
    static int lua_dealDamage(lua_State* L);
    static int lua_destroyEntity(lua_State* L);
    static int lua_updateUIComponent(lua_State* L);
    static int lua_toggleDrawColliders(lua_State* L);
    static int lua_closeEngine(lua_State* L);

private:
    LuaState();
    ~LuaState();
    static LuaState* instance;
    lua_State* L;
    void registerFunctions();
    EventManager* eventManager;

};

