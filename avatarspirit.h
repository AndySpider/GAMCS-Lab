#ifndef AVATARSPIRIT_H
#define AVATARSPIRIT_H
#include <gamcs/Avatar.h>
#include <gamcs/CSOSAgent.h>
#include "spirit.h"

class Channel;
class MemHandler;

using namespace gamcs;

class AvatarSpirit : public Spirit, public Avatar
{
public:
    AvatarSpirit(int id);
    virtual ~AvatarSpirit();

    void act();
    void postAct();

    void setId(int i);
    int getId() const;
    void setShareRange(int range);
    int getShareRange() const;
    void setAwake(bool val);
    bool isAwake() const;
    void setLearningMode(Agent::Mode lm);
    Agent::Mode getLearningMode() const;
    void setChannel(Channel *c);
    void setShareFreq(int f);
    int getShareFreq() const;

    void sendMsg(AvatarSpirit *receiver, const State_Info_Header *stif);
    void recvMsg(const State_Info_Header *recstif);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    Agent::State perceiveState();
    void performAction(Agent::Action);
    OSpace availableActions(Agent::State);
    float originalPayoff(Agent::State);

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    QList<AvatarSpirit *> getNeighbors() const;	// the area is a rectangle
    bool timeToShare();

    Channel *mychannel;
    MemHandler *memhandler;
    CSOSAgent *myagent;
    Agent::Mode learning_mode;

private:
    QPoint changePos();
    State_Info_Header *mergeStateInfo(const State_Info_Header *stif1, const State_Info_Header *stif2);

    int tmp_delta_grid_x;   // this is used to postporn the moving action
    int tmp_delta_grid_y;
    int share_range;
    bool is_awake;
    bool is_sending;
    QString storage;
    int share_freq;
    long _counter;
    float discount_rate;
    float accuracy;
};

#endif // AVATARSPIRIT_H
