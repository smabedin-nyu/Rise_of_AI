#include "Death.h"
void Death::initialise() { mGameState.nextSceneID = NO_SCENE; mGameState.hinako = nullptr; mGameState.map = nullptr; }
void Death::update(float) {
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = gLives > 0 ? levelScene(gCurrentLevel) : MENU;
}
void Death::render() {
    ClearBackground(Color{24,20,43,255});
    DrawText("YOU DIED", 302, 185, 65, RAYWHITE);
    DrawText(TextFormat("HEARTS LEFT: %d", gLives), 384, 290, 25, PINK);
    DrawText(gLives > 0 ? "Enter: retry this level" : "Enter: return to menu", 355, 475, 23, LIGHTGRAY);
}
void Death::shutdown() {}
