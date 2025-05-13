#pragma once
#include "raylib.h"
#include <cstdint>

enum Layer {
  LAYER_BACKGROUND = 0,
  LAYER_TERRAIN    = 1,
  LAYER_PLAYER     = 2,
  LAYER_OBJECTS    = 3,
  LAYER_UI         = 4,
  LAYER_CUTSCENE   = 5,
};

struct Sprite {
  Texture2D texture;
  Rectangle src;
  Rectangle dest;
  Vector2 origin;
  float rotation;
  Color tint;
  Layer layer;

  int frameCount;       
  int currentFrame;
  float frameTime; 
  float timeAccumulator;
  bool looping;

  Sprite();
  Sprite(
    Texture2D texture, Rectangle src, Rectangle dest, Vector2 origin, float rotation, 
    Color tint, Layer layer, int frameCount, float frameTime, bool looping
  );
  Sprite(Texture2D texture, Rectangle src, Rectangle dest, Layer layer);
  Sprite(Texture2D texture, Rectangle src, Rectangle dest);
  Sprite(Texture2D texture, Rectangle src);
};