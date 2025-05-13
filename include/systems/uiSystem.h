#pragma once

#include "../components/ui.h"
#include <vector>
#include "../core/ecs.h"
#include "eventManager.h"
#include <cstdint>

class UISystem{
public:
  static UISystem* Instance();
  void update(float dt);
  void createUIComponent(UI ui, uint32_t entityID);
  std::vector<UI>& getUIs();
  void createSubscriptions();

  void updateUIComponent(UI ui, uint32_t entityID);

  ECS<UI>* getUIComponentSystem();

  static void destroyEntity(const Event& event);
private:
  static UISystem* instance;
  ECS<UI>* uiComponentSystem;
  std::vector<UI>& uis;
  std::vector<UI> sortedUIs;
  uint32_t& inactiveIndex;
  EventManager* eventManager;
  UISystem();

};
