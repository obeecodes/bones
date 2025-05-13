Fire = {}

function Fire:init()
    callScript("src/scripts/fireball.lua")
    callScript("src/scripts/firenova.lua")
end

function Fire:createFireBall(x, y)
    local id = createEntity()
    FireBall:init(x, y, dx, dy, id);
end 

function Fire:unleashFireNova(x, y)
    local id = createEntity()
    FireNova:init(x, y, id)
end