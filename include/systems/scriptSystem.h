#pragma once
#include "../components/script.h"
#include <vector>
#include "../core/ecs.h"
#include "eventManager.h"
#include <cstdint>
#include "../core/luastate.h"
#include <unordered_map>
#include <filesystem>
#include <set>
#include <string>


class ScriptSystem{
public:
  static ScriptSystem* Instance();
  void update(float dt);
  void createScriptComponent(Script script, uint32_t entityID);
  void createSubscriptions();
  void reloadScript(const std::string& scriptPath, const char* table, uint32_t entityID);
  static void onCollision(const Event& event);
  ECS<Script>* getScriptComponentSystem();
  std::vector<Script>& getScripts();

  static void destroyEntity(const Event& event);

private:
  static ScriptSystem* instance;
  lua_State* L;std::unordered_map<std::string, std::filesystem::file_time_type> scriptTimestamps;
  std::set<std::string> modifiedScripts;
  ECS<Script>* scriptComponentSystem;
  std::vector<Script>& scripts;
  uint32_t& inactiveIndex;
  EventManager* eventManager;
  ScriptSystem();

};
