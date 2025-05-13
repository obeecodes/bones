width = 16
height = 8

Map = {}


function Map:init()
    Map.tiles = {}
    self.id = createEntity()
    local w, h = getMapSize()
    local tileSize = 32

    local row = 1
    for y = 0, h - tileSize, tileSize do
        Map.tiles[row] = {}
        local col = 1
        for x = 0, w - tileSize, tileSize do
            local id = createEntity()
            Map.tiles[row][col] = id

            if (y == h - tileSize and x==0 or y == h - tileSize and x == 10*32) or (x==0 and y ==0 or y==0 and x == 10*32) then 
                 
                createSpriteComponent(
                    "MAP",
                    x, y, 32, 32,
                    x, y, 32, 32,
                    0, 0,
                    0.0,
                    255, 255, 255, 255,
                    0,
                    1, 0.0, false,
                    id
                )

                createColliderComponent(
                 x, y, 32*6, 32,    -- rect (x, y, w, h)
                 0, 0,            -- offset (x, y)
                 0,               -- ColliderType (e.g., 0=STATIC, 1=DYNAMIC, etc.)
                 id          -- entityID
                 )
            else 
                createSpriteComponent(
                    "MAP",
                    x, y, 32, 32,
                    x, y, 32, 32,
                    0, 0,
                    0.0,
                    255, 255, 255, 255,
                    0,
                    1, 0.0, false,
                    id
                )
            end

            createPhysicsComponent(
                0,0,0,0,
                id
            )

            createTransformComponent(
                x, 
                y,
                id
            )
            col = col + 1
        end
        row = row + 1
    end
    createAudioComponent(
        "THEME",
        true,
        self.id
    )
end

function Map:cleanup()
    for row = 1, #Map.tiles do
        for col = 1, #Map.tiles[row] do
            local id = Map.tiles[row][col]
            if id then
                destroyEntity(id)
            end
        end
    end
    Map.tiles = {}
    destroyEntity(self.id)
end