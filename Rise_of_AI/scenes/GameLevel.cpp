
#include "GameLevel.h"
#include <cmath>

static void fillGround(unsigned int *tiles, int left, int right, int top, unsigned int topTile) {
    for (int x = left; x <= right; x++)
        for (int y = top; y < MAP_ROWS; y++)
            tiles[y * MAP_COLS + x] = topTile;
}

static void fillPlatform(unsigned int *tiles, int left, int right, int row, unsigned int tile) {
    for (int x = left; x <= right; x++) tiles[row * MAP_COLS + x] = tile;
}

void GameLevel::buildMap() {
    for (int i = 0; i < MAP_COLS * MAP_ROWS; i++) mTiles[i] = 0;
    unsigned int tile = mNumber == 1 ? 1 : (mNumber == 2 ? 5 : 9);

    if (mNumber == 1) {
        fillGround(mTiles, 0, 15, 10, tile);
        fillGround(mTiles, 32, 47, 10, tile);
        fillPlatform(mTiles, 18, 22, 8, tile);
        fillPlatform(mTiles, 25, 29, 7, tile);
        fillPlatform(mTiles, 41, 45, 7, tile);
        fillPlatform(mTiles, 48, 52, 6, tile);
        fillPlatform(mTiles, 54, 57, 5, tile);
        fillPlatform(mTiles, 58, 59, 4, tile);
        mCoinPosition = { 2925, 165 };
    } else if (mNumber == 2) {
        fillGround(mTiles, 0, 24, 10, tile);
        fillGround(mTiles, 31, 59, 10, tile);
        fillPlatform(mTiles, 26, 30, 8, tile);
        mCoinPosition = { 2925, 465 };
    } else {
        fillGround(mTiles, 0, 8, 10, tile);
        fillGround(mTiles, 13, 19, 10, tile);
        fillGround(mTiles, 24, 31, 10, tile);
        fillGround(mTiles, 36, 43, 10, tile);
        fillGround(mTiles, 48, 53, 10, tile);
        fillGround(mTiles, 57, 59, 10, tile);

        mCoinPosition = { 2925, 465 };
    }


    mMap = new Map(MAP_COLS, MAP_ROWS, mTiles, "assets/platforms.png",
                   TILE_SIZE, 4, 4, {1500, 300});
}

void GameLevel::spawnEnemies() {
    if (mNumber == 1) return; // Level 1: I didn't want to keep enemies

    std::map<Direction, std::vector<int>> slimeFrames = {
        { LEFT, { 0, 1, 2, 3, 4, 5, 6, 7 } },
        { RIGHT, { 0, 1, 2, 3, 4, 5, 6, 7 } }
    };

    auto makeSlime = [&](float x, float y, bool follower, float left, float right) {
        Entity *slime = new Entity({x,y}, {48,48},
            follower ? "assets/opponent2.png" : "assets/opponent1.png",
            ATLAS, {3,4}, slimeFrames, NPC);

        slime->setColliderDimensions({32,34});
        slime->setSpeed(follower ? 165 : 85);
        slime->setAcceleration({0, 981});
        slime->setAIType(follower ? FOLLOWER : WANDERER);
        slime->setAIState(IDLE);
        slime->setPatrolBounds(left,right);
        slime->setFrameSpeed(9);
        slime->setIdleFrame(4);
        mEnemies.push_back(slime);
    };

    if (mNumber == 2) { // Level 2: I wanted a few green slimes around the map
        makeSlime(350, 460, false, 200, 600);
        makeSlime(650, 460, false, 560, 930);
        makeSlime(950, 460, false, 800, 1150);

        makeSlime(1650, 460, false, 1550, 1850);
        makeSlime(2040, 460, false, 1900, 2240);
        makeSlime(2450, 460, false, 2350, 2700);
        makeSlime(2800, 460, false, 2700, 2900);
    }
    else {
        // Level 3: To add difficulty, I added purple slimes
        makeSlime(250, 460, true, 100, 400);
        makeSlime(780, 460, true, 700, 910);
        makeSlime(1400, 460, true, 1250, 1500);
        makeSlime(1850, 460, true, 1800, 2100);
        makeSlime(2550, 460, true, 2450, 2650);
        makeSlime(2850, 460, true, 2750, 2950);
    }
}


