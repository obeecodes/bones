#pragma once 

#include <stack>
#include "raylib.h"

class Engine{
public:
    static Engine* Instance();
    bool getRunning();
    void setRunning(bool running);
    void run();

    void update();
    void render();

    ~Engine();

private:
    static Engine* instance;
    bool running;
    Engine();

    Texture2D text;
    int counter;

    uint64_t fps;
    std::chrono::duration<float> cycleTime;
    std::chrono::high_resolution_clock::time_point systemTimerStart;
    std::chrono::high_resolution_clock::time_point systemTimerEnd;
    std::chrono::duration<float> elapsedSeconds;
    double accumulatedSeconds;
};