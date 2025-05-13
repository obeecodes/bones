BackButton = {}

function BackButton:init(callback, x, y, id)
    self.id = id
    self.callback = callback
    self.x, self.y = x, y
    self.w, self.h = 128, 32
    self.text = "BACK"

    createUIComponent(
        function() self:onClick() end,
        self.x, self.y,
        self.w, self.h,
        78, 65, 55, 255,
        1,
        self.text, 0, 0, 20,
        0, 0, 0, 255,
        id
    )

    createAudioComponent(
        "CLICK",
        false,
        id
    )

end 

function BackButton:onClick()
    playSound(self.id)
    self.callback()
end