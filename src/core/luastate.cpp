#include "../../include/core/luastate.h"
#include "../../include/entities/entityManager.h"
#include "../../include/systems/eventManager.h"
#include "../../include/core/engine.h"

LuaState* LuaState::instance = nullptr;

LuaState* LuaState::Instance(){
    if(!instance){
        instance = new LuaState();
    }
    return instance;
}

LuaState::LuaState()
:   eventManager(EventManager::Instance()){
    L = luaL_newstate();
    luaL_openlibs(L);
}

LuaState::~LuaState(){
    lua_close(L);
}

void LuaState::init(){
    registerFunctions();
    createKeyboardKeys();
}


void LuaState::registerFunctions() {
    lua_register(L, "createEntity", lua_createEntity);
    lua_register(L, "createAudioComponent", lua_createAudioComponent);
    lua_register(L, "createColliderComponent", lua_createColliderComponent);
    lua_register(L, "createHealthComponent", lua_createHealthComponent);
    lua_register(L, "createInputComponent", lua_createInputComponent);
    lua_register(L, "createPhysicsComponent", lua_createPhysicsComponent);
    lua_register(L, "createTransformComponent", lua_createTransformComponent);
    lua_register(L, "createScriptComponent", lua_createScriptComponent);
    lua_register(L, "createSpriteComponent", lua_createSpriteComponent);
    lua_register(L, "createUIComponent", lua_createUIComponent);
    
    lua_register(L, "addTexture", lua_addTexture);
    lua_register(L, "addSound", lua_addSound);
    lua_register(L, "playSound", lua_playSound);
    lua_register(L, "moveIntent", lua_moveIntent);
    lua_register(L, "setKeys", lua_setKeys);
    lua_register(L, "isKeyDown", lua_isKeyDown);
    lua_register(L, "isKeyPressed", lua_isKeyPressed);
    lua_register(L, "getMapSize", lua_getMapSize);
    
    lua_register(L, "callScript", lua_callScript);
    lua_register(L, "getPosition", lua_getPosition);
    lua_register(L, "getVelocity", lua_getVelocity);
    lua_register(L, "dealDamage", lua_dealDamage);
    lua_register(L, "getHealth", lua_getHealth);
    lua_register(L, "destroyEntity", lua_destroyEntity);
    lua_register(L, "updateUIComponent", lua_updateUIComponent);
    lua_register(L, "toggleDrawColliders", lua_toggleDrawColliders);
    lua_register(L, "closeEngine", lua_closeEngine);
}

void LuaState::callUICallback(int luaCallbackRef, uint32_t index) {
    if (luaCallbackRef == LUA_NOREF) return;

    lua_rawgeti(this->L, LUA_REGISTRYINDEX, luaCallbackRef);
    lua_pushinteger(this->L, static_cast<lua_Integer>(index));

    if (lua_pcall(this->L, 1, 0, 0) != LUA_OK) {
        std::cerr << "Lua UI callback error: " << lua_tostring(this->L, -1) << std::endl;
        lua_pop(this->L, 1);
    }
}

void LuaState::pushEntityId(uint32_t entityID) {
    lua_pushinteger(L, entityID);
}

