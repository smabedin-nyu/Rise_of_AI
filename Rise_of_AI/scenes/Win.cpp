#include "Win.h"
void Win::initialise() { mGameState.nextSceneID = NO_SCENE; mGameState.hinako = nullptr; mGameState.map = nullptr; }
void Win::update(float) { if (IsKeyPressed(KEY_ENTER)) mGameState.nextSceneID = MENU; }
void Win::render() {
    ClearBackground(Color{33,55,89,255});
    DrawText("You WIN!", 320, 205, 70, RAYWHITE);
    DrawText("Thank you for playing!", 340, 300, 25, SKYBLUE);
    DrawText("Press Enter to return to menu", 310, 405, 24, WHITE);
}
void Win::shutdown() {}
