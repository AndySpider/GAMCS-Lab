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
    int getId();
    void setRadarRange(int range);
    int getRadarRange();
    void setAwake(bool val);
    bool isAwake();
    void setLearningMode(Agent::Mode lm);
    Agent::Mode getLearningMode();
    void setChannel(Channel *c);
    void setComFreq(int f);
    int getComFreq();

    void sendMsg(AvatarSpirit *receiver, State_Info_Header *stif);
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

    QList<AvatarSpirit *> getNeighbors();	// the area is a rectangle
    bool timeToCom();

    Channel *mychannel;
    MemHandler *memhandler;
    CSOSAgent *myagent;
    Agent::Mode learning_mode;

private:
    QPoint changePos();

    int tmp_delta_grid_x;   // this is used to postporn the moving action
    int tmp_delta_grid_y;
    int radar_range;
    bool is_awake;
    bool is_sending;
    QString storage;
    int com_freq;
    long com_count;
};

#endif // AVATARSPIRIT_H
