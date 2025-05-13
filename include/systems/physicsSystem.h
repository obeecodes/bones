#pragma once

#include "../components/physics.h"
#include "../components/transform2d.h"
#include "../components/vec2.h"
#include <vector>
#include "../core/ecs.h"
#include "eventManager.h"
#include <cstdint>

class PhysicsSystem{
public:
  static PhysicsSystem* Instance();
  void update(float dt);
  void createPhysicsComponent(Physics physics, uint32_t entityID);
  void createTransformComponent(Transform2D transform, uint32_t entityID);
  void createSubscriptions();
  uint32_t getInactiveIndex();


  // handle this in scripts
  void cancelVelocity(uint32_t entityID);
  void bounceEntities(uint32_t entityA, uint32_t entityB);
  

  std::vector<Transform2D>& getTransforms();
  std::vector<Physics>& getPhysics();

  ECS<Physics>* getPhysicsComponentSystem();
  ECS<Transform2D>* getTransformComponentSystem();

  static void moveEntity(const Event& event);

  static void destroyEntity(const Event& event);


private:
  static PhysicsSystem* instance;
  ECS<Physics>* physicsComponentSystem;
  std::vector<Physics>& physics;
  ECS<Transform2D>* transformComponentSystem;
  std::vector<Transform2D>& transforms;
  uint32_t& inactiveIndex;

  PhysicsSystem();
  EventManager* eventManager;

};
