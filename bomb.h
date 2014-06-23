#ifndef ABYSS_H
#define ABYSS_H
#include "spirit.h"

class Bomb : public Spirit
{
public:
    Bomb();
    ~Bomb();

private:
    void act();
};

#endif // ABYSS_H
