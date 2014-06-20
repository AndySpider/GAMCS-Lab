#ifndef MOUSE_H
#define MOUSE_H
#include "spirit.h"
#include "comavatar.h"

class Mouse : public Spirit, public ComAvatar
{
public:
    Mouse(int id);
    ~Mouse();
    void act();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    Agent::State perceiveState();
    void performAction(Agent::Action);
    OSpace availableActions(Agent::State);
    float originalPayoff(Agent::State);

private:
    QString storage;
};

#endif // MOUSE_H
