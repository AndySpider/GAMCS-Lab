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
        BOMB,
        MOUSE,
        CAT,
        ELEPHANT
    };

    enum SCategory
    {
        STATICSPIRIT = 0,
        AVATARSPIRIT
    };

    // stack priority, or z value
    static const int ELEPHANT_Z = 3;
    static const int CAT_Z = 2;
    static const int MOUSE_Z = 1;
    static const int BOMB_Z = 0;
    static const int NAIL_Z = 0;
    static const int BLOCK_Z = 0;
    static const int CHEESE_Z = 0;

    static const int SPIRIT_NUM = 7;

    // QGraphicsItem type
    enum { Type = UserType + 1};

    int initialize();	// initialize scene and grid_x, grid_y

    SType spiritType();
    SCategory spiritCategory();
    float life();
    void setLife(float l);
    void injured(float l);
    void healed(float l);
    QPoint gridPos();

    void setMarked(bool val);
    bool isMarked();

    virtual void act();
    virtual void postAct();

    static QString stypeToString(SType stype);
    static SType stringToSType(const QString &str);

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual int type() const;
    bool collidesWithItem(const QGraphicsItem * other, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape) const;

protected:
    QList<Spirit *> collidingSpirits();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

protected:
    float _life;
    SType _type;
    SCategory _category;
    QColor _color;

    Scene *myscene;
    int grid_x;
    int grid_y;

private:
    QMutex _life_mutex;
    bool is_marked;
};

#endif // SPIRIT_H
