#ifndef MOUSE_H
#define MOUSE_H
#include "avatarspirit.h"

class Mouse : public AvatarSpirit
{
public:
    Mouse(int id);
    ~Mouse();

private:
    float originalPayoff(Agent::State);
};

#endif // MOUSE_H
