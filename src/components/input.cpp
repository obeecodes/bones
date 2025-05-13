#include "../../include/components/input.h"


Input::Input()
    : scrollDelta(0.0f),
      mousePosition{0.0f, 0.0f},
      mouseDelta{0.0f, 0.0f} {
    mouseButtonStates.resize(3, false);
}