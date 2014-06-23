#ifndef CHEESE_H
#define CHEESE_H
#include "staticspirit.h"

class Cheese : public StaticSpirit
{
public:
    Cheese();
    ~Cheese();

private:
    void act();
};

#endif // CHEESE_H
