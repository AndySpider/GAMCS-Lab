#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsItem>

class Block : public QGraphicsItem
{
public:
    explicit Block();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:

public slots:

};

#endif // BLOCK_H
