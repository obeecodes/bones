#pragma once

#include <string>

enum AudioSample{
  NONE,
  EXPLOSION,
  HURT,
  LASER,
  PICKUP,
  POWERUP,
  THEME,
  LIGHTNING,
  FIREBALL,
};

struct Audio{
  std::string sound;
  bool instant; // play sound on creation?
  Audio();
  Audio(std::string sound, bool instant);

};
