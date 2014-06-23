#ifndef ABYSS_H
#define ABYSS_H
#include "staticspirit.h"

class Bomb : public StaticSpirit
{
public:
    Bomb();
    ~Bomb();

private:
    void act();
};

#endif // ABYSS_H
