#ifndef SCENE_H
#define SCENE_H
#include <QGraphicsScene>
#include <QTimer>
#include <QQueue>

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

    void startMice();
    void pauseMice();
    void resumeMice();
    void stopMice();

    void setCurTool(Tool);
    void useTool(const QPoint &);

private slots:
    void step();

private:
    // current tool
    Tool cur_tool;

    // mice
    QQueue<Mouse *> mice;

    // control
    QTimer *timer;

private:
    void buildWalls();
    void eraseToolAt(const QPoint &);
    void addToolAt(QGraphicsItem *item, const QPoint &);
    QPoint gridPoint(const QPointF &);

    void mousePressEvent(QGraphicsSceneMouseEvent  *event);
};

#endif // SCENE_H
