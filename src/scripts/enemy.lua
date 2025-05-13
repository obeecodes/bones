Enemy = {}

function Enemy:init(x, y, id)
    maxHealth = 20
    self.soundCounter = 1;

    createHealthComponent(maxHealth, id)

    createPhysicsComponent(
        0,0,0,0,
       id
    )
 
    createColliderComponent(
     x, y, 8, 18,    -- rect (x, y, w, h)
     12, 10,            -- offset (x, y)
     1,               -- ColliderType (e.g., 0=STATIC, 1=DYNAMIC, etc.)
     id          -- entityID
     )
 
 
     createTransformComponent(
        x, 
        y,
        id
     )

     createSpriteComponent(
        "ENEMY",
        0, 0, 32, 32,        -- src
        x, y, 32, 32,        -- dest
        0, 0,                -- origin (default top-left)
        0.0,                 -- rotation
        255, 255, 255, 255,  -- tint (white)
        2,                   -- layer
        1, 0.0, true,         -- animation: 1 frame, loop
        id
    )

    createAudioComponent(
        "LASER",
        false,
        id
    )

    createScriptComponent("src/scripts/enemy.lua", "Enemy", id)

end 

function Enemy:update(dt, id)
    local x, y = getPosition(id)
    local speed = 50

    local dx, dy = Player.x - x, Player.y - y
    local distance = math.sqrt(dx * dx + dy * dy)

    local stopRadius = 20
    if distance < stopRadius and not Player.special then
        moveIntent(id, 0, 0) -- Stop movement
        if self.soundCounter < 0.5 then 
            playSound(id)
            self.soundCounter = 1
        end
        dealDamage(Player.id, id, 10)
        return
    end
    self.soundCounter = self.soundCounter - dt

    -- Normalize direction
    dx = dx / distance
    dy = dy / distance

    if not Player.special then
        moveIntent(id, dx * speed, dy * speed)
    end
end

function Enemy:onCollision(otherID)
    --
end