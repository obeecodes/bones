InputBindings = {
    movement = {
        up    = KEY_W,
        down  = KEY_S,
        left  = KEY_A,
        right = KEY_D,
    },
    actions = {
        attack = KEY_J,
        special   = KEY_K,
    },
    global = {
        pause = KEY_P,
        map   = KEY_M,
        drawColliders = KEY_C,
    }
}

function getKeys()
    local keys = {}
    for _, category in pairs (InputBindings) do 
        for _, key in pairs(category) do 
            table.insert(keys, key)
        end 
    end
    return keys
end
