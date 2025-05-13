#include "../../include/core/engine.h"
#include "../../include/systems/inputSystem.h"
#include "../../include/systems/audioSystem.h"
#include "../../include/systems/collisionSystem.h"
#include "../../include/systems/healthSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/entities/entityManager.h"
#include "../../include/systems/scriptSystem.h"
#include "../../include/systems/spriteSystem.h"
#include "../../include/core/luastate.h"

#include <iostream>
#include <thread>

const int screenWidth = 640;
const int screenHeight = 360;

Engine* Engine::instance = nullptr;

Engine* Engine::Instance(){
  if(!instance){
    instance = new Engine();
  }
  return instance;
}

Engine::Engine(){
  this->running = true;

  InitWindow(screenWidth, screenHeight, "Bones");

  LuaState::Instance()->init();
  InputSystem::Instance()->init();
  SpriteSystem::Instance()->init();
  EntityManager::Instance()->init();
  AudioSystem::Instance()->init();
  
  SetTargetFPS(61);
  

  this->counter = 0;

  this->fps = 60;
  this->cycleTime = std::chrono::duration<float>{1.0f / fps};
  this->elapsedSeconds = std::chrono::duration<float>{0.0f};
  this->accumulatedSeconds = 0.0f;
  this->systemTimerStart = std::chrono::high_resolution_clock::now();

}

Engine::~Engine(){
}

bool Engine::getRunning(){
  return this->running;
}

void Engine::setRunning(bool running){
  this->running = running;
}

void Engine::run(){
  while (running && !WindowShouldClose()) {
    systemTimerEnd = std::chrono::high_resolution_clock::now();
    elapsedSeconds = systemTimerEnd - systemTimerStart;
    accumulatedSeconds += elapsedSeconds.count();
    systemTimerStart = systemTimerEnd;

    while (accumulatedSeconds >= cycleTime.count()) {
      update();
      accumulatedSeconds -= cycleTime.count();
    }

    render();  
    this->counter++;
  }

  CloseWindow();
}


void Engine::update(){
  float dt = GetFrameTime();

  InputSystem::Instance()->update();
  EventManager::Instance()->processEvents();
  ScriptSystem::Instance()->update(dt);
  PhysicsSystem::Instance()->update(dt);
  CollisionSystem::Instance()->update(dt);
  AudioSystem::Instance()->update();
  EventManager::Instance()->processEvents();
}

void Engine::render(){
  BeginDrawing();
  ClearBackground(WHITE);
  
  SpriteSystem::Instance()->update(GetFrameTime());
  UISystem::Instance()->update(GetFrameTime());
  
  EndDrawing();
}