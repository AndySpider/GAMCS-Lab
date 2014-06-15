#ifndef CAT_H
#define CAT_H

#include <gamcs/Avatar.h>
#include <gamcs/CSOSAgent.h>

using namespace gamcs;

class Cat : public Spirit, public Avatar
{
public:
    Cat(int id);
    ~Cat();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void act();

private:
    Agent::State perceiveState();
    void performAction(Agent::Action);
    OSpace availableActions(Agent::State);
    float originalPayoff(Agent::State);

private:
    CSOSAgent *myagent;
};

#endif // CAT_H
