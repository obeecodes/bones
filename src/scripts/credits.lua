Credits = {}

function Credits:init()
    callScript("src/scripts/backButton.lua")

    self.id = createEntity()
    self.entities = {}
    self.entities[1] = self.id
    self.entities[2] = createEntity()
    self.x, self.y = 0, 0
    self.w, self.h = 640, 360
    self.text = 
    "Obinsonne Servius\n\
	[]	Game engine architecture (C++ / Lua ECS)\
	[]	Custom 2D renderer (Raylib)\
	[]	UI and audio systems\
	[]	Game logic, enemy AI, and scripting\
	[]	Hot reloading, asset management\
    "

    createUIComponent(
        function() end,
        self.x, self.y,
        self.w, self.h,
        105, 145, 125, 255,
        0,
        self.text, 0,0, 20,
        0, 0, 0, 255,
        self.id
    )

    BackButton:init(function() self:cleanup() end, 32, 32, self.entities[2])

end 

function Credits:cleanup()
    for i = 1, #self.entities do
        destroyEntity(self.entities[i])
    end
    MainMenu:init()
end 

