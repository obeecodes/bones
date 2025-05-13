#include "../../include/systems/spriteSystem.h"
#include "../../include/systems/collisionSystem.h"
#include "../../include/systems/physicsSystem.h"
#include "../../include/systems/inputSystem.h"
#include "../../include/entities/entityManager.h"
#include "../../include/core/luastate.h"

#include <algorithm>
#include <iostream>

SpriteSystem* SpriteSystem::instance = nullptr;

SpriteSystem* SpriteSystem::Instance() {
    if (!instance) {
        instance = new SpriteSystem();
    }
    return instance;
}

SpriteSystem::SpriteSystem()
: spriteComponentSystem(new ECS<Sprite>()),
  sprites(spriteComponentSystem->getComponents()),
  inactiveIndex(spriteComponentSystem->getInactiveIndex()),
  transforms(PhysicsSystem::Instance()->getTransforms()),
  eventManager(EventManager::Instance()) {
    createSubscriptions();
    camera = {0};
    this->camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

}

void SpriteSystem::init() {
    LuaState::Instance()->callScript("src/scripts/textures.lua");
}

void SpriteSystem::addTexture(const std::string& texture, const char* file) {
    SpriteSystem::Instance()->textures[texture] = LoadTexture(file);
}

void SpriteSystem::createSpriteComponent(const std::string& texture, Rectangle src, Rectangle dest, Layer layer, uint32_t entityID) {
    spriteComponentSystem->createComponent(Sprite{textures[texture], src, dest, layer}, entityID);
}

void SpriteSystem::createSpriteComponent(
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
) {
    Sprite sprite(texture, src, dst, origin, rotation, tint, layer, frameCount, frameTime, looping);
    createSpriteComponent(sprite, entityID);
}

void SpriteSystem::createSpriteComponent(const std::string& texture, Rectangle src, Rectangle dest, uint32_t entityID) {
    spriteComponentSystem->createComponent(Sprite{textures[texture], src, dest}, entityID);
}

void SpriteSystem::createSpriteComponent(Sprite sprite, uint32_t entityID) {
    spriteComponentSystem->createComponent(sprite, entityID);
}

void SpriteSystem::destroySpriteComponent(uint32_t entityID) {
    spriteComponentSystem->destroyComponent(entityID);
}

void SpriteSystem::update(float dt) {
    updateCamera();
    BeginMode2D(this->camera);
    updateAnimation(dt);

    sortedSprites.clear();

    // sort sprites based on layer
    for (size_t i = 0; i < inactiveIndex; ++i) {
        const Sprite& sprite = sprites[i];
        
        auto it = std::lower_bound(
            sortedSprites.begin(),
            sortedSprites.end(),
            sprite,
            [](const Sprite& a, const Sprite& b) {
                if (a.layer != b.layer)
                    return a.layer < b.layer;
                return a.dest.y < b.dest.y;
            });
            
        sortedSprites.insert(it, sprite);
    }
        
    for (uint32_t i = 0; i < inactiveIndex; i++) {
        const Sprite& sprite = sortedSprites[i];
        
        Rectangle dest = {
            sprite.dest.x,// - camera.target.x,
            sprite.dest.y,// - camera.target.y,
            sprite.dest.width,
            sprite.dest.height
        };
        
        DrawTexturePro(
            sprite.texture,
            sprite.src,
            sprite.dest,
            sprite.origin,
            sprite.rotation,
            sprite.tint
        );
    }

    CollisionSystem::Instance()->drawColliders();
    EndMode2D();
}

void SpriteSystem::updateCamera(){
    float minX = 0.0f;
    float maxX = 512;
    float minY = 0.0f;
    float maxY = 256;
    sprites[0].dest.x = transforms[0].position.x;
    sprites[0].dest.y = transforms[0].position.y;

    camera.target = (Vector2){ transforms[0].position.x + sprites[0].src.width/2, transforms[0].position.y + sprites[0].src.height/2};

    if (camera.target.x < minX + 160) camera.target.x = minX + 160;
    if (camera.target.x > maxX - 160) camera.target.x = maxX - 160;
    if (camera.target.y < minY + 90 ) camera.target.y = minY + 90;
    if (camera.target.y > maxY - 90) camera.target.y = maxY - 90;

}

std::vector<Sprite>& SpriteSystem::getSprites() {
    return sprites;
}

ECS<Sprite>* SpriteSystem::getSpriteComponentSystem() {
    return spriteComponentSystem;
}

Texture2D SpriteSystem::getTexture(const std::string& texture) {
    return textures[texture];
}

uint32_t SpriteSystem::getInactiveIndex() {
    return inactiveIndex;
}

void SpriteSystem::updateAnimation(float dt) {

    for(uint32_t i = 0; i < inactiveIndex; i++){
        Sprite& sprite = sprites[i];
        if (sprite.frameCount <= 1) continue;
    
        sprite.timeAccumulator += dt;
        if (sprite.timeAccumulator >= sprite.frameTime) {
            sprite.timeAccumulator -= sprite.frameTime;
            sprite.currentFrame++;
    
            if (sprite.currentFrame >= sprite.frameCount) {
                sprite.currentFrame = sprite.looping ? 0 : sprite.frameCount - 1;
            }
    
            sprite.src.x = sprite.src.width * sprite.currentFrame;
        }

    }
}

void SpriteSystem::createSubscriptions() {
    eventManager->subscribe(EventType::MOVE, moveEntity);
    eventManager->subscribe(EventType::DESTROY, destroyEntity);
}

void SpriteSystem::moveEntity(const Event& event) {
    SpriteSystem* spriteSystem = SpriteSystem::Instance();

    uint32_t entityIndex = spriteSystem->spriteComponentSystem->getEntityIndex(event.entityA);
    Sprite& sprite = spriteSystem->sprites[entityIndex];

    const Vec2& direction = std::get<Vec2>(event.data);


    entityIndex = PhysicsSystem::Instance()->getPhysicsComponentSystem()->getEntityIndex(event.entityA);
    Transform2D& transform = PhysicsSystem::Instance()->getTransforms()[entityIndex];
    sprite.dest.x = transform.position.x;
    sprite.dest.y = transform.position.y;

    if(sprite.frameCount == 1){
        if (direction.x < 0) {
            sprite.src.y = 2 * sprite.src.height; // Left
        } else if (direction.x > 0) {
            sprite.src.y = 3 * sprite.src.height; // Right
        } else if (direction.y < 0) {
            sprite.src.y = 0; // Up
        } else if (direction.y > 0) {
            sprite.src.y = 1 * sprite.src.height; // Down
        }
    }else{
        if (direction.x < 0) {
            sprite.dest.x += 40;
            sprite.dest.y += 16;
            sprite.rotation = 45.0f + 90; // Left
        } else if (direction.x > 0) {
            sprite.dest.x -= 8;
            sprite.dest.y += 16;
            sprite.rotation = 45.0f + 90 + 90 + 90; // Right
        } else if (direction.y < 0) {
            sprite.dest.x += 16;
            sprite.dest.y += 40;
            sprite.rotation = 45.0f + 90 + 90; // Up
        } else if (direction.y > 0) {
            sprite.dest.x += 16;
            sprite.dest.y -= 8;
            sprite.rotation = 45.0f; // Down
        }
    }
}

void SpriteSystem::destroyEntity(const Event& event){
    SpriteSystem::Instance()->getSpriteComponentSystem()->destroyComponent(event.entityA);
}

