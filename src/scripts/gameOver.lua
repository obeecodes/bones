GameOver = {}

function GameOver:init()

    self.id = {}
    self.id[1] = createEntity()
    self.id[2] = createEntity()
    callScript("src/scripts/timer.lua")
    self.fontSize = 14
    self.timer = 0
    self.charDelay = 0.03
    self.currentIndex = 0
    self.x, self.y = 0, 0
    self.w, self.h = 640, 360
    self.currentText = ""
    self.text = 
    "\
    I started this project to study data-oriented design and to change the way I\
    view data. I now understand the most important aspects.\
    This project is a testament to that.\
    \
    No Unity. No Unreal. Just raw code.\
    \
    A simple and unique Entity Component System made using C++ and Lua.\
    \
    Besides the FPS in the top right, everything is a component created \
    using a lua script and iterated over in a system in C++.\
    \
    This is only a 2D renderer, so I will need to create a 3D renderer next. \
    This time, one that is written in C++ instead of Python.\
    \
    - Obinsonne Servius\n\n\
    "

    createUIComponent(
        function() end,
        self.x, self.y,
        self.w, self.h,
        105, 145, 125, 255,
        1,
        "", 0, 0,
        0, 0, 0, 255, self.fontSize,
        self.id[1]
    )

    createUIComponent(
        function() end,
        self.x, self.y,
        self.w, self.h,
        105, 145, 125, 255,
        1,
        self.currentText, 0, 0, self.fontSize,
        0, 0, 0, 255,
        self.id[2]
    )

    createScriptComponent("src/scripts/gameOver.lua", "GameOver", self.id[2])

    Timer:init()

end

function GameOver:update(dt, id)
    updateUIComponent(
        function() end,
        self.x, self.y,
        self.w, self.h,
        105, 145, 125, 255,
        1,
        "", 0, 0, self.fontSize,
        0, 0, 0, 255,
        self.id[1]
    )
    self.timer = self.timer + dt

    while self.timer >= self.charDelay and self.currentIndex < #self.text do
        self.timer = self.timer - self.charDelay
        self.currentIndex = self.currentIndex + 1
        self.currentText = string.sub(self.text, 1, self.currentIndex)
    end

    updateUIComponent(
        function() end,
        self.x, self.y,
        self.w-200, self.h,
        105, 145, 125, 0,
        1,
        self.currentText, 96, 32, self.fontSize,
        0, 0, 0, 255,
        self.id[2]
    )
end

function GameOver:cleanup()
    for i = 1, #self.id do 
        destroyEntity(self.id[i])
    end 
end