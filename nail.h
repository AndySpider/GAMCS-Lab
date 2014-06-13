#ifndef NAIL_H
#define NAIL_H

#include <QGraphicsItem>

class Nail : public QGraphicsItem
{
public:
    Nail();

    int getPins();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

private:
    int pins;
};

#endif // NAIL_H
