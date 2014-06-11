#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPointF>
#include <QTransform>
#include <QDebug>
#include "mouse.h"

Mouse::Mouse(QGraphicsScene *s) : scene(s)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setData(0, "Mouse");
}

QRectF Mouse::boundingRect() const
{
    return QRectF(0, 0, 10, 10);    // the size of mouse
}

QPainterPath Mouse::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 10, 10);
    return path;
}

void Mouse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor;
    if (option->state & QStyle::State_MouseOver)
        fillColor = Qt::green;
    else
        fillColor = Qt::blue;

    painter->setBrush(fillColor);
    painter->drawRect(0, 0, 10, 10);

    return;
}

void Mouse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Mouse::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    update();
}

void Mouse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

Agent::State Mouse::perceiveState()
{
    QPointF pos = this->pos();

    // x : [0, 80], y: [0, 60]
    qreal st = pos.x();
    st += pos.y() * 80;

    return (int) st;
}

void Mouse::performAction(Agent::Action act)
{
    QPointF pos = this->pos();
    qreal x = pos.x();
    qreal y = pos.y();

    QGraphicsItem *item = NULL;

    switch (act)
    {
    case 1: // move up
        // check if there're blocks
        item = scene->itemAt(x, y-10, QTransform());
        if (item == NULL || item->data(0).toString() != "Block")
            y -= 10;
        break;
    case 2: // move down
        item = scene->itemAt(x, y+10, QTransform());
        if (item == NULL || item->data(0).toString() != "Block")
            y += 10;
        break;
    case 3: // move left
        item = scene->itemAt(x-10, y,QTransform());
        if (item == NULL || item->data(0).toString() != "Block")
            x -= 10;
        break;
    case 4: // move right
        item = scene->itemAt(x+10, y,QTransform());
        if (item == NULL || item->data(0).toString() != "Block")
            x += 10;
        break;
    }

    this->setPos(x, y);
}

OSpace Mouse::availableActions(Agent::State st)
{
    OSpace acts;
    acts.add(1, 4, 1);
    return acts;
}

float Mouse::originalPayoff(Agent::State st)
{
    return 0;
}
