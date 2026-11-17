#ifndef GAME_SHARED_H
#define GAME_SHARED_H
#include "CS3113/Scene.h"

constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 600;
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
constexpr float TILE_SIZE = 50.0f;
constexpr int MAP_COLS = 60;
constexpr int MAP_ROWS = 12;

extern int gLives;
extern int gCurrentLevel;
extern Music gBGM;
extern Sound gJumpSound, gDamageSound, gCoinSound, gExplosionSound;
extern Camera2D gCamera;

inline SceneID levelScene(int n) { return n == 1 ? LEVEL1 : n == 2 ? LEVEL2 : LEVEL3; }
#endif