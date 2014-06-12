#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "cheese.h"

Cheese::Cheese()
{
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
    update();
}

void Cheese::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

