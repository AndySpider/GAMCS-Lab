#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "scene.h"
#include "nail.h"

Nail::Nail()
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setData(0, "Nail");
}

QRectF Nail::boundingRect() const
{
    return QRectF(0, 0, 10, 10);    // the size of Nail
}

QPainterPath Nail::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 10, 10);
    return path;
}

void Nail::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor;
    fillColor = Qt::black;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.lighter();

    painter->setBrush(fillColor);
    painter->drawRect(0, 0, 10, 10);

    return;
}

void Nail::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Nail::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Scene *scene = dynamic_cast<Scene *>(this->scene());
    QPoint pos = scene->gridPoint(event->scenePos());
    this->setPos(pos);
    update();
}

void Nail::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

