#ifndef MOUSE_H
#define MOUSE_H

#include <QGraphicsItem>
#include <gamcs/Avatar.h>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

using namespace gamcs;

class Mouse : public QGraphicsItem, public Avatar
{
public:
    Mouse(QGraphicsScene *scene);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Agent::State perceiveState();
    void performAction(Agent::Action);
    OSpace availableActions(Agent::State);
    float originalPayoff(Agent::State);

private:
    QGraphicsScene *scene;
};

#endif // MOUSE_H