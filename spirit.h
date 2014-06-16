#ifndef SPIRIT_H
#define SPIRIT_H

#include <QGraphicsItem>
#include <QMutex>
#include <QColor>

class Scene;

class Spirit : public QGraphicsItem
{
public:
    Spirit();
    virtual ~Spirit();

    // Spirit Type
    enum SType
    {
        BLOCK = 0,
        CHEESE,
        NAIL,
        MOUSE,
        CAT
    };

    // stack priority, or z value
    static const int CAT_Z = 2;
    static const int MOUSE_Z = 1;
    static const int NAIL_Z = 0;
    static const int BLOCK_Z = 0;
    static const int CHEESE_Z = 0;

    // QGraphicsItem type
    enum { Type = UserType + 1};

    int initialize();	// initialize scene and grid_x, grid_y

    SType spiritType();
    float life();
    void injured(float l);
    void healed(float l);

    virtual void act();

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual int type() const;

protected:
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    QList<Spirit *> collidingSpirits();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

protected:
    float _life;
    QMutex _life_mutex;
    SType _type;
    QColor _color;

    Scene *myscene;
    int grid_x;
    int grid_y;
};

#endif // SPIRIT_H
