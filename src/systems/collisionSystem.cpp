#include "../../include/systems/collisionSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/eventManager.h"
#include "../../include/components/vec2.h"

#include "../../include/entities/entityManager.h"

CollisionSystem* CollisionSystem::instance = nullptr;

CollisionSystem* CollisionSystem::Instance(){
  if(!instance){
    instance = new CollisionSystem();
  }
  return instance;
}

CollisionSystem::CollisionSystem()
: colliderComponentSystem(new ECS<Collider>()),
  colliders(colliderComponentSystem->getComponents()),
  inactiveIndex(colliderComponentSystem->getInactiveIndex()),
  quadtree(new Quadtree(Rectangle{ 0, 0, 32*32, 32*32})),
  eventManager(EventManager::Instance()){
    enableDrawColliders = false;

  //subscribe to events
  createSubscriptions();
}

void CollisionSystem::update(float dt) {
  for (uint32_t i = 0; i < this->inactiveIndex; i++) {
    for (uint32_t j = i + 1; j < this->inactiveIndex; j++) {

      uint32_t entityA = colliderComponentSystem->getEntityID(i);
      uint32_t entityB = colliderComponentSystem->getEntityID(j);
      
      uint32_t entityIndexA = PhysicsSystem::Instance()->getPhysicsComponentSystem()->getEntityIndex(entityA);
      uint32_t entityIndexB = PhysicsSystem::Instance()->getPhysicsComponentSystem()->getEntityIndex(entityB);
      
      Collider& colliderA = this->colliders[i];
      Collider& colliderB = this->colliders[j];

      Transform2D& transformA = PhysicsSystem::Instance()->getTransforms()[entityIndexA];
      Transform2D& transformB = PhysicsSystem::Instance()->getTransforms()[entityIndexB];

      colliderA.rect.x = transformA.position.x + colliderA.offset.x;
      colliderA.rect.y = transformA.position.y + colliderA.offset.y;
      colliderB.rect.x = transformB.position.x + colliderB.offset.x;
      colliderB.rect.y = transformB.position.y + colliderB.offset.y;

      if (!isCollision(colliderA.rect, colliderB.rect)) continue;
      
      const ColliderType typeA = colliderA.type;
      const ColliderType typeB = colliderB.type;
      
      
      // implement in script later
      if (typeA == ColliderType::DYNAMIC && typeB == ColliderType::STATIC) {
        resolveWallCollision(colliderA, entityA, colliderB, entityB);

        continue;
      }
      if (typeB == ColliderType::DYNAMIC && typeA == ColliderType::STATIC) {
        resolveWallCollision(colliderB, entityB, colliderA, entityA);

        continue;
      }

      // implement in script later
      if (typeA == ColliderType::TRIGGER || typeB == ColliderType::TRIGGER) {
        uint32_t triggerEntity = (typeA == ColliderType::TRIGGER) ? entityA : entityB;
        uint32_t targetEntity  = (triggerEntity == entityA) ? entityB : entityA;

        EventManager::Instance()->publish(Event{
            EventType::COLLISION,
            targetEntity,    
            triggerEntity     
        });

        continue;
      }

      // implement in script
      if (typeA == ColliderType::DYNAMIC && typeB == ColliderType::DYNAMIC) {
        PhysicsSystem::Instance()->bounceEntities(entityA, entityB);
        continue;
      }
    }
  }
}

void CollisionSystem::resolveWallCollision(Collider& colliderA, uint32_t entityA, Collider& colliderB, uint32_t entityB) {
  uint32_t index = PhysicsSystem::Instance()->getPhysicsComponentSystem()->getEntityIndex(entityA);
  Transform2D& transform = PhysicsSystem::Instance()->getTransforms()[index];
  Vec2& velocityA = PhysicsSystem::Instance()->getPhysics()[index].velocity;
  Rectangle predicted = colliderA.rect;
  predicted.x += velocityA.x;
  predicted.y += velocityA.y;

  if (!isCollision(predicted, colliderB.rect)) return;

  Vec2 normal = getCollisionNormal(predicted, colliderB.rect);

  if (normal.x != 0) {
    velocityA.x = 0.0f;
  }
  if (normal.y != 0) {
    velocityA.y = 0.0f;
  }
  
  transform.position+= normal*3.5;
}

ECS<Collider>* CollisionSystem::getColliderComponentSystem(){
  return colliderComponentSystem;
}

bool CollisionSystem::isCollision(Rectangle a, Rectangle b){
  return CheckCollisionRecs(a, b);
}

Rectangle CollisionSystem::getSweptRect(Rectangle& rect, int xVel, int yVel) {
  Rectangle swept = rect;

  if (xVel < 0) {
      swept.x += xVel;
      swept.width -= xVel;
  } else {
      swept.width += xVel;
  }

  if (yVel < 0) {
      swept.y += yVel;
      swept.height -= yVel;
  } else {
      swept.height += yVel;
  }

  return swept;
}

void CollisionSystem::toggleDrawColliders(){
  enableDrawColliders = !enableDrawColliders;
}


void CollisionSystem::createColliderComponent(Collider collider, uint32_t entityID){
  this->colliderComponentSystem->createComponent(collider, entityID);

  this->quadtree->insert(collider);
}

std::vector<Collider>& CollisionSystem::getColliders(){
  return this->colliders;
}

Vec2 CollisionSystem::getCollisionNormal(Rectangle& a, Rectangle& b) {
  int overlapLeft   = (a.x + a.width)  - b.x;
  int overlapRight  = (b.x + b.width)  - a.x;
  int overlapTop    = (a.y + a.height) - b.y;
  int overlapBottom = (b.y + b.height) - a.y;

  int minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

  if (minOverlap == overlapTop) {
      return Vec2{0, -1}; 
  } else if (minOverlap == overlapBottom) {
      return Vec2{0, 1}; 
  } else if (minOverlap == overlapLeft) {
      return Vec2{-1, 0}; 
  } else if (minOverlap == overlapRight) {
      return Vec2{1, 0}; 
  }

  return Vec2{0, 0};
}


void CollisionSystem::createSubscriptions(){
  eventManager->subscribe(EventType::DESTROY, destroyEntity);
}

void CollisionSystem::destroyEntity(const Event& event){
    CollisionSystem::Instance()->getColliderComponentSystem()->destroyComponent(event.entityA);
}

void CollisionSystem::drawColliders() {
  if(!enableDrawColliders)return;

  for (uint32_t i = 0; i < this->inactiveIndex; i++) {
      uint32_t entityID = colliderComponentSystem->getEntityID(i);
      uint32_t physicsIndex = PhysicsSystem::Instance()->getPhysicsComponentSystem()->getEntityIndex(entityID);

      Collider& collider = this->colliders[i];
      Transform2D& transform = PhysicsSystem::Instance()->getTransforms()[physicsIndex];

      // update collider rect position
      Rectangle worldRect = collider.rect;
      worldRect.x = transform.position.x + collider.offset.x;
      worldRect.y = transform.position.y + collider.offset.y;

      // set color based on collider type
      Color color = WHITE;
      switch (collider.type) {
          case ColliderType::STATIC:    color = GRAY;   break;
          case ColliderType::DYNAMIC:   color = GREEN;  break;
          case ColliderType::KINEMATIC: color = BLUE;   break;
          case ColliderType::TRIGGER:   color = RED;    break;
          case ColliderType::AUTOMATIC: color = YELLOW; break;
      }

      DrawRectangleLinesEx(worldRect, 1.0f, color);
  }
}
