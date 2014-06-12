#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "scene.h"
#include "block.h"
#include "cheese.h"
#include "nail.h"
#include "mouse.h"
#include "config.h"

// FIXME
const int SCENE_WIDTH = 800;
const int SCENE_HEIGHT = 600;
const int GRID_SIZE = 10;

Scene::Scene(QObject *parent) : QGraphicsScene(parent), cur_tool(NONE)
{
    buildWalls();

    this->setSceneRect(-10, -10, SCENE_WIDTH + 20, SCENE_HEIGHT + 20);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));
    timer->start(100);
}

Scene::~Scene()
{
    delete timer;
}

void Scene::step()
{
    for (QQueue<Mouse *>::iterator it = mice.begin(); it != mice.end(); ++it)
    {
        (*it)->step();
    }
}

void Scene::startMice()
{
    timer->start(100);
}

void Scene::pauseMice()
{
    timer->stop();
}

void Scene::resumeMice()
{
    timer->start(100);
}

void Scene::stopMice()
{
    timer->stop();
}

void Scene::buildWalls()
{
    int num;
    // the left wall, [0, 0] ~ [0, h-10]
    num = SCENE_HEIGHT / GRID_SIZE;
    for (int i =0; i< num; i++)
    {
        Block *block = new Block();
        block->setPos(0 , i*GRID_SIZE);
        this->addItem(block);
    }

    // the right wall, [w-5, 5] ~ [w-5, h-5]
    for (int i=0; i< num; i++)
    {
        Block *block = new Block();
        block->setPos(SCENE_WIDTH - GRID_SIZE, i*GRID_SIZE);
        this->addItem(block);
    }

    // the up wall, [15, 5] ~ [w-15, 5]
    num = (SCENE_WIDTH - 2 * GRID_SIZE) / GRID_SIZE;
    for (int i=0; i < num; i++)
    {
        Block *block = new Block();
        block->setPos((i + 1) * GRID_SIZE, 0);
        this->addItem(block);
    }

    // the bottom wall, [15, h-5] ~ [w-15, h-5]
    for (int i = 0; i < num; i++)
    {
        Block *block = new Block();
        block->setPos((i + 1) * GRID_SIZE, SCENE_HEIGHT - GRID_SIZE);
        this->addItem(block);
    }
}

void Scene::setCurTool(Tool t)
{
    cur_tool = t;
    qDebug() << "+++ Scene - change tool to " << cur_tool;
}

void Scene::useTool(const QPoint &pos)
{
    if (cur_tool == ERASER)
    {
        qDebug() << "use Eraser";
        eraseToolAt(pos);
    }
    else if (cur_tool == BLOCK)
    {
        qDebug() << "use Block";
        Block *block = new Block();
        addToolAt(block, pos);
    }
    else if (cur_tool == CHEESE)
    {
        qDebug() << "use Cheese";
        Cheese *cheese = new Cheese();
        addToolAt(cheese, pos);
    }
    else if (cur_tool == NAIL)
    {
        qDebug() << "use Nail";
        Nail *nail = new Nail();
        addToolAt(nail, pos);
    }
    else if (cur_tool == MOUSE)
    {
        qDebug() << "use Mouse";
        Mouse *mouse = new Mouse();
        addToolAt(mouse, pos);
    }
    else if (cur_tool == NONE)
    {
        //
    }
}

void Scene::eraseToolAt(const QPoint &pos)
{
    QGraphicsItem *item = this->itemAt(pos.x(), pos.y(), QTransform());
    if (item != NULL)	// has item
    {
        if (item->data(0).toString() == "Mouse")	// it's a mouse
        {
            bool val = mice.removeOne(dynamic_cast<Mouse *>(item));	// remove the mouse from queue
            qDebug() << "remove a mouse from mice: " << val;
        }
        // remove item from scene
        this->removeItem(item);
    }
}

void Scene::addToolAt(QGraphicsItem *item, const QPoint &pos)
{
    eraseToolAt(pos);	// erase first

    item->setPos(pos);
    this->addItem(item);

    if (item->data(0).toString() == "Mouse")
    {
        qDebug() << "add a mouse to mice!";
        mice.append(dynamic_cast<Mouse *>(item));
    }
}

/**
 * @brief Snap to grid.
 * @param pos
 * @return
 */
QPoint Scene::gridPoint(const QPointF &pos)
{
    int x = (int) pos.x();
    int y = (int) pos.y();

    int half_grid = GRID_SIZE / 2;

    int dx = x % GRID_SIZE;
    if (dx > half_grid)
        x = x - dx + GRID_SIZE;
    else
        x -= dx;

    int dy = y % GRID_SIZE;
    if (dy > half_grid)
        y = y - dy + GRID_SIZE;
    else
        y -= dy;

    return QPoint(x, y);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->useTool(gridPoint(event->scenePos()));
    }

    QGraphicsScene::mousePressEvent(event);
}
