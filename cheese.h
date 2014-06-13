#ifndef CHEESE_H
#define CHEESE_H

#include <QGraphicsItem>
#include <QMutex>

class Cheese : public QGraphicsItem
{
public:
    Cheese();

    int eaten();
    int getAmount();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

private:
    int amount;
    QMutex mutex;
};

#endif // CHEESE_H
