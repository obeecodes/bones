#pragma once
#include "../components/health.h"
#include <vector>
#include "../core/ecs.h"
#include "eventManager.h"
#include <cstdint>

class HealthSystem{
public:
  static HealthSystem* Instance();
  void update();
  void createHealthComponent(Health health, uint32_t entityID);
  static void applyDamage(const Event& event);
  std::vector<Health>& getHealths();
  void createSubscriptions();

  ECS<Health>* getHealthComponentSystem();

  static void destroyEntity(const Event& event);
private:
  static HealthSystem* instance;
  ECS<Health>* healthComponentSystem;
  std::vector<Health>& healths;
  uint32_t& inactiveIndex;
  EventManager* eventManager;
  HealthSystem();

};
