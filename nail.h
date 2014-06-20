#ifndef NAIL_H
#define NAIL_H

#include "spirit.h"

class Nail : public Spirit
{
public:
    Nail();
    ~Nail();

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    void act();
};

#endif // NAIL_H
