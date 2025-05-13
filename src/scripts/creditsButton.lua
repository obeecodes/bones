CreditsButton = {}

function CreditsButton:init(id)
    callScript("src/scripts/credits.lua")
    self.x, self.y = 32*8, 192 + 32 + 16
    self.w, self.h = 128, 32

    createUIComponent(
        function() self:onClick() end,
        self.x, self.y,
        self.w, self.h,
        246, 129, 97, 255,
        1,
        "Credits", 0, 0, 20,
        0, 0, 0, 255,
        id
    )

    createAudioComponent(
        "CLICK",
        false,
        id
    )

end

function CreditsButton:onClick()
    playSound(MainMenu.entities[3])
    MainMenu:cleanup()
    Credits:init()
end