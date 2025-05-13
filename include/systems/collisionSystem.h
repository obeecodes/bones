#pragma once
#include "../components/collider.h"
#include "../systems/quadtree.h"
#include "../systems/physicsSystem.h"
#include <vector>
#include "../core/ecs.h"
#include "../components/vec2.h"
#include "eventManager.h"
#include "raylib.h"
#include <cstdint>

class CollisionSystem{
public:
  static CollisionSystem* Instance();
  void update(float dt);
  void createColliderComponent(Collider collider, uint32_t entityID);
  std::vector<Collider>& getColliders();
  void createSubscriptions();
  ECS<Collider>* getColliderComponentSystem();
  void drawColliders();
  void toggleDrawColliders();

  void resolveWallCollision(Collider& colliderA, uint32_t entityA, Collider& colliderB, uint32_t entityB);
  
  bool isCollision(Rectangle a, Rectangle b);
  Rectangle getSweptRect(Rectangle& rect, int xVel, int yVel);
  Vec2 getCollisionNormal(Rectangle& a, Rectangle& b);
  Quadtree* quadtree;

  static void destroyEntity(const Event& event);

private:
  bool enableDrawColliders;
  static CollisionSystem* instance;
  ECS<Collider>* colliderComponentSystem;
  std::vector<Collider>& colliders;
  uint32_t& inactiveIndex;
  EventManager* eventManager;
  CollisionSystem();

};
