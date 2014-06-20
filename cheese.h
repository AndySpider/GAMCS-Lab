#ifndef CHEESE_H
#define CHEESE_H

#include "spirit.h"

class Cheese : public Spirit
{
public:
    Cheese();
    ~Cheese();

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    void act();
};

#endif // CHEESE_H
