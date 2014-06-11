#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include "spirit.h"

Spirit::Spirit()
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF Spirit::boundingRect() const
{
    return QRectF(0, 0, 10, 10);    // the size of mouse
}

QPainterPath Spirit::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 10, 10);
    return path;
}

void Spirit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void Spirit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Spirit::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    update();
}

void Spirit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
