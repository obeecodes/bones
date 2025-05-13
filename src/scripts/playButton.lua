PlayButton = {}

function PlayButton:init(id)
    callScript("src/scripts/enemySpawner.lua")
    callScript("src/scripts/map.lua")

    self.x, self.y = 32*8, 192 
    self.w, self.h = 128, 32

    createUIComponent(
        function() self:onClick() end,
        self.x, self.y,
        self.w, self.h,
        204, 83, 83, 255,
        1,
        "Play", 0, 0, 20,
        0, 0, 0, 255,
        id
    )
    createAudioComponent(
        "CLICK",
        false,
        id
    )

end

function PlayButton:onClick()
    playSound(MainMenu.entities[2])
    MainMenu:cleanup()
    Player.play = true
    EnemySpawner:init()
    Map:init()
end