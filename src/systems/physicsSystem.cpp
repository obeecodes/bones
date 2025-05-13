#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/spriteSystem.h"
#include "../../include/systems/collisionSystem.h"

#include <iostream>

PhysicsSystem* PhysicsSystem::instance = nullptr;

PhysicsSystem* PhysicsSystem::Instance(){
  if(!instance){
    instance = new PhysicsSystem();
  }
  return instance;
}

PhysicsSystem::PhysicsSystem()
: physicsComponentSystem(new ECS<Physics>()),
  physics(physicsComponentSystem->getComponents()),
  inactiveIndex(physicsComponentSystem->getInactiveIndex()),
  transformComponentSystem(new ECS<Transform2D>()),
  transforms(transformComponentSystem->getComponents()),
  eventManager(EventManager::Instance()){
    createSubscriptions();
    
}

void PhysicsSystem::update(float dt){

  for(uint32_t i = 0; i < this->inactiveIndex; i++){
    Physics& physics = this->physics[i];
    physics.velocity = physics.velocity * dt;

    Transform2D& transform = this->transforms[i];

    transforms[i].position += physics.velocity;

    physics.velocity = Vec2{0,0};
  }
}


void PhysicsSystem::createPhysicsComponent(Physics physics, uint32_t entityID){
  this->physicsComponentSystem->createComponent(physics, entityID);
}

void PhysicsSystem::createTransformComponent(Transform2D transform, uint32_t entityID){
  this->transformComponentSystem->createComponent(transform, entityID);
}

std::vector<Transform2D>& PhysicsSystem::getTransforms(){
  return this->transforms;
}

std::vector<Physics>& PhysicsSystem::getPhysics(){
  return this->physics;
}

ECS<Physics>* PhysicsSystem::getPhysicsComponentSystem(){
  return this->physicsComponentSystem;
}

ECS<Transform2D>* PhysicsSystem::getTransformComponentSystem(){
  return this->transformComponentSystem;
}

void PhysicsSystem::createSubscriptions(){
  eventManager->subscribe(EventType::MOVE, moveEntity);
  eventManager->subscribe(EventType::DESTROY, destroyEntity);
}

void PhysicsSystem::moveEntity(const Event& event) {
  PhysicsSystem* physicsSystem = PhysicsSystem::Instance();

  uint32_t entityIndex = physicsSystem->physicsComponentSystem->getEntityIndex(event.entityA);
  Physics& physics = physicsSystem->physics[entityIndex];

  entityIndex = physicsSystem->transformComponentSystem->getEntityIndex(event.entityA);
  Transform2D& transform = physicsSystem->transforms[entityIndex];

  const Vec2& direction = std::get<Vec2>(event.data);

  physics.velocity += direction;

}

uint32_t PhysicsSystem::getInactiveIndex(){
  return inactiveIndex;
}

void PhysicsSystem::cancelVelocity(uint32_t entityID) {
  uint32_t index = physicsComponentSystem->getEntityIndex(entityID);
  if (index >= inactiveIndex) return;

  physics[index].velocity = Vec2{0, 0};
}

void PhysicsSystem::bounceEntities(uint32_t entityA, uint32_t entityB) {
  uint32_t indexA = physicsComponentSystem->getEntityIndex(entityA);
  uint32_t indexB = physicsComponentSystem->getEntityIndex(entityB);
  if (indexA >= inactiveIndex || indexB >= inactiveIndex) return;

  Transform2D& tA = transforms[indexA];
  Transform2D& tB = transforms[indexB];

  Vec2 delta = tA.position - tB.position;
  float dist = delta.length();

  if (dist == 0.0f) {
      // prevent division by zero
      delta = Vec2{1, 0};
      dist = 1.0f;
  }

  Vec2 normal = delta / dist;

  // bounce entities
  float bounceStrength = 30.0f;
  physics[indexA].velocity = normal * bounceStrength;
  physics[indexB].velocity = normal * -bounceStrength;
}

void PhysicsSystem::destroyEntity(const Event& event){
    PhysicsSystem::Instance()->getPhysicsComponentSystem()->destroyComponent(event.entityA);
    PhysicsSystem::Instance()->getTransformComponentSystem()->destroyComponent(event.entityA);
}