#pragma once

#include <cstdint>
#include <vector>
#include <queue>
#include <set>
#include "../systems/eventManager.h"


class EntityManager {
public:
  static EntityManager* Instance();
  void init();
  void initMainMenu();
  void initMap();
  void initEnemySpawner();
  void initPlayer();
  static uint32_t createEntity();

  std::vector<uint32_t> entityFlags;

private:
  std::queue<uint32_t> reuseableID;
  EventManager* eventManager;

  EntityManager();
  static EntityManager* instance;
  uint32_t entityID;
};