#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H
#include "../GameShared.h"
#include <vector>

class GameLevel : public Scene {
    int mNumber;
    unsigned int mTiles[MAP_COLS * MAP_ROWS]{};
    Entity *mPlayer = nullptr;
    Entity *mFairy = nullptr;
    Map *mMap = nullptr;
    std::vector<Entity*> mEnemies;
    Texture2D mCoin{};
    Texture2D mHeart{};
    Texture2D mDecorations{};
    Texture2D mPlatforms{};
    float mCoinTime = 0.0f;
    Vector2 mCoinPosition{};
    bool mCompleted = false;
    void buildMap();
    void spawnEnemies();
    void die();
    void drawWorld();
    void drawDecorations();
public:
    explicit GameLevel(int number) : mNumber(number) {}
    void initialise() override;
    void update(float dt) override;
    void render() override;
    void shutdown() override;
};
#endif
