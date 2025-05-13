
#pragma once

#include "raylib.h"
#include "../components/sprite.h"
#include "../components/transform2d.h"
#include "../components/input.h"
#include "../core/ecs.h"
#include "eventManager.h"
#include <vector>
#include <unordered_map>
#include <cstdint>

class SpriteSystem {
public:
    static SpriteSystem* Instance();
    void init();
    static void addTexture(const std::string& texture, const char* file);
    void update(float dt);
    void updateAnimation(float dt);
    void createSpriteComponent(
        Texture2D texture,
        Rectangle src,
        Rectangle dst,
        Vector2 origin,
        float rotation,
        Color tint,
        Layer layer,
        int frameCount,
        float frameTime,
        bool looping,
        uint32_t entityID
    );
    void createSpriteComponent(const std::string& entity, Rectangle src, Rectangle dst, Layer layer, uint32_t entityID);
    void createSpriteComponent(const std::string& entity, Rectangle src, Rectangle dst, uint32_t entityID);
    void createSpriteComponent(Sprite sprite, uint32_t entityID);
    void destroySpriteComponent(uint32_t entityID);
    std::vector<Sprite>& getSprites();
    ECS<Sprite>* getSpriteComponentSystem();
    Texture2D getTexture(const std::string& texture);
    void createSubscriptions();
    uint32_t getInactiveIndex();

    void updateCamera();

    static void moveEntity(const Event& event);
    static void destroyEntity(const Event& event);

private:
    Camera2D camera;
    static SpriteSystem* instance;
    std::unordered_map<std::string, Texture2D> textures;
    std::vector<Transform2D>& transforms;
    ECS<Sprite>* spriteComponentSystem;
    std::vector<Sprite>& sprites;
    std::vector<Sprite> sortedSprites;
    uint32_t& inactiveIndex;
    SpriteSystem();
    EventManager* eventManager;
};
