#ifndef MENU_H
#define MENU_H
#include "../GameShared.h"
class Menu : public Scene {
    Texture2D mButton{};
    Texture2D mPlayer{};
public:
    void initialise() override;
    void update(float dt) override;
    void render() override;
    void shutdown() override;
};
#endif
