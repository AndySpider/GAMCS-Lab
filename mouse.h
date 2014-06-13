#ifndef MOUSE_H
#define MOUSE_H

#include <QGraphicsItem>
#include <gamcs/Avatar.h>
#include <gamcs/CSOSAgent.h>
#include "amount.h"

using namespace gamcs;

class Mouse : public QGraphicsItem, public Avatar
{
public:
    Mouse(int id);
    ~Mouse();

    Amount *amount;

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

private:
    Agent::State perceiveState();
    void performAction(Agent::Action);
    OSpace availableActions(Agent::State);
    float originalPayoff(Agent::State);

private:
    CSOSAgent *myagent;
};

#endif // MOUSE_H
