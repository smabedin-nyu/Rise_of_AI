#ifndef DEATH_H
#define DEATH_H
#include "../GameShared.h"
class Death : public Scene {
public:
    void initialise() override;
    void update(float dt) override;
    void render() override;
    void shutdown() override;
};
#endif
