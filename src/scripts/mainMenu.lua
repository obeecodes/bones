MainMenu = {}

function MainMenu:init()
    if not Player then 
        callScript("src/scripts/player.lua")
        Player:init()
    end 

    callScript("src/scripts/playButton.lua")
    callScript("src/scripts/creditsButton.lua")

    self.id = createEntity()
    self.x, self.y = 32*8, 32
    self.w, self.h = 128, 64
    self.entities = {}
    self.entities[1] = self.id
    self.entities[2] = createEntity()
    self.entities[3] = createEntity()

    createUIComponent(
        function() end,
        self.x, self.y,
        self.w, self.h,
        78, 65, 55, 150,
        0,
        "Bones", 0, 0, 40,
        0, 0, 0, 255,
        self.id
    )

    createSpriteComponent(
        "BACKGROUND",
        0, 0, 640, 360,
        0, 0, 640, 360,
        0, 0,
        0.0,
        255,255, 255, 255,
        0,
        1, 0.0, false,
        self.id
    )

    PlayButton:init(self.entities[2])
    CreditsButton:init(self.entities[3])

end 

function MainMenu:cleanup()
    for i = 1, #self.entities do
        destroyEntity(self.entities[i])
    end
end

