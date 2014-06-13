#ifndef SCENE_H
#define SCENE_H
#include <QGraphicsScene>
#include <QTimer>
#include <QList>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Mouse;
class Cheese;
class Block;
class Nail;
class Cat;

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(QObject *parent);
    ~Scene();

    enum Tool
    {
        NONE = -1,
        ERASER,
        BLOCK,
        CHEESE,
        NAIL,
        MOUSE,
        CAT
    };

    friend class Mouse;
    friend class Cheese;
    friend class Block;
    friend class Nail;
    friend class Cat;

    int open(const QString &file);
    void save(const QString &file);

    void pause();
    void resume();

    void setCurTool(Tool);
    void speedUp();
    void speedDown();

    int miceNum();
    int catsNum();

signals:
    void miceNumChanged(int);
    void catsNumChanged(int);

private slots:
    void step();

private:
    // current tool
    Tool cur_tool;

    // mice
    QList<Mouse *> mice;
    int mouse_id;

    // cats
    QList<Cat *> cats;
    int cat_id;

    // control
    QTimer *timer;
    int timer_interval;

private:
    void buildWalls();
    void showGrids();

    void useTool(const QPoint &);
    void eraseToolAt(const QPoint &);
    void addToolAt(Tool tool, const QPoint &);
    QPoint gridPoint(const QPointF &);
    QPoint findAvatarNewPos(const QPoint &pos, bool *found);

    Block *addBlockAt(const QPoint &);
    Cheese *addCheeseAt(const QPoint &);
    Nail *addNailAt(const QPoint &);
    Mouse *addMouseAt(const QPoint &);
    Cat *addCatAt(const QPoint &);
    void eraseBlock(QGraphicsItem *block);
    void eraseCheese(QGraphicsItem *cheese);
    void eraseNail(QGraphicsItem *nail);
    void eraseMouse(QGraphicsItem *mouse);
    void eraseCat(QGraphicsItem *cat);

    void mousePressEvent(QGraphicsSceneMouseEvent  *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENE_H
