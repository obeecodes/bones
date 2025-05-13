#include "../../include/components/sprite.h"

Sprite::Sprite()
: texture(), src{0, 0, 0, 0}, dest{0, 0, 0, 0} {}

Sprite::Sprite(Texture2D texture, Rectangle src, Rectangle dest, Layer layer)
: texture(texture), src(src), dest(dest), layer(layer)  {
}

Sprite::Sprite(Texture2D texture, Rectangle src, Rectangle dest)
: texture(texture), src(src), dest(dest), layer(LAYER_BACKGROUND)  {
}

Sprite::Sprite(Texture2D texture, Rectangle src)
: texture(), src(src), layer(LAYER_BACKGROUND)  {}

Sprite::Sprite(
    Texture2D texture, Rectangle src, Rectangle dest, Vector2 origin, float rotation, 
    Color tint, Layer layer, int frameCount, float frameTime, bool looping
) : texture(texture),
    src(src),
    dest(dest),
    origin(origin),
    rotation(rotation),
    tint(tint),
    layer(layer),
    frameCount(frameCount),
    currentFrame(0),
    frameTime(frameTime),
    timeAccumulator(0.0f),
    looping(looping)
{}