#ifndef SCENE_H
#define SCENE_H
#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include "spirit.h"
#include "channel.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

struct SpiritInfo;

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(QObject *parent);
    ~Scene();

    enum Tool
    {
        T_NONE = -1,
        T_ERASER,
        T_BLOCK,
        T_CHEESE,
        T_NAIL,
        T_BOMB,
        T_MOUSE,
        T_CAT,
        T_ELEPHANT
    };

    int load(const QString &file);
    void save(const QString &file);

    void clean();

    void pause();
    void resume();

    void setCurTool(Tool);
    void speedUp();
    void speedDown();
    bool empty();
    int width();
    int height();

    void genRandSpirit(int num);

    QList<SpiritInfo> statistics();

    QPoint calGridPos(const QPointF &);
    Spirit *getSpiritAt(int grid_x, int grid_y);
    bool outOfLimitLine(const QPoint &grid_pos);

signals:
    void spiritsNumChanged(int);
    void paused();
    void resumed();
    void currentSpeedLevel(int);

private slots:
    void step();

private:
    // current tool
    Tool cur_tool;

    // mice
    QList<Spirit *> spirits;

    int mouse_id;
    int cat_id;
    int elephant_id;

    // control
    QTimer *timer;
    int timer_interval;

private:
    int num_blocks;
    int num_cheeses;
    int num_nails;
    int num_bombs;
    int num_mice;
    int num_cats;
    int num_elephants;
    Channel *channel;
    int _width;
    int _height;

    void drawAxis();
    void drawLimitLine();

    void useToolAt(const QPoint &grid_pos);
    void addSpiritAt(Spirit::SType type, const QPoint &grid_pos);
    void removeSpiritAt(const QPoint &grid_pos);
    QPoint findAvatarNewPos(const QPoint &grid_pos, bool *found);

    Spirit *newSpiritAt(Spirit::SType type, const QPoint &grid_pos);
    void removeSpirit(Spirit *spt);

    void mousePressEvent(QGraphicsSceneMouseEvent  *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

struct SpiritInfo
{
    QString name;
    int num;
};

#endif // SCENE_H
