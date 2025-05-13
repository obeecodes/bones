#include "../../include/systems/uiSystem.h"
#include "../../include/core/luastate.h"

UISystem* UISystem::instance = nullptr;

UISystem* UISystem::Instance(){
  if(!instance){
    instance = new UISystem();
  }
  return instance;
}

UISystem::UISystem()
: uiComponentSystem(new ECS<UI>()),
  uis(uiComponentSystem->getComponents()),
  inactiveIndex(uiComponentSystem->getInactiveIndex()),
  eventManager(EventManager::Instance()){
    createSubscriptions();

}

void UISystem::update(float dt){
  sortedUIs.clear();
  // render ui components based on layer
  for (size_t i = 0; i < inactiveIndex; ++i) {
    const UI& ui = uis[i];

    auto it = std::lower_bound(
      sortedUIs.begin(),
      sortedUIs.end(),
      ui,
      [](const UI& a, const UI& b) {
          return a.layer < b.layer;
      }
    );
    sortedUIs.insert(it, ui);
  }

  // render ui components and call callback functions
  for (uint32_t i = 0; i < this->inactiveIndex; i++) {
    UI& ui = sortedUIs[i];

    Vector2 mouse = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mouse, ui.bounds);

    ui.hovered = isHovering;
    ui.pressed = isHovering && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    if (isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      LuaState::Instance()->callUICallback(ui.luaCallbackRef, uiComponentSystem->getEntityIndex(i));
    }

    DrawRectangleRec(ui.bounds, ui.color);

    if (!ui.label.empty()) {
      int fontSize = ui.fontSize;
      Vector2 textSize = MeasureTextEx(GetFontDefault(), ui.label.c_str(), fontSize, 1);
      Vector2 textPos = {
        ui.bounds.x + (ui.bounds.width - textSize.x) / 2 + ui.labelOffset.x,
        ui.bounds.y + (ui.bounds.height - textSize.y) / 2 + ui.labelOffset.y
      };
      DrawText(ui.label.c_str(), static_cast<int>(textPos.x), static_cast<int>(textPos.y), fontSize, ui.textColor);
    }
  }

  // render fps
  int fps = GetFPS();
  std::string fpsText = "FPS: " + std::to_string(fps);
  
  DrawText(fpsText.c_str(), 10, 10, 20, BLACK);
}

void UISystem::createUIComponent(UI ui, uint32_t entityID){
  this->uiComponentSystem->createComponent(ui, entityID);
}

void UISystem::updateUIComponent(UI ui, uint32_t entityID){
  uint32_t index = uiComponentSystem->getEntityIndex(entityID);
  uis[index] = ui;
}

ECS<UI>* UISystem::getUIComponentSystem(){
  return uiComponentSystem;
}

std::vector<UI>& UISystem::getUIs(){
  return uis;
}

void UISystem::createSubscriptions(){
  eventManager->subscribe(EventType::DESTROY, destroyEntity);
}

void UISystem::destroyEntity(const Event& event){
  UISystem::Instance()->getUIComponentSystem()->destroyComponent(event.entityA);
}
