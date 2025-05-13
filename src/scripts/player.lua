Player = {}

function Player:init()
    self.play = false
    self.special = false
    self.id = createEntity()
    callScript("src/scripts/fire.lua")
    callScript("src/scripts/healthBar.lua")
    callScript("src/scripts/gameOver.lua")
    Fire:init()

    self.x = 0
    self.y = 0
    self.dir = {0, 1}
    self.speed = 150
    self.fireCooldown = 0
    x, y = 8 *32, 4 *32
    self.maxHealth = 10000
    self.currentHealth = 10000
    self.blastCounter = 0

    HealthBar:init(self.maxHealth)

    createHealthComponent(self.maxHealth, self.id)

    createInputComponent(self.id)

   createPhysicsComponent(
    0,0,0,0,
        self.id
   )

   createColliderComponent(
    x, y, 8, 18,    -- rect (x, y, w, h)
    12, 10,            -- offset (x, y)
    1,               -- ColliderType (e.g., 0=STATIC, 1=DYNAMIC, etc.)
    self.id          -- entityID
    )


    createTransformComponent(
        x, 
        y,
        self.id
    )

    createSpriteComponent(
    "PLAYER",
    0, 32, 32, 32,
    x, y, 32, 32,
    0, 0,
    0.0,
    255, 255, 255, 255,
    2,
    1, 0.0, false,
    self.id
    )
    createScriptComponent("src/scripts/player.lua", "Player", self.id)

    createAudioComponent(
        "EXPLOSION",
        true,
        self.id
    )

end 

function Player:update(dt, id)
    self.x, self.y = getPosition(self.id);
    self.fireCooldown = self.fireCooldown - dt
    self.currentHealth = getHealth(Player.id)

    local k = InputBindings.movement
    if isKeyDown(k.up) then
        self.dir = {0, -1}
        moveIntent(self.id, 0, -self.speed)
    elseif isKeyDown(k.left) then
        self.dir = {-1, 0}
        moveIntent(self.id, -self.speed, 0)
    elseif isKeyDown(k.down) then
        self.dir = {0, 1}
        moveIntent(self.id, 0, self.speed)
    elseif isKeyDown(k.right) then
        self.dir = {1, 0}
        moveIntent(self.id, self.speed, 0)
    end

    k = InputBindings.actions
    if isKeyDown(k.attack) and self.fireCooldown <= 0 and not Player.special then
        Fire:createFireBall(self.x, self.y)
        self.fireCooldown = 1/2
    end 
    
    if (isKeyDown(k.special) and self.fireCooldown <= 0 and self.currentHealth < 2000 or self.currentHealth < 1000) and not Player.special then 
        Player.special = true
        Player.speed = 0
        Fire:unleashFireNova(self.x, self.y)
        
        self.fireCooldown = 1/2
    end
    
    if Player.special then 
        self.blastCounter = self.blastCounter + dt 
        playSound(self.id)
        if self.blastCounter > 3 then 
            Player:destroyAllEnemies()
        end
    end
        
    k = InputBindings.global
        
    if isKeyPressed(k.drawColliders) then 
        toggleDrawColliders()
    end


    HealthBar:update(dt)
end

function Player:onReload()
    self.x, self.y = getPosition(0) 
    self.currentHealth = getHealth(0)
    self.dir = {0,-1}
    self.speed = 400
    self.fireCooldown = 0
    x, y = 8 *32, 4 *32
end

function Player:destroyAllEnemies()
    for i = 1, #EnemySpawner.enemies do
        destroyEntity(EnemySpawner.enemies[i])
    end 
    Player:cleanup()
end

function Player:cleanup()
    HealthBar:cleanup()
    Map:cleanup()
    destroyEntity(self.id)
    GameOver:init()
end