void GameLevel::initialise() {
    mGameState.nextSceneID = NO_SCENE;
    mCompleted = false;
    mCoinTime = 0;
    gCurrentLevel = mNumber;

    buildMap();

    std::map<Direction, std::vector<int>> playerFrames = {
        { RIGHT, {24,25,26,27,28,29,30,31} },
        { LEFT, {24,25,26,27,28,29,30,31} },
        { UP, {24,25,26,27,28,29,30,31} },
        { DOWN, {24,25,26,27,28,29,30,31} }
    };

    mPlayer = new Entity({100,450}, {58,58}, "assets/protagonist.png",
        ATLAS, {8,8}, playerFrames, PLAYER);

    mPlayer->setColliderDimensions({28,42});
    mPlayer->setSpeed(290);
    mPlayer->setJumpingPower(560);
    mPlayer->setAcceleration({0,981});
    mPlayer->setFrameSpeed(10);
    mPlayer->setIdleFrame(0);

    mFairy = new Entity({65,400}, {38,38}, "assets/lerper_fairy.png", NPC);
    mFairy->setAIType(LERPER);
    mFairy->setAIState(FOLLOWING);
    mFairy->setLerpFactor(5.0f);
    mFairy->setColliderDimensions({12,12});

    mCoin = LoadTexture("assets/coin.png");
    mHeart = LoadTexture("assets/heart.png");
    mDecorations = LoadTexture("assets/maps_tileset.png");
    mPlatforms = LoadTexture("assets/platforms.png");

    spawnEnemies();

    mGameState.hinako = mPlayer;
    mGameState.map = mMap;

    gCamera.offset = {SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.5f};
    gCamera.target = {mPlayer->getPosition().x, SCREEN_HEIGHT * 0.5f};
    gCamera.rotation = 0;
    gCamera.zoom = 1.0f;
}

void GameLevel::die() {
    if (mGameState.nextSceneID != NO_SCENE) return;

    gLives--;
    PlaySound(gDamageSound);
    mGameState.nextSceneID = DEATH;
}

void GameLevel::update(float dt) {
    if (mGameState.nextSceneID != NO_SCENE) return;

    mCoinTime += dt;
    mPlayer->resetMovement();

    if (IsKeyDown(KEY_A)) mPlayer->moveLeft();
    else if (IsKeyDown(KEY_D)) mPlayer->moveRight();

    extern bool gJumpQueued;

    if (gJumpQueued && mPlayer->isCollidingBottom()) {
        mPlayer->jump();
        PlaySound(gJumpSound);
    }

    gJumpQueued = false;

    float oldBottom = mPlayer->getPosition().y +
                      mPlayer->getColliderDimensions().y / 2;

    mPlayer->update(dt, nullptr, mMap, nullptr, 0);
    mFairy->update(dt, mPlayer, nullptr, nullptr, 0);

    for (Entity *enemy : mEnemies) {
        if (!enemy->isActive()) continue;

        enemy->resetMovement();
        enemy->update(dt, mPlayer, mMap, nullptr, 0);

        if (!mPlayer->overlaps(enemy)) continue;

        float enemyTop = enemy->getPosition().y -
                         enemy->getColliderDimensions().y / 2;

        bool stomp = mPlayer->getVelocity().y > 0 &&
                     oldBottom <= enemyTop + 12;

        if (stomp) {
            enemy->deactivate();
            mPlayer->jump();
            PlaySound(gExplosionSound);
        } else {
            die();
            return;
        }
    }

    Vector2 pos = mPlayer->getPosition();

    if (pos.y > 555 || pos.x < -40) {
        die();
        return;
    }

    if (pos.x > 2970)
        mPlayer->setPosition({2970, pos.y});

    Rectangle playerBox = {pos.x - 14, pos.y - 21, 28, 42};
    Rectangle coinBox = {
        mCoinPosition.x - 18,
        mCoinPosition.y - 18,
        36,
        36
    };

    if (CheckCollisionRecs(playerBox, coinBox) && !mCompleted) {
        mCompleted = true;
        PlaySound(gCoinSound);
        mGameState.nextSceneID =
            mNumber == 3 ? WIN : levelScene(mNumber + 1);
    }

    float minTarget = gCamera.offset.x;
    float maxTarget = MAP_COLS * TILE_SIZE -
                      (SCREEN_WIDTH - gCamera.offset.x);

    gCamera.target.x = fmaxf(minTarget, fminf(maxTarget, pos.x));
}

