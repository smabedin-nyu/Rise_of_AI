#include "Menu.h"
void Menu::initialise() {
    mGameState.nextSceneID = NO_SCENE;
    mGameState.hinako = nullptr;
    mGameState.map = nullptr;
}
void Menu::update(float) {
    Rectangle button = { 385, 340, 230, 100 };
    if (IsKeyPressed(KEY_ENTER)) {
        gLives = 3;
        gCurrentLevel = 1;
        mGameState.nextSceneID = LEVEL1;
    }
}
void Menu::render() {
    ClearBackground(Color{ 125, 200, 245, 255 });
    DrawText("RISE OF AI", 285, 125, 70, Color{ 31, 63, 88, 255 });
    DrawText("Press ENTER to Start or ESC to Quit", 275, 465, 23, DARKBLUE);
    DrawText("A / D: Move     W: Jump     1 / 2 / 3: Level     R: Restart Level", 210, 520, 20, DARKBLUE);
}
void Menu::shutdown(){}