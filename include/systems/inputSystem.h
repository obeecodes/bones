#pragma once 

#include <vector>
#include <memory>
#include <utility>
#include "../components/input.h"
#include "../components/physics.h"
#include "../components/sprite.h"
#include "../core/ecs.h"
#include "../components/vec2.h"
#include "eventManager.h"

enum mouse_buttons{
  LEFT = 0,
  MIDDLE = 1,
  RIGHT = 2,
};

class InputSystem{
public:
  static InputSystem* Instance();
  void update();
  void init();
  void clean();
  
  bool getMouseButtonState(int buttonNumber);
  Vec2& getMousePosition();
  void createInputComponent(Input input, uint32_t entityID);
  std::vector<Input>& getInputs();
  void setKeys(const std::vector<int>& keys);

  void createSubscriptions();

  ECS<Input>* getInputComponentSystem();

  static void destroyEntity(const Event& event);
 
  void updateMouseButtonStates();
  void updateKeyStates();
  void handleMainMenuInput(uint32_t entityId);
  void handleTransitionInput(uint32_t entityId);
  void handlePlayInput(uint32_t entityId);
  void handlePauseInput(uint32_t entityId);
  void handlePlayKeyRelease(uint32_t entityId);

private:
  InputSystem();

  static InputSystem* instance;

  std::vector<int> trackedKeys;
  std::unordered_map<int, KeyState> keyStates;
  std::unordered_map<int, bool> previousKeyStates;

  ECS<Input>* inputComponentSystem;
  std::vector<bool> mouseButtonStates;
  Vec2 mousePosition;
  Vec2 mouseDelta = Vec2{0.0f, 0.0f};
  float scrollDelta = 0.0f;
  std::vector<Input>& inputs;
  std::vector<Physics>& physics;
  std::vector<Sprite>& sprites; 
  uint32_t& inactiveIndex;
  EventManager* eventManager;
  
};
