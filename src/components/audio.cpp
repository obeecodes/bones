#include "../../include/components/audio.h"

Audio::Audio(){
    this->sound = NONE;
}

Audio::Audio(std::string sound, bool instant)
: sound(sound), instant(instant){}
