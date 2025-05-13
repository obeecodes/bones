#include "../../include/systems/healthSystem.h"

HealthSystem* HealthSystem::instance = nullptr;

HealthSystem* HealthSystem::Instance(){
  if(!instance){
    instance = new HealthSystem();
  }
  return instance;
}

HealthSystem::HealthSystem()
: healthComponentSystem(new ECS<Health>()),
  healths(healthComponentSystem->getComponents()),
  inactiveIndex(healthComponentSystem->getInactiveIndex()),
  eventManager(EventManager::Instance()){
    createSubscriptions();

}

void HealthSystem::update(){
  for(uint32_t i = 0; i < this->inactiveIndex; i++){
    if(healths[i].current == 0){
    }
  }
}

void HealthSystem::createHealthComponent(Health health, uint32_t entityID){
  this->healthComponentSystem->createComponent(health, entityID);
}

ECS<Health>* HealthSystem::getHealthComponentSystem(){
  return healthComponentSystem;
}

std::vector<Health>& HealthSystem::getHealths(){
  return healths;
}

void HealthSystem::createSubscriptions(){
  eventManager->subscribe(EventType::DAMAGE, applyDamage);
  eventManager->subscribe(EventType::DESTROY, destroyEntity);
}

void HealthSystem::applyDamage(const Event& event){
  uint32_t index = HealthSystem::Instance()->getHealthComponentSystem()->getEntityIndex(event.entityA);
  if(index == UINT32_MAX){
    return; 
  }
  Health& health = HealthSystem::Instance()->getHealths()[index];
  uint32_t damage = std::get<uint32_t>(event.data);

  if(health.current <= damage){
    health.current = 0;
  }else{
    health.current -= damage;
  }

  if(health.current == 0){
    EventManager::Instance()->publish(Event{EventType::DESTROY, event.entityA});
  }

}

void HealthSystem::destroyEntity(const Event& event){
    HealthSystem::Instance()->getHealthComponentSystem()->destroyComponent(event.entityA);
}
