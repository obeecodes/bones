EnemySpawner = {
    enemies = {},
    timer = 0,
    spawnPoints = {},
    spawn = true
}

function EnemySpawner:init()
    self.id = createEntity()
    self.counter = 0
    callScript("src/scripts/enemy.lua")
    createScriptComponent("src/scripts/enemySpawner.lua", "EnemySpawner", self.id)
    -- set up spawn points
    for i = 6, 9 do
        table.insert(self.spawnPoints, { x = i * 32, y = 0 })
        table.insert(self.spawnPoints, { x = i * 32, y = 8 * 32 })
    end
    for i = 2, 6, 4 do
        table.insert(self.spawnPoints, { x = 0,      y = i * 32 })       
        table.insert(self.spawnPoints, { x = 16 * 32,  y = i * 32 })       
    end
end

function EnemySpawner:spawnEnemy(x, y)
    local id = createEntity()
    Enemy:init(x, y, id)
    table.insert(self.enemies, id)
end

function EnemySpawner:update(dt)
    -- spawn logic
    self.timer = self.timer - dt
    if self.timer <= 0 and self.counter < 2000 and self.spawn and not Player.special then
        -- pick random spawn point
        local point = self.spawnPoints[math.random(#self.spawnPoints)]
        self:spawnEnemy(point.x, point.y)
        self.spawnDelay = math.max(0.1, (self.spawnDelay or 0.5) * 0.98)
        self.timer = self.spawnDelay
        self.counter = self.counter + 1
    end
end