void LuaState::callScript(const char* file) {
    if (luaL_dofile(L, file) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}

int LuaState::lua_callScript(lua_State* L){
    const char* script = luaL_checkstring(L, 1);
    LuaState::Instance()->callScript(script);
    return 0;
}

lua_State* LuaState::getLuaState(){
    return L;
}

int LuaState::lua_createEntity(lua_State* L){
    uint32_t id = EntityManager::Instance()->createEntity();
    lua_pushinteger(L, id); 
    return 1;
}

int LuaState::lua_createAudioComponent(lua_State* L){
    std::string sound = std::string(luaL_checkstring(L, 1));
    bool instant = lua_toboolean(L, 2);
    uint32_t entityID = static_cast<uint32_t>(luaL_checknumber(L, 3));

    AudioSystem::Instance()->createAudioComponent(Audio(sound, instant), entityID);
    return 0;
}

int LuaState::lua_createColliderComponent(lua_State* L) {
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    float w = luaL_checknumber(L, 3);
    float h = luaL_checknumber(L, 4);

    float ox = luaL_checknumber(L, 5);
    float oy = luaL_checknumber(L, 6);

    int typeInt = luaL_checkinteger(L, 7);
    uint32_t entityID = static_cast<uint32_t>(luaL_checkinteger(L, 8));

    Rectangle rect = {x, y, w, h};
    Vec2 offset = {ox, oy};
    ColliderType type = static_cast<ColliderType>(typeInt);

    CollisionSystem::Instance()->createColliderComponent(Collider{rect, offset, type}, entityID);
    return 0;
}

int LuaState::lua_createHealthComponent(lua_State* L){
    uint32_t max = static_cast<uint32_t>(luaL_checkinteger(L, 1));
    uint32_t entityID = static_cast<uint32_t>(luaL_checkinteger(L, 2));

    HealthSystem::Instance()->createHealthComponent(Health{max}, entityID);
    return 0;
}

int LuaState::lua_getPosition(lua_State* L) {
    if (!lua_isinteger(L, 1)) {
        lua_pushstring(L, "Expected entity ID as first argument");
        lua_error(L);
        return 0;
    }

    uint32_t entityID = static_cast<uint32_t>(lua_tointeger(L, 1));

    uint32_t index = PhysicsSystem::Instance()->getPhysicsComponentSystem()->getEntityIndex(entityID);

    Transform2D& transform = PhysicsSystem::Instance()->getTransforms()[index];

    lua_pushnumber(L, transform.position.x);
    lua_pushnumber(L, transform.position.y);

    return 2; 
}

int LuaState::lua_getHealth(lua_State* L){
    uint32_t entityID = static_cast<uint32_t>(lua_tointeger(L, 1));

    uint32_t index = HealthSystem::Instance()->getHealthComponentSystem()->getEntityIndex(entityID);

    Health& health = HealthSystem::Instance()->getHealths()[index];

    lua_pushnumber(L, static_cast<lua_Number>(health.current));

    return 1;
}


int LuaState::lua_getVelocity(lua_State* L){
    if (!lua_isinteger(L, 1)) {
        lua_pushstring(L, "Expected entity ID as first argument");
        lua_error(L);
        return 0;
    }

    uint32_t entityID = static_cast<uint32_t>(lua_tointeger(L, 1));

    uint32_t index = PhysicsSystem::Instance()->getTransformComponentSystem()->getEntityIndex(entityID);

    Physics& physics = PhysicsSystem::Instance()->getPhysics()[index];

    lua_pushnumber(L, physics.velocity.x);
    lua_pushnumber(L, physics.velocity.y);

    return 2; 
}


int LuaState::lua_createInputComponent(lua_State* L){
    uint32_t entityID = static_cast<uint32_t>(luaL_checkinteger(L, 1));

    InputSystem::Instance()->createInputComponent(Input{}, entityID);
    return 0;
}

int LuaState::lua_createPhysicsComponent(lua_State* L) {
    float vx = static_cast<float>(luaL_checknumber(L, 1));
    float vy = static_cast<float>(luaL_checknumber(L, 2));
    float ax = static_cast<float>(luaL_checknumber(L, 3));
    float ay = static_cast<float>(luaL_checknumber(L, 4));
    uint32_t entityID = static_cast<uint32_t>(luaL_checkinteger(L, 5));

    Physics physics(Vec2{vx, vy}, Vec2{ax, ay});
    PhysicsSystem::Instance()->createPhysicsComponent(physics, entityID);

    return 0;
}

int LuaState::lua_createTransformComponent(lua_State* L){

    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    uint32_t entityID = static_cast<uint32_t>(luaL_checkinteger(L, 3));

    PhysicsSystem::Instance()->createTransformComponent(Vec2{x,y}, entityID);

    return 0;
}

int LuaState::lua_createScriptComponent(lua_State* L){
    const char* script = luaL_checkstring(L, 1);
    const char* table = luaL_checkstring(L, 2);
    uint32_t entityID = static_cast<uint32_t>(luaL_checkinteger(L, 3));

    ScriptSystem::Instance()->createScriptComponent(Script{script, table}, entityID);
    return 0;
}

int LuaState::lua_createSpriteComponent(lua_State* L) {
    const char* text = luaL_checkstring(L, 1);
    Texture2D texture = SpriteSystem::Instance()->getTexture(std::string(text));

    float sx = luaL_checknumber(L, 2);
    float sy = luaL_checknumber(L, 3);
    float sw = luaL_checknumber(L, 4);
    float sh = luaL_checknumber(L, 5);

    float dx = luaL_checknumber(L, 6);
    float dy = luaL_checknumber(L, 7);
    float dw = luaL_checknumber(L, 8);
    float dh = luaL_checknumber(L, 9);

    float ox = luaL_checknumber(L, 10);
    float oy = luaL_checknumber(L, 11);

    float rotation = luaL_checknumber(L, 12);

    unsigned char r = static_cast<unsigned char>(luaL_checkinteger(L, 13));
    unsigned char g = static_cast<unsigned char>(luaL_checkinteger(L, 14));
    unsigned char b = static_cast<unsigned char>(luaL_checkinteger(L, 15));
    unsigned char a = static_cast<unsigned char>(luaL_checkinteger(L, 16));

    Layer layer = static_cast<Layer>(luaL_checknumber(L, 17));
    int frameCount = static_cast<int>(luaL_checkinteger(L, 18));
    float frameTime = static_cast<float>(luaL_checknumber(L, 19));
    bool looping = lua_toboolean(L, 20);

    uint32_t entityID = static_cast<uint32_t>(luaL_checkinteger(L, 21));

    Rectangle src = { sx, sy, sw, sh };
    Rectangle dst = { dx, dy, dw, dh };
    Vector2 origin = { ox, oy };
    Color tint = { r, g, b, a };

    Sprite sprite(texture, src, dst, origin, rotation, tint, layer, frameCount, frameTime, looping);
    SpriteSystem::Instance()->createSpriteComponent(sprite, entityID);

    return 0;
}

int LuaState::lua_createUIComponent(lua_State* L) {
    UI ui;

    if (lua_isfunction(L, 1)) {
        lua_pushvalue(L, 1);
        ui.luaCallbackRef = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    ui.bounds.x = static_cast<float>(lua_tonumber(L, 2));
    ui.bounds.y = static_cast<float>(lua_tonumber(L, 3));
    ui.bounds.width = static_cast<float>(lua_tonumber(L, 4));
    ui.bounds.height = static_cast<float>(lua_tonumber(L, 5));

    ui.color = {
        static_cast<unsigned char>(lua_tointeger(L, 6)),
        static_cast<unsigned char>(lua_tointeger(L, 7)),
        static_cast<unsigned char>(lua_tointeger(L, 8)),
        static_cast<unsigned char>(lua_tointeger(L, 9))
    };

    ui.layer = static_cast<int>(lua_tointeger(L, 10));

    if (lua_isstring(L, 11)) {
        ui.label = lua_tostring(L, 11);
    }

    if (lua_isnumber(L, 12) && lua_isnumber(L, 13)) {
        ui.labelOffset.x = static_cast<float>(lua_tonumber(L, 12));
        ui.labelOffset.y = static_cast<float>(lua_tonumber(L, 13));
    }

    ui.fontSize = static_cast<int>(lua_tointeger(L, 14));

    ui.textColor = {
        static_cast<unsigned char>(lua_tointeger(L, 15)),
        static_cast<unsigned char>(lua_tointeger(L, 16)),
        static_cast<unsigned char>(lua_tointeger(L, 17)),
        static_cast<unsigned char>(lua_tointeger(L, 18))
    };

    uint32_t entityID = static_cast<uint32_t>(lua_tointeger(L, 19));
    UISystem::Instance()->createUIComponent(ui, entityID);

    return 0;
}

int LuaState::lua_addTexture(lua_State* L){

    const char* texture = luaL_checkstring(L, 1);
    const char* file = luaL_checkstring(L, 2);

    SpriteSystem::Instance()->addTexture(texture, file);

    return 0;

}

int LuaState::lua_addSound(lua_State* L){

    const char* sound = luaL_checkstring(L, 1);
    const char* file = luaL_checkstring(L, 2);

    AudioSystem::Instance()->addSound(sound, file);

    return 0;

}


int LuaState::lua_toggleDrawColliders(lua_State* L){
    CollisionSystem::Instance()->toggleDrawColliders();
    return 0;
}


int LuaState::lua_dealDamage(lua_State* L){
    uint32_t target= static_cast<uint32_t>(luaL_checknumber(L, 1));
    uint32_t source = static_cast<uint32_t>(luaL_checknumber(L, 2));
    uint32_t damage = static_cast<uint32_t>(luaL_checknumber(L, 3));
 
    Events::publishDamage(target, source, damage);

    return 0;
}

int LuaState::lua_moveIntent(lua_State* L){
    uint32_t entityID = static_cast<uint32_t>(luaL_checkinteger(L, 1));
    float dx = luaL_checknumber(L, 2);
    float dy = luaL_checknumber(L, 3);

    Events::publishMove(entityID, Vec2{dx,dy});
    return 0;
}

int LuaState::lua_updateUIComponent(lua_State* L) {
    UI ui;

    if (lua_isfunction(L, 1)) {
        lua_pushvalue(L, 1);
        ui.luaCallbackRef = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    ui.bounds.x = static_cast<float>(lua_tonumber(L, 2));
    ui.bounds.y = static_cast<float>(lua_tonumber(L, 3));
    ui.bounds.width = static_cast<float>(lua_tonumber(L, 4));
    ui.bounds.height = static_cast<float>(lua_tonumber(L, 5));

    ui.color = {
        static_cast<unsigned char>(lua_tointeger(L, 6)),
        static_cast<unsigned char>(lua_tointeger(L, 7)),
        static_cast<unsigned char>(lua_tointeger(L, 8)),
        static_cast<unsigned char>(lua_tointeger(L, 9))
    };

    ui.layer = static_cast<int>(lua_tointeger(L, 10));

    if (lua_isstring(L, 11)) {
        ui.label = lua_tostring(L, 11);
    }

    if (lua_isnumber(L, 12) && lua_isnumber(L, 13)) {
        ui.labelOffset.x = static_cast<float>(lua_tonumber(L, 12));
        ui.labelOffset.y = static_cast<float>(lua_tonumber(L, 13));
    }

    ui.fontSize = static_cast<int>(lua_tointeger(L, 14));

    ui.textColor = {
        static_cast<unsigned char>(lua_tointeger(L, 15)),
        static_cast<unsigned char>(lua_tointeger(L, 16)),
        static_cast<unsigned char>(lua_tointeger(L, 17)),
        static_cast<unsigned char>(lua_tointeger(L, 18))
    };

    uint32_t entityID = static_cast<uint32_t>(lua_tointeger(L, 19));
    UISystem::Instance()->updateUIComponent(ui, entityID);

    return 0;
}

int LuaState::lua_setKeys(lua_State* L) {
    if (!lua_istable(L, 1)) {
        std::cerr << "[LuaState] Error: Expected table of keycodes as argument to setKeys." << std::endl;
        return 0;
    }

    std::vector<int> keys;
    size_t len = lua_rawlen(L, 1);
    for (size_t i = 1; i <= len; ++i) {
        lua_rawgeti(L, 1, i);
        if (lua_isinteger(L, -1)) {
            keys.push_back(static_cast<int>(lua_tointeger(L, -1)));
        }
        lua_pop(L, 1); // pop value
    }

    InputSystem::Instance()->setKeys(keys);
    return 0;
}


void LuaState::createKeyboardKeys() {
    struct KeyDef {
        const char* name;
        int value;
    };

    const KeyDef keys[] = {
        { "KEY_NULL", 0 },
        { "KEY_APOSTROPHE", 39 },
        { "KEY_COMMA", 44 },
        { "KEY_MINUS", 45 },
        { "KEY_PERIOD", 46 },
        { "KEY_SLASH", 47 },
        { "KEY_ZERO", 48 },
        { "KEY_ONE", 49 },
        { "KEY_TWO", 50 },
        { "KEY_THREE", 51 },
        { "KEY_FOUR", 52 },
        { "KEY_FIVE", 53 },
        { "KEY_SIX", 54 },
        { "KEY_SEVEN", 55 },
        { "KEY_EIGHT", 56 },
        { "KEY_NINE", 57 },
        { "KEY_SEMICOLON", 59 },
        { "KEY_EQUAL", 61 },
        { "KEY_A", 65 },
        { "KEY_B", 66 },
        { "KEY_C", 67 },
        { "KEY_D", 68 },
        { "KEY_E", 69 },
        { "KEY_F", 70 },
        { "KEY_G", 71 },
        { "KEY_H", 72 },
        { "KEY_I", 73 },
        { "KEY_J", 74 },
        { "KEY_K", 75 },
        { "KEY_L", 76 },
        { "KEY_M", 77 },
        { "KEY_N", 78 },
        { "KEY_O", 79 },
        { "KEY_P", 80 },
        { "KEY_Q", 81 },
        { "KEY_R", 82 },
        { "KEY_S", 83 },
        { "KEY_T", 84 },
        { "KEY_U", 85 },
        { "KEY_V", 86 },
        { "KEY_W", 87 },
        { "KEY_X", 88 },
        { "KEY_Y", 89 },
        { "KEY_Z", 90 },
        { "KEY_SPACE", 32 },
        { "KEY_LEFT_BRACKET", 91 },
        { "KEY_BACKSLASH", 92 },
        { "KEY_RIGHT_BRACKET", 93 },
        { "KEY_GRAVE", 96 },
        { "KEY_ESCAPE", 256 },
        { "KEY_ENTER", 257 },
        { "KEY_TAB", 258 },
        { "KEY_BACKSPACE", 259 },
        { "KEY_INSERT", 260 },
        { "KEY_DELETE", 261 },
        { "KEY_RIGHT", 262 },
        { "KEY_LEFT", 263 },
        { "KEY_DOWN", 264 },
        { "KEY_UP", 265 },
        { "KEY_PAGE_UP", 266 },
        { "KEY_PAGE_DOWN", 267 },
        { "KEY_HOME", 268 },
        { "KEY_END", 269 },
        { "KEY_CAPS_LOCK", 280 },
        { "KEY_SCROLL_LOCK", 281 },
        { "KEY_NUM_LOCK", 282 },
        { "KEY_PRINT_SCREEN", 283 },
        { "KEY_PAUSE", 284 },
        { "KEY_F1", 290 },
        { "KEY_F2", 291 },
        { "KEY_F3", 292 },
        { "KEY_F4", 293 },
        { "KEY_F5", 294 },
        { "KEY_F6", 295 },
        { "KEY_F7", 296 },
        { "KEY_F8", 297 },
        { "KEY_F9", 298 },
        { "KEY_F10", 299 },
        { "KEY_F11", 300 },
        { "KEY_F12", 301 },
        { "KEY_LEFT_SHIFT", 340 },
        { "KEY_LEFT_CONTROL", 341 },
        { "KEY_LEFT_ALT", 342 },
        { "KEY_LEFT_SUPER", 343 },
        { "KEY_RIGHT_SHIFT", 344 },
        { "KEY_RIGHT_CONTROL", 345 },
        { "KEY_RIGHT_ALT", 346 },
        { "KEY_RIGHT_SUPER", 347 },
        { "KEY_KB_MENU", 348 },
        { "KEY_KP_0", 320 },
        { "KEY_KP_1", 321 },
        { "KEY_KP_2", 322 },
        { "KEY_KP_3", 323 },
        { "KEY_KP_4", 324 },
        { "KEY_KP_5", 325 },
        { "KEY_KP_6", 326 },
        { "KEY_KP_7", 327 },
        { "KEY_KP_8", 328 },
        { "KEY_KP_9", 329 },
        { "KEY_KP_DECIMAL", 330 },
        { "KEY_KP_DIVIDE", 331 },
        { "KEY_KP_MULTIPLY", 332 },
        { "KEY_KP_SUBTRACT", 333 },
        { "KEY_KP_ADD", 334 },
        { "KEY_KP_ENTER", 335 },
        { "KEY_KP_EQUAL", 336 },
        { "KEY_BACK", 4 },
        { "KEY_MENU", 5 },
        { "KEY_VOLUME_UP", 24 },
        { "KEY_VOLUME_DOWN", 25 }
    };

    // register each key constant as a Lua global
    for (const auto& key : keys) {
        lua_pushinteger(L, key.value);
        lua_setglobal(L, key.name);
    }
}

int LuaState::lua_isKeyDown(lua_State* L) {
    int key = luaL_checkinteger(L, 1);

    bool down = IsKeyDown(key);

    lua_pushboolean(L, down); 
    return 1;
}

int LuaState::lua_isKeyPressed(lua_State* L){
    int key = luaL_checkinteger(L, 1);

    bool down = IsKeyPressed(key);

    lua_pushboolean(L, down); 
    return 1;
}


int LuaState::lua_getMapSize(lua_State* L){
    Texture2D tex = SpriteSystem::Instance()->getTexture("MAP");
    lua_pushinteger(L, tex.width);
    lua_pushinteger(L, tex.height);
    return 2;
}

int LuaState::lua_destroyEntity(lua_State* L){
    uint32_t entityID = luaL_checknumber(L, 1);

    EventManager::Instance()->publish(Event{EventType::DESTROY, entityID});
    return 0;
}

int LuaState::lua_playSound(lua_State* L){
    uint32_t entityID = static_cast<uint32_t>(luaL_checknumber(L, 1));
    AudioSystem::Instance()->playSound(entityID);

    return 0;
}

int LuaState::lua_closeEngine(lua_State* L){
    Engine::Instance()->setRunning(false);
    return 0;
}



