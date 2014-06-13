#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "scene.h"
#include "block.h"
#include "cheese.h"
#include "nail.h"
#include "mouse.h"
#include "config.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent), cur_tool(NONE), mouse_id(0), timer_interval(100)
{
    setItemIndexMethod(BspTreeIndex);   // NoIndex is slow!

    showGrids();
    buildWalls();

    this->setSceneRect(-10, -10, SCENE_WIDTH *GRID_SIZE + 20, SCENE_HEIGHT * GRID_SIZE + 20);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));
    timer->start(timer_interval);
}

Scene::~Scene()
{
    delete timer;
}

void Scene::step()
{
    for (QList<Mouse *>::iterator it = mice.begin(); it != mice.end(); ++it)
    {
        (*it)->step();
    }
    timer->start(timer_interval);
}

int Scene::open(const QString &file)
{

}

void Scene::save(const QString &file)
{

}

void Scene::pause()
{
    timer->stop();
}

void Scene::resume()
{
    timer->start(timer_interval);
}

void Scene::buildWalls()
{
    // the left wall, [0, 0] ~ [0, h-10]
    for (int i =0; i< SCENE_HEIGHT; i++)
    {
        Block *block = new Block();
        block->setPos(0 , i*GRID_SIZE);
        this->addItem(block);
    }

    // the right wall, [w-5, 5] ~ [w-5, h-5]
    for (int i=0; i< SCENE_HEIGHT; i++)
    {
        Block *block = new Block();
        block->setPos((SCENE_WIDTH - 1) * GRID_SIZE, i*GRID_SIZE);
        this->addItem(block);
    }

    // the up wall, [15, 5] ~ [w-15, 5]
    for (int i=0; i < SCENE_WIDTH - 2; i++)
    {
        Block *block = new Block();
        block->setPos((i + 1) * GRID_SIZE, 0);
        this->addItem(block);
    }

    // the bottom wall, [15, h-5] ~ [w-15, h-5]
    for (int i = 0; i < SCENE_WIDTH - 2; i++)
    {
        Block *block = new Block();
        block->setPos((i + 1) * GRID_SIZE, (SCENE_HEIGHT - 1) * GRID_SIZE);
        this->addItem(block);
    }
}

void Scene::showGrids()
{
    QRectF dot_sz = QRectF(0, 0, 0.2, 0.2);
    QPen pen = QPen(Qt::black);
    QBrush brush = QBrush(Qt::SolidPattern);
    QGraphicsEllipseItem *dot = NULL;

     for (int i = 0; i < SCENE_HEIGHT; i++)
     {
         for (int j = 0; j < SCENE_WIDTH; j++)
         {
             dot = this->addEllipse(dot_sz, pen, brush);
             dot->setZValue(-10000);     // at the most background!
             dot->setPos(j * GRID_SIZE, i * GRID_SIZE);
             dot->setEnabled(false);    // no need to be enabled
         }
     }
}

void Scene::setCurTool(Tool t)
{
    cur_tool = t;
    qDebug() << "+++ Scene - change tool to " << cur_tool;
}

void Scene::speedUp()
{
    switch (timer_interval)
    {
    case 1000:
        timer_interval = 500;
        break;
    case 500:
        timer_interval = 200;
        break;
    case 200:
        timer_interval = 100;
        break;
    case 100:
        timer_interval = 50;
        break;
    case 50:
        timer_interval = 20;
        break;
    case 20:
        timer_interval = 10;
        break;
    case 10:
        timer_interval = 5;
        break;
    case 5:
        timer_interval = 1;
        break;
    case 1:
        qDebug() << "It's the maximum speed!";
        break;
    default:
        qWarning() << "!!! The timer interval is out of control!";
    }
}

void Scene::speedDown()
{
    switch (timer_interval)
    {
    case 1:
        timer_interval = 5;
        break;
    case 5:
        timer_interval = 10;
        break;
    case 10:
        timer_interval = 20;
        break;
    case 20:
        timer_interval = 50;
        break;
    case 50:
        timer_interval = 100;
        break;
    case 100:
        timer_interval = 200;
        break;
    case 200:
        timer_interval = 500;
        break;
    case 500:
        timer_interval = 1000;
        break;
    case 1000:
        qDebug() << "It's the minimum speed!";
        break;
    default:
        qWarning() << "!!! The timer interval is out of control!";
    }
}

int Scene::miceNum()
{
    return mice.size();
}

void Scene::useTool(const QPoint &pos)
{
    if (cur_tool == ERASER)
    {
        eraseToolAt(pos);
    }
    else if (cur_tool == BLOCK)
    {
        Block *block = new Block();
        addToolAt(block, pos);
    }
    else if (cur_tool == CHEESE)
    {
        Cheese *cheese = new Cheese();
        addToolAt(cheese, pos);
    }
    else if (cur_tool == NAIL)
    {
        Nail *nail = new Nail();
        addToolAt(nail, pos);
    }
    else if (cur_tool == MOUSE)
    {
        Mouse *mouse = new Mouse(mouse_id++);
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

    if (item == NULL || item->data(0).toString() == "") // not exits or not a valid tool
    {
        return;
    }
    else    // found a tool
    {
        if (item->data(0).toString() == "Mouse")	// it's a mouse
        {
            mice.removeOne(dynamic_cast<Mouse *>(item));	// remove the mouse from queue
            emit miceNumChanged(miceNum());
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

    QString item_name = item->data(0).toString();

    if (item_name == "Mouse")
    {
        mice.append(dynamic_cast<Mouse *>(item));
        emit miceNumChanged(miceNum());
    }
    else if (item_name == "Cheese" || item_name == "Nail")
    {
        item->setZValue(-10);  // stack after other tools

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

    int dx = x % GRID_SIZE;
    int dy = y % GRID_SIZE;

    x -= dx;
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

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() == Qt::CTRL)    // ctrl + move
    {
        this->useTool(gridPoint(event->scenePos()));
    }

    QGraphicsScene::mouseMoveEvent(event);
}
