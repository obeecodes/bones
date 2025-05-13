FireNova = {}

function FireNova:init(x, y, id)
    self.id = id
    w, h = 512, 256
    ox, oy = 256+16, 128+64
     
    createSpriteComponent(
       "FIRENOVA",             -- Texture name
   
       0, 0, w, h,       -- src rectangle (x, y, w, h)
       x - ox, y - oy, w + 32 + 32, h+32 + 32,       -- dest rectangle (x, y, w, h)
   
       0, 0,             -- origin (x, y)
       0.0,                -- rotation
   
       255, 255, 255, 255, -- tint (r, g, b, a)
   
       5,                  -- layer
   
       28,                  -- frameCount
       1/8,            -- frameTime
       false,              -- looping
       id             
   )

   createAudioComponent(
    "EXPLOSION",
    false,
    id
    )  

end

function FireNova:onCollision(otherID)
    --
end