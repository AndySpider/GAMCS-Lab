#ifndef ELEPHANT_H
#define ELEPHANT_H
#include "avatarspirit.h"

class Elephant : public AvatarSpirit
{
public:
    Elephant(int id);
    ~Elephant();

private:
    float originalPayoff(Agent::State);
};

#endif // ELEPHANT_H
