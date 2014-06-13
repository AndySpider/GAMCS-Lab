#ifndef NAIL_H
#define NAIL_H

#include <QGraphicsItem>
#include "amount.h"

class Nail : public QGraphicsItem
{
public:
    Nail();
    ~Nail();

    Amount *amount;

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
};

#endif // NAIL_H
