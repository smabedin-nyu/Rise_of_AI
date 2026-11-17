#ifndef WIN_H
#define WIN_H
#include "../GameShared.h"
class Win : public Scene {
public:
    void initialise() override;
    void update(float dt) override;
    void render() override;
    void shutdown() override;
};
#endif
