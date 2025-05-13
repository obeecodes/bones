#pragma once 

#include "../components/audio.h"
#include "eventManager.h"
#include "../core/ecs.h"
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include "raylib.h"

class AudioSystem{
public: 
    static AudioSystem* Instance();
    void update();
    void createAudioComponent(std::string sound, bool instant, uint32_t entityID);
    void createAudioComponent(Audio audio, uint32_t entityID);

    void init();
    void addSound(const std::string& sound, const char* file);
    void playSound(uint32_t entityID);
    void createSubscriptions();
    ECS<Audio>* getAudioComponentSystem();
    ~AudioSystem();

    static void destroyEntity(const Event& event);


private:

  std::queue<Sound> playSounds;

  std::unordered_map <std::string, Sound> sounds;
  static AudioSystem* instance;
  ECS<Audio>* audioComponentSystem;
  EventManager* eventManager;
  std::vector<Audio>& audios;
  uint32_t& inactiveIndex;
  AudioSystem();
};