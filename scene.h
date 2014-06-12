#ifndef SCENE_H
#define SCENE_H
#include <QGraphicsScene>
#include <QTimer>
#include <QQueue>

class Mouse;

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
};

#endif // SCENE_H
