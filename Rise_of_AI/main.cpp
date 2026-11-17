#include "GameShared.h"
#include "scenes/Menu.h"
#include "scenes/GameLevel.h"
#include "scenes/Death.h"
#include "scenes/Win.h"

int gLives = 3;
int gCurrentLevel = 1;
bool gJumpQueued = false;
Music gBGM{};
Sound gJumpSound{}, gDamageSound{}, gCoinSound{}, gExplosionSound{};
Camera2D gCamera{};

AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f;
float gTimeAccumulator = 0.0f;
Scene *gCurrentScene = nullptr;
std::map<SceneID, Scene*> gScenes;

void switchToScene(SceneID id) {
    if (gCurrentScene != nullptr) gCurrentScene->shutdown();
    gCurrentScene = gScenes.at(id);
    gCurrentScene->initialise();
    gTimeAccumulator = 0.0f;
    gJumpQueued = false;
}
void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rise of AI");
    InitAudioDevice();
    gBGM = LoadMusicStream("assets/audio/background.mp3");
    gBGM.looping = true;
    SetMusicVolume(gBGM, 0.33f);
    PlayMusicStream(gBGM);
    gJumpSound = LoadSound("assets/audio/jump_sound.wav");
    gDamageSound = LoadSound("assets/audio/damage_sound.wav");
    gCoinSound = LoadSound("assets/audio/coin_sound.wav");
    gExplosionSound = LoadSound("assets/audio/explosion_sound.wav");
    gScenes[MENU] = new Menu();
    gScenes[LEVEL1] = new GameLevel(1);
    gScenes[LEVEL2] = new GameLevel(2);
    gScenes[LEVEL3] = new GameLevel(3);
    gScenes[DEATH] = new Death();
    gScenes[WIN] = new Win();
    switchToScene(MENU);
    SetTargetFPS(120);
    gPreviousTicks = (float)GetTime();
}
void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
    if (gCurrentScene == nullptr) return;
    SceneID scene = NO_SCENE;
    for (const auto &item : gScenes)
        if (item.second == gCurrentScene) scene = item.first;
    bool playing = scene == LEVEL1 || scene == LEVEL2 || scene == LEVEL3;
    if (playing) {
        if (IsKeyPressed(KEY_W)) gJumpQueued = true;
        if (IsKeyPressed(KEY_R)) {
            gLives--;
            PlaySound(gDamageSound);
            switchToScene(DEATH);
            return;
        }
    }
    if (IsKeyPressed(KEY_ONE)) { gCurrentLevel = 1; switchToScene(LEVEL1); }
    if (IsKeyPressed(KEY_TWO)) { gCurrentLevel = 2; switchToScene(LEVEL2); }
    if (IsKeyPressed(KEY_THREE)) { gCurrentLevel = 3; switchToScene(LEVEL3); }
}
void update() {
    UpdateMusicStream(gBGM);
    float ticks = (float)GetTime();
    float dt = ticks - gPreviousTicks;
    gPreviousTicks = ticks;
    if (dt > 0.25f) dt = 0.25f;
    gTimeAccumulator += dt;
    while (gTimeAccumulator >= FIXED_TIMESTEP) {
        gCurrentScene->update(FIXED_TIMESTEP);
        gTimeAccumulator -= FIXED_TIMESTEP;
        SceneID next = gCurrentScene->getState().nextSceneID;
        if (next != NO_SCENE) {
            switchToScene(next);
            break;
        }
    }
}
void render() {
    BeginDrawing();
    gCurrentScene->render();
    EndDrawing();
}
void shutdown() {
    if (gCurrentScene != nullptr) gCurrentScene->shutdown();
    for (auto &item : gScenes) delete item.second;
    gScenes.clear();
    UnloadSound(gJumpSound);
    UnloadSound(gDamageSound);
    UnloadSound(gCoinSound);
    UnloadSound(gExplosionSound);
    UnloadMusicStream(gBGM);
    CloseAudioDevice();
    CloseWindow();
}
int main() {
    initialise();
    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }
    shutdown();
    return 0;
}
