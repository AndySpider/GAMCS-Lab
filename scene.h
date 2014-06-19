#ifndef SCENE_H
#define SCENE_H
#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include "spirit.h"
#include "comavatar.h"
#include "channel.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

struct SpiritInfo;
struct Message;

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
        T_MOUSE,
        T_CAT
    };

    enum GameMode
    {
        DEAD = 0,
        UNDEAD
    };

    int load(const QString &file);
    void save(const QString &file);

    void init();

    void pause();
    void resume();

    void setCurTool(Tool);
    void speedUp();
    void speedDown();
    void setGameMode(GameMode);

    void genRandSpirit(int num);

    QList<SpiritInfo> statistics();

    QPoint calGridPos(const QPointF &);
    Spirit *getSpiritAt(int grid_x, int grid_y);

signals:
    void spiritsNumChanged(int);

private slots:
    void step();

private:
    // current tool
    Tool cur_tool;

    // mice
    QList<Spirit *> spirits;

    int mouse_id;
    int cat_id;

    // control
    QTimer *timer;
    int timer_interval;

private:
    int num_blocks;
    int num_cheeses;
    int num_nails;
    int num_mice;
    int num_cats;
    GameMode game_mode;
    Channel *channel;

    void buildWalls();
    void showGrids();

    void useToolAt(const QPoint &grid_pos);
    void addSpiritAt(Spirit::SType type, const QPoint &grid_pos);
    void removeSpiritAt(const QPoint &grid_pos);
    QPoint findSpiritNewPos(const QPoint &grid_pos, bool *found);

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
