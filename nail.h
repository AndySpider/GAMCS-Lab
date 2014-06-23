#ifndef NAIL_H
#define NAIL_H
#include "staticspirit.h"

class Nail : public StaticSpirit
{
public:
    Nail();
    ~Nail();

private:
    void act();
};

#endif // NAIL_H
