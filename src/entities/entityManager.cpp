#include "../../include/entities/entityManager.h"
#include "../../include/components/sprite.h"
#include "../../include/components/collider.h"
#include "../../include/components/audio.h"
#include "../../include/systems/spriteSystem.h"
#include "../../include/systems/collisionSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/audioSystem.h"
#include "../../include/systems/inputSystem.h"
#include "../../include/systems/healthSystem.h"
#include "../../include/core/luastate.h"
#include <cstdlib>
#include <iostream>

#include <set>
#include "../../include/components/vec2.h"

EntityManager* EntityManager::instance = nullptr;

EntityManager::EntityManager()
:eventManager(EventManager::Instance()){
    this->entityID = 0;
    
}

EntityManager* EntityManager::Instance(){
    if(!instance){
    srand((unsigned)time(NULL));
        instance = new EntityManager();
    }
    return instance;
}

void EntityManager::init(){
    initMainMenu();
}

void EntityManager::initMainMenu() {
    LuaState::Instance()->callScript("src/scripts/mainMenu.lua");
    lua_State* L = LuaState::Instance()->getLuaState();

    lua_getglobal(L, "MainMenu");

    if (!lua_istable(L, -1)) {
        std::cerr << "Error: 'MainMenu' is not a table or not defined.\n";
        lua_pop(L, 1);
        return;
    }

    lua_getfield(L, -1, "init");

    if (!lua_isfunction(L, -1)) {
        std::cerr << "Error: 'MainMenu.init' is not a function.\n";
        lua_pop(L, 2);
        return;
    }

    lua_pushvalue(L, -2);

    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
        std::cerr << "Lua error in MainMenu:init: " << lua_tostring(L, -1) << "\n";
        lua_pop(L, 1);
    }

    lua_pop(L, 1);
}

void EntityManager::initPlayer() {
    LuaState::Instance()->callScript("src/scripts/player.lua");
    lua_State* L = LuaState::Instance()->getLuaState();

    lua_getglobal(L, "Player"); 

    if (!lua_istable(L, -1)) {
        std::cerr << "Error: 'Player' is not a table or not defined.\n";
        lua_pop(L, 1);
        return;
    }

    lua_getfield(L, -1, "init");

    if (!lua_isfunction(L, -1)) {
        std::cerr << "Error: 'Player.init' is not a function.\n";
        lua_pop(L, 2); 
        return;
    }

    lua_pushvalue(L, -2); 

    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
        std::cerr << "Lua error in Player:init: " << lua_tostring(L, -1) << "\n";
        lua_pop(L, 1); 
    }

    lua_pop(L, 1); 
}

void EntityManager::initMap(){
    LuaState::Instance()->callScript("src/scripts/map.lua");

    lua_State* L = LuaState::Instance()->getLuaState();

    lua_getglobal(L, "Map");

    lua_getfield(L, -1, "init");

    lua_pushvalue(L, -2);

    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
        std::cerr << "Lua error in Map:init: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }

    lua_pop(L, 1); 
}

void EntityManager::initEnemySpawner(){
    LuaState::Instance()->callScript("src/scripts/enemySpawner.lua");
    lua_State* L = LuaState::Instance()->getLuaState();

    lua_getglobal(L, "EnemySpawner");

    if (!lua_istable(L, -1)) {
        std::cerr << "Error: 'EnemySpawner' is not a table or not defined.\n";
        lua_pop(L, 1);
        return;
    }

    lua_getfield(L, -1, "init");

    if (!lua_isfunction(L, -1)) {
        std::cerr << "Error: 'EnemySpawner.init' is not a function.\n";
        lua_pop(L, 2); 
        return;
    }

    lua_pushvalue(L, -2);

    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
        std::cerr << "Lua error in EnemySpawner:init: " << lua_tostring(L, -1) << "\n";
        lua_pop(L, 1); 
    }

    lua_pop(L, 1); 
}




uint32_t EntityManager::createEntity() {
    return EntityManager::Instance()->entityID++;
}



