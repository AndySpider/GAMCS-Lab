#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "block.h"

Block::Block()
{
}

QRectF Block::boundingRect() const
{
    return QRectF(0, 0, 10, 10);    // the size of block
}

QPainterPath Block::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 10, 10);
    return path;
}

void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor;
    fillColor = Qt::gray;

    painter->setBrush(fillColor);
    painter->drawRect(0, 0, 10, 10);

    return;
}

void Block::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Block::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    update();
}

void Block::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
