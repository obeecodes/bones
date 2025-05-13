#include "../../include/systems/audioSystem.h"
#include "../../include/core/luastate.h"
#include <iostream>

AudioSystem* AudioSystem::instance = nullptr;

AudioSystem* AudioSystem::Instance() {
    if (!instance) {
        instance = new AudioSystem();
    }
    return instance;
}

AudioSystem::AudioSystem()
: audioComponentSystem(new ECS<Audio>()),
  audios(audioComponentSystem->getComponents()),
  inactiveIndex(audioComponentSystem->getInactiveIndex()),
  eventManager(EventManager::Instance()) {
    createSubscriptions();
  }

void AudioSystem::createAudioComponent(std::string sound, bool instant, uint32_t entityID) {
    this->audioComponentSystem->createComponent(Audio{sound, instant}, entityID);
    if(instant){
        PlaySound(sounds[sound]);
    }
}

void AudioSystem::createAudioComponent(Audio audio, uint32_t entityID) {
    this->audioComponentSystem->createComponent(audio, entityID);

    if(audio.instant){
        PlaySound(sounds[audio.sound]);
    }
}

void AudioSystem::createSubscriptions(){
  eventManager->subscribe(EventType::DESTROY, destroyEntity);
}

void AudioSystem::update() {
    while (!playSounds.empty()) {
        PlaySound(playSounds.front());
        playSounds.pop();
    }
}

void AudioSystem::init() {
    InitAudioDevice();
    LuaState::Instance()->callScript("src/scripts/sounds.lua");
}



void AudioSystem::addSound(const std::string& sound, const char* file) {
    Sound s = LoadSound(file);
    if (s.frameCount == 0) {
        std::cerr << "Failed to load sound: " << file << std::endl;
    }
    sounds[sound] = s;
}

void AudioSystem::playSound(uint32_t entityID) {
    this->playSounds.push(sounds[audioComponentSystem->getEntityComponent(entityID).sound]);
}

AudioSystem::~AudioSystem() {
    for (auto& pair : sounds) {
        UnloadSound(pair.second);
    }
    sounds.clear();

    CloseAudioDevice();
}

void AudioSystem::destroyEntity(const Event& event){
    AudioSystem::Instance()->getAudioComponentSystem()->destroyComponent(event.entityA);
}

ECS<Audio>* AudioSystem::getAudioComponentSystem(){
    return this->audioComponentSystem;
}
