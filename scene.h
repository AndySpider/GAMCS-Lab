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
        MOUSE
    };

    friend class Mouse;
    friend class Cheese;
    friend class Block;
    friend class Nail;

    int open(const QString &file);
    void save(const QString &file);

    void pause();
    void resume();

    void setCurTool(Tool);
    void speedUp();
    void speedDown();

    int miceNum();

signals:
    void miceNumChanged(int);

private slots:
    void step();

private:
    // current tool
    Tool cur_tool;

    // mice
    QList<Mouse *> mice;
    int mouse_id;

    // control
    QTimer *timer;
    int timer_interval;

private:
    void buildWalls();
    void showGrids();

    void useTool(const QPoint &);
    void eraseToolAt(const QPoint &);
    void addToolAt(QGraphicsItem *item, const QPoint &);
    QPoint gridPoint(const QPointF &);

    void mousePressEvent(QGraphicsSceneMouseEvent  *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENE_H
