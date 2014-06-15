#ifndef CHEESE_H
#define CHEESE_H

#include "spirit.h"

class Cheese : public Spirit
{
public:
    Cheese();
    ~Cheese();

private:
    void act();
};

#endif // CHEESE_H
