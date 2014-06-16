#ifndef MOUSE_H
#define MOUSE_H

#include <gamcs/Avatar.h>
#include <gamcs/CSOSAgent.h>

using namespace gamcs;

class Mouse : public Spirit, public Avatar
{
public:
    Mouse(int id);
    ~Mouse();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void act();

private:
    Agent::State perceiveState();
    void performAction(Agent::Action);
    OSpace availableActions(Agent::State);
    float originalPayoff(Agent::State);

private:
    CSOSAgent *myagent;
    Agent::Mode learning_mode;
};

#endif // MOUSE_H
