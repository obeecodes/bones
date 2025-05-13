Timer = {}

function Timer:init()
    self.id = createEntity()
    self.x, self.y = 0, 0
    self.w, self.h = 640, 360
    self.timer = 30
    self.fontSize = 100

    createUIComponent(
        function() end,
        self.x, self.y,
        self.w, self.h,
        0, 0, 0, 0,
        0,
        tostring(math.floor(self.timer)), 0, 0, self.fontSize,
        0, 0, 0, 255,
        self.id
    )

    createScriptComponent("src/scripts/timer.lua", "Timer", self.id)

end

function Timer:update(dt)
    self.timer = self.timer - dt 

    updateUIComponent(
        function() end,
        self.x, self.y,
        self.w, self.h,
        0, 0, 0, 0,
        2,
        tostring(math.floor(self.timer)), 0, 0, self.fontSize,
        0, 0, 0, 25,
        self.id
    )

    if self.timer <= 1 then 
        self:cleanup()
    end

end 

function Timer:cleanup()
    GameOver:cleanup()
    destroyEntity(self.id)
    closeEngine()
end


