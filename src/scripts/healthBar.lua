HealthBar = {}

function HealthBar:init(max)
    self.max = max
    self.id = {}

    self.x, self.y = 20, 40
    self.w, self.h = 180, 20
    self.padding = 2

    self.id[1] = createEntity()
    createUIComponent(
        function() end,
        self.x - self.padding, self.y - self.padding,
        self.w + self.padding * 2, self.h + self.padding * 2,
        0, 0, 0, 0,
        0,
        "", 0, 0, 20,
        0,0,0,0,
        self.id[1]
    )

    
    self.id[2] = createEntity()
    createUIComponent(
        function()  end,
        self.x, self.y,
        self.w, self.h,
        255, 0, 0, 0,
        1,
        "", 4, 2, 20,
        0,0,0,0,
        self.id[2]
    )
end

function HealthBar:update(dt)
    if not Player.play then
        return 
    end
    updateUIComponent(
        function() end,
        self.x - self.padding, self.y - self.padding,
        self.w + self.padding * 2, self.h + self.padding * 2,
        0, 0, 0, 255,
        0,
        "", 0, 0, 20,
        0,0,0,0,
        self.id[1]
    )
    
    local health = getHealth(Player.id)
    local t = health / self.max

    local r = 255
    local g = math.floor(255 * (1 - t))
    local b = math.floor(255 * (1 - t))

    local width = math.floor(self.w * t)
    
    updateUIComponent(
        function() end,
        self.x, self.y, width, self.h,
        r, g, b, 255,
        1,
        "", 0, 0, 20,
        0,0,0,0,
        self.id[2]
    )
end

function HealthBar:cleanup()
    destroyEntity(self.id[1])
    destroyEntity(self.id[2])
end

function HealthBar:onReload()

end