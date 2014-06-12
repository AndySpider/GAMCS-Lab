#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "scene.h"
#include "cheese.h"

Cheese::Cheese()
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setData(0, "Cheese");
}


QRectF Cheese::boundingRect() const
{
    return QRectF(0, 0, 10, 10);    // the size of Cheese
}

QPainterPath Cheese::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 10, 10);
    return path;
}

void Cheese::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor;
    fillColor = Qt::yellow;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.lighter();

    painter->setBrush(fillColor);
    painter->drawRect(0, 0, 10, 10);

    return;
}

void Cheese::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Cheese::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Scene *scene = dynamic_cast<Scene *>(this->scene());
    QPoint pos = scene->gridPoint(event->scenePos());
    this->setPos(pos);
    update();
}

void Cheese::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