void GameLevel::drawDecorations() {
    float treeX = mNumber == 1 ? 0.0f :
                  (mNumber == 2 ? 80.0f : 96.0f);

    Rectangle treeSource = {treeX, 48.0f, 16.0f, 48.0f};

    Rectangle bushSource = mNumber == 1
        ? Rectangle{16.0f, 48.0f, 16.0f, 16.0f}
        : Rectangle{treeX, 96.0f, 16.0f, 16.0f};

    const float groundTop = 10.0f * TILE_SIZE;

    for (int i = 3; i < MAP_COLS; i += 7) {
        if (mTiles[10 * MAP_COLS + i] == 0) continue;

        float treeWidth = 55.0f;
        float treeHeight = 150.0f;
        float x = i * TILE_SIZE + 5.0f;

        DrawTexturePro(
            mDecorations,
            treeSource,
            {x, groundTop - treeHeight, treeWidth, treeHeight},
            {0,0},
            0,
            WHITE
        );

        int bushCol = i + 2;

        if (bushCol < MAP_COLS &&
            mTiles[10 * MAP_COLS + bushCol] != 0) {

            float bushWidth = 45.0f;
            float bushHeight = 45.0f;

            DrawTexturePro(
                mDecorations,
                bushSource,
                {x + 75.0f, groundTop - bushHeight,
                 bushWidth, bushHeight},
                {0,0},
                0,
                WHITE
            );
        }
    }
}

void GameLevel::drawWorld() {
    Color sky = mNumber == 1
        ? Color{158,222,248,255}
        : mNumber == 2
        ? Color{252,211,100,255}
        : Color{27,49,102,255};

    Color water = mNumber == 1
        ? Color{108,154,205,255}
        : mNumber == 2
        ? Color{235,167,62,255}
        : Color{108,62,164,255};

    ClearBackground(sky);
    BeginMode2D(gCamera);

    DrawRectangle(
        0,
        500,
        MAP_COLS * TILE_SIZE,
        100,
        water
    );

    int terrainX = mNumber == 1 ? 0 :
                   (mNumber == 2 ? 64 : 128);

    int platformRow = mNumber == 1 ? 0 :
                      (mNumber == 2 ? 1 : 2);

    for (int row = 0; row < MAP_ROWS; ++row) {
        for (int col = 0; col < MAP_COLS; ++col) {
            if (mTiles[row * MAP_COLS + col] == 0)
                continue;

            float x = col * TILE_SIZE;
            float y = row * TILE_SIZE;

            if (row >= 10) {

                bool top = row == 0 ||
                    mTiles[(row - 1) * MAP_COLS + col] == 0;

                DrawTexturePro(
                    mDecorations,
                    {(float)terrainX,
                     top ? 0.0f : 16.0f,
                     16,16},
                    {x,y,TILE_SIZE,TILE_SIZE},
                    {0,0},
                    0,
                    WHITE
                );
            } else {

                DrawTexturePro(
                    mPlatforms,
                    {0.0f,(float)(platformRow*16),16,16},
                    {x,y,TILE_SIZE,TILE_SIZE},
                    {0,0},
                    0,
                    WHITE
                );
            }
        }
    }

    drawDecorations();

    for (Entity *enemy : mEnemies)
        enemy->render();

    mPlayer->render();
    mFairy->render();

    int frame = (int)(mCoinTime * 10) % 12;

    DrawTexturePro(
        mCoin,
        {(float)(frame * 16),0,16,16},
        {mCoinPosition.x - 24,
         mCoinPosition.y - 24,
         48,48},
        {0,0},
        0,
        WHITE
    );

    EndMode2D();
}

void GameLevel::render() {
    drawWorld();

    for (int i = 0; i < gLives; i++) {
        DrawTexturePro(
            mHeart,
            {0,0,(float)mHeart.width,(float)mHeart.height},
            {(float)(16+i*49),16,43,43},
            {0,0},
            0,
            WHITE
        );
    }

    DrawText(
        TextFormat("LEVEL %d", mNumber),
        SCREEN_WIDTH - 175,
        22,
        28,
        WHITE
    );

    DrawText(
        "A/D MOVE   W JUMP   R RESTART",
        20,
        SCREEN_HEIGHT - 30,
        18,
        WHITE
    );
}

void GameLevel::shutdown() {
    delete mPlayer;
    mPlayer = nullptr;

    delete mFairy;
    mFairy = nullptr;

    for (Entity *enemy : mEnemies)
        delete enemy;

    mEnemies.clear();

    delete mMap;
    mMap = nullptr;

    UnloadTexture(mCoin);
    UnloadTexture(mHeart);
    UnloadTexture(mDecorations);
    UnloadTexture(mPlatforms);

    mGameState.hinako = nullptr;
    mGameState.map = nullptr;
}
