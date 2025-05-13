#include "../../include/systems/scriptSystem.h"


ScriptSystem* ScriptSystem::instance = nullptr;

ScriptSystem* ScriptSystem::Instance(){
  if(!instance){
    instance = new ScriptSystem();
  }
  return instance;
}

ScriptSystem::ScriptSystem()
: scriptComponentSystem(new ECS<Script>()),
  scripts(scriptComponentSystem->getComponents()),
  inactiveIndex(scriptComponentSystem->getInactiveIndex()),
  eventManager(EventManager::Instance()){
    this->L = LuaState::Instance()->getLuaState();
    createSubscriptions();

}

void ScriptSystem::update(float dt) {

  // reloads modifed scripts -- must implement onReload function to work
  for (const auto& [scriptPath, oldTime] : scriptTimestamps) {
    auto newTime = std::filesystem::last_write_time(scriptPath);
    if (newTime != oldTime) {
      scriptTimestamps[scriptPath] = newTime;
      modifiedScripts.insert(scriptPath);
    }
  }
  for (uint32_t i = 0; i < this->inactiveIndex; i++) {
    Script& script = this->scripts[i];
    if (modifiedScripts.find(std::string(script.script)) != modifiedScripts.end()) {
      reloadScript(script.script, script.table, scriptComponentSystem->getEntityID(i));
    }
  }
  for (uint32_t i = 0; i < this->inactiveIndex; i++) {
    Script& script = this->scripts[i];
    uint32_t entityID = scriptComponentSystem->getEntityID(i);

    lua_getglobal(L, script.table); 

    if (!lua_istable(L, -1)) {
      std::cerr << "Lua error: Global table '" << script.table << "' not found\n";
      lua_pop(L, 1);
      continue;
    }

    lua_getfield(L, -1, "update"); 

    if (!lua_isfunction(L, -1)) {
      std::cerr << "Lua error: 'update' not found in table '" << script.table << "'\n";
      lua_pop(L, 2);
      continue;
    }

    lua_pushvalue(L, -2); 
    lua_pushnumber(L, dt); 
    lua_pushinteger(L, entityID); 
    
    if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
      printf("ID: %d\n", entityID);
      std::cerr << "Lua error during '" << script.table << ":update': " 
                << lua_tostring(L, -1) << std::endl;
      lua_pop(L, 1);
    }

    lua_pop(L, 1); 
  }
}

// hot reloading logic
void ScriptSystem::reloadScript(const std::string& scriptPath, const char* table, uint32_t entityID){
  if (luaL_dofile(L, scriptPath.c_str()) != LUA_OK) {
    std::cerr << "Failed to reload script: " << scriptPath << "\n";
    std::cerr << lua_tostring(L, -1) << "\n";
    lua_pop(L, 1);
    return;
  }
  
  lua_getglobal(L, table);
  
  lua_pushinteger(L, static_cast<lua_Integer>(entityID));
  lua_setfield(L, -2, "id");

  lua_getfield(L, -1, "onReload");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 2); 
    return;
  }


  lua_pushvalue(L, -2); 
  if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
    std::cerr << "Lua error during '" << table << ":onReload': " << lua_tostring(L, -1) << std::endl;
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
}


ECS<Script>* ScriptSystem::getScriptComponentSystem(){
  return scriptComponentSystem;
}

void ScriptSystem::createScriptComponent(Script script, uint32_t entityID){
  this->scriptComponentSystem->createComponent(script, entityID);
  scriptTimestamps[std::string(script.script)] = std::filesystem::last_write_time(script.script);
}

void ScriptSystem::createSubscriptions(){
  eventManager->subscribe(EventType::COLLISION, onCollision);
  eventManager->subscribe(EventType::DESTROY, destroyEntity);
}

std::vector<Script>& ScriptSystem::getScripts(){
  return this->scripts;
}

void ScriptSystem::onCollision(const Event& event) {
  uint32_t targetID = event.entityA; 
  uint32_t sourceID = event.entityB;

  uint32_t index = ScriptSystem::Instance()->getScriptComponentSystem()->getEntityIndex(sourceID);
  Script& script = ScriptSystem::Instance()->getScripts()[index];

  lua_State* L = LuaState::Instance()->getLuaState();

  lua_getglobal(L, script.table); 
  if (!lua_istable(L, -1)) {
    std::cerr << "Lua error: Global table '" << script.table << "' not found\n";
    lua_pop(L, 1);
    return;
  }

  lua_getfield(L, -1, "onCollision");

  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 2); 
    return;
  }

  lua_pushvalue(L, -2);       
  lua_pushinteger(L, targetID); 
  lua_pushinteger(L, sourceID); 

  if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
    std::cerr << "Lua error during '" << script.table << ":onCollision': "
              << lua_tostring(L, -1) << std::endl;
    lua_pop(L, 1); 
  }

  lua_pop(L, 1);
}

void ScriptSystem::destroyEntity(const Event& event){
    ScriptSystem::Instance()->getScriptComponentSystem()->destroyComponent(event.entityA);
}
