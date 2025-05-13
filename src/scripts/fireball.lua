FireBall = {
    counter = {}
}

function FireBall:init(x, y, dx, dy, id) 

    self.counter[id] = 0

    local dx, dy = Player.dir[1], Player.dir[2]
    local speed = 200

    createPhysicsComponent(
        dx * speed,
        dy * speed,
        0,
        0,
        id
    )
 
    createColliderComponent(
     x, y, 8, 8,    -- rect (x, y, w, h)
     12, 12,            -- offset (x, y)
     4,               -- ColliderType (e.g., 0=STATIC, 1=DYNAMIC, etc.)
     id          -- entityID
     )
 
 
     createTransformComponent(
        x, 
        y,
        id
     )
     
     createSpriteComponent(
        "FIRE",             -- Texture name
    
        0, 0, 32, 32,       -- src rectangle (x, y, w, h)
        x, y, 32, 32,       -- dest rectangle (x, y, w, h)
    
        0, 0,             -- origin (x, y)
        0.0,                -- rotation
    
        255, 255, 255, 255, -- tint (r, g, b, a)
    
        2,                  -- layer
    
        4,                  -- frameCount
        1/16,            -- frameTime
        true,              -- looping
        id             
    )
    createScriptComponent("src/scripts/fireball.lua", "FireBall", id)
    createAudioComponent(
        "FIREBALL",
        true,
        id
    )

end 

function FireBall:update(dt, id)
    local dx, dy = getVelocity(id)
    moveIntent(id, dx, dy)
end

function FireBall:onCollision(otherID, sourceID)
    -- EnemySpawner stores id of all enemies, compare with that
    if otherID ~= 0 then
        dealDamage(otherID, 0, 10)
        self.counter[sourceID] = self.counter[sourceID] + 1
    end

    -- fireballs can damage 5 entities before being destroyed
    if self.counter[sourceID] >= 10 then 
        destroyEntity(sourceID)
    end
end