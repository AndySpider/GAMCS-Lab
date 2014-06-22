#ifndef CAT_H
#define CAT_H
#include "avatarspirit.h"

class Cat : public AvatarSpirit
{
public:
    Cat(int id);
    ~Cat();

private:
    float originalPayoff(Agent::State);
};

#endif // CAT_H
