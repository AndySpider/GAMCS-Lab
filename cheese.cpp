#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QDebug>
#include "scene.h"
#include "cheese.h"
#include "config.h"
#include "amount.h"

Cheese::Cheese()
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setData(0, "Cheese");

    amount = new Amount(this, 10);
}

Cheese::~Cheese()
{
    delete amount;
}

QRectF Cheese::boundingRect() const
{
    return QRectF(0, 0, GRID_SIZE, GRID_SIZE);    // the size of Cheese
}

QPainterPath Cheese::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, GRID_SIZE, GRID_SIZE);
    return path;
}

void Cheese::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor;
    fillColor = QColor(255,204,0);
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.lighter();

    painter->setBrush(fillColor);
    painter->drawRect(0, 0, GRID_SIZE, GRID_SIZE);

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

void Cheese::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    QString tips;
    QTextStream(&tips) << "amount: " << amount->amount();
    setToolTip(tips);
}
