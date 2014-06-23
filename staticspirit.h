#ifndef STATICSPIRIT_H
#define STATICSPIRIT_H
#include "spirit.h"

class StaticSpirit : public Spirit
{
public:
    StaticSpirit();
    virtual ~StaticSpirit();

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

#endif // STATICSPIRIT_H
