#ifndef CAT_H
#define CAT_H
#include "spirit.h"
#include "comavatar.h"

class Cat : public Spirit, public ComAvatar
{
public:
    Cat(int id);
    ~Cat();

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
    QString storage;
};

#endif // CAT_H
