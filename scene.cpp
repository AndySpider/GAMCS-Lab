#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "scene.h"
#include "block.h"
#include "cheese.h"
#include "nail.h"
#include "mouse.h"
#include "cat.h"
#include "config.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent), cur_tool(NONE), mouse_id(0), cat_id(0), timer_interval(100)
{
    setItemIndexMethod(BspTreeIndex);   // NoIndex is slow!

    showGrids();
    buildWalls();

    this->setSceneRect(-10, -10, SCENE_WIDTH *GRID_SIZE + 20, SCENE_HEIGHT * GRID_SIZE + 20);
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));
    timer->start(timer_interval);
}

Scene::~Scene()
{
    delete timer;
}

void Scene::step()
{
    // mice
    for (QList<Mouse *>::iterator it = mice.begin(); it != mice.end(); ++it)
    {
        (*it)->step();
    }
    // cats
    for (QList<Cat *>::iterator it = cats.begin(); it != cats.end(); ++it)
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
        block->setEnabled(false);
        this->addItem(block);
    }

    // the right wall, [w-5, 5] ~ [w-5, h-5]
    for (int i=0; i< SCENE_HEIGHT; i++)
    {
        Block *block = new Block();
        block->setPos((SCENE_WIDTH - 1) * GRID_SIZE, i*GRID_SIZE);
        block->setEnabled(false);
        this->addItem(block);
    }

    // the up wall, [15, 5] ~ [w-15, 5]
    for (int i=0; i < SCENE_WIDTH - 2; i++)
    {
        Block *block = new Block();
        block->setPos((i + 1) * GRID_SIZE, 0);
        block->setEnabled(false);
        this->addItem(block);
    }

    // the bottom wall, [15, h-5] ~ [w-15, h-5]
    for (int i = 0; i < SCENE_WIDTH - 2; i++)
    {
        Block *block = new Block();
        block->setPos((i + 1) * GRID_SIZE, (SCENE_HEIGHT - 1) * GRID_SIZE);
        block->setEnabled(false);
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

    if (timer->isActive())
        timer->start(timer_interval);
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

    if (timer->isActive())
        timer->start(timer_interval);
}

int Scene::miceNum()
{
    return mice.size();
}

int Scene::catsNum()
{
    return cats.size();
}

void Scene::useTool(const QPoint &pos)
{
    if (cur_tool == ERASER)
    {
        eraseToolAt(pos);
    }
    else if (cur_tool == BLOCK)
    {
        addToolAt(BLOCK, pos);
    }
    else if (cur_tool == CHEESE)
    {
        addToolAt(CHEESE, pos);
    }
    else if (cur_tool == NAIL)
    {
        addToolAt(NAIL, pos);
    }
    else if (cur_tool == MOUSE)
    {
        addToolAt(MOUSE, pos);
    }
    else if (cur_tool == CAT)
    {
        addToolAt(CAT, pos);
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
        QString item_name = item->data(0).toString();

        if (item_name == "Block")
        {
            eraseBlock(item);
        }
        else if (item_name == "Mouse")
        {
            eraseMouse(item);
        }
        else if (item_name == "Cheese")
        {
            eraseCheese(item);
        }
        else if (item_name == "Nail")
        {
            eraseNail(item);
        }
        else if (item_name == "Cat")
        {
            eraseCat(item);
        }
        // other tools come here
    }
}

void Scene::addToolAt(Tool tool, const QPoint &pos)
{
    // check if there's some tool already at pos
    QGraphicsItem *old_item = this->itemAt(pos.x(), pos.y(), QTransform());
    if (old_item == NULL) // no tool at pos, add directly
    {
        if (tool == BLOCK)
        {
            addBlockAt(pos);
        }
        else if (tool == CHEESE)
        {
            addCheeseAt(pos);
        }
        else if (tool == NAIL)
        {
            addNailAt(pos);
        }
        else if (tool == MOUSE)
        {
            addMouseAt(pos);
        }
        else if (tool == CAT)
        {
            addCatAt(pos);
        }
        else
        {
            qWarning() << "Unknown tool:" << tool;
        }
        // other tools come here
    }
    else 	// some tool is at pos
    {
        QString old_item_name = old_item->data(0).toString();
        if (old_item_name == "Block")
        {
            qDebug() << "can not add tools on a Block, erase it first!";
        }
        else if (old_item_name == "Cheese")
        {
            if (tool == BLOCK)	// erase cheese
            {
                eraseCheese(old_item);

                addBlockAt(pos);
            }
            else if (tool == CHEESE)	// supply
            {
                Cheese *cheese  = addCheeseAt(pos);
                cheese->amount->increase(dynamic_cast<Cheese *>(old_item)->amount->amount());
                eraseCheese(old_item);
            }
            else if (tool == NAIL)	// erase cheese
            {
                eraseCheese(old_item);

                addNailAt(pos);
            }
            else if (tool == MOUSE)	// add without erase
            {
                addMouseAt(pos);
            }
            else if (tool == CAT)	// add direct
            {
                addCatAt(pos);
            }
            // other tools come here
        }
        else if (old_item_name == "Nail")
        {
            if (tool == BLOCK)	// erase nail
            {
                eraseNail(old_item);
                addBlockAt(pos);
            }
            else if (tool == CHEESE)	// erase nail
            {
                eraseNail(old_item);
                addCheeseAt(pos);
            }
            else if (tool == NAIL)	// supply
            {
                Nail *nail = addNailAt(pos);
                nail->amount->increase(dynamic_cast<Nail *>(old_item)->amount->amount());
                eraseNail(old_item);
            }
            else if (tool == MOUSE) // add directly
            {
                addMouseAt(pos);
            }
            else if (tool == CAT)	// add directly
            {
                addCatAt(pos);
            }
            // other tools come here
        }
        else if (old_item_name == "Mouse")
        {
            if (tool == BLOCK)	// move mouse
            {
                bool found;
                QPoint npos = findAvatarNewPos(pos, &found);
                if (found)
                {
                    old_item->setPos(npos);
                }
                else
                {
                    eraseMouse(old_item);
                }

                addBlockAt(pos);
            }
            else if (tool == CHEESE)	// add directly
            {
                addCheeseAt(pos);
            }
            else if (tool == NAIL)	// add directly
            {
                addNailAt(pos);
            }
            else if (tool == MOUSE)	// do nothing
            {
                qDebug() << "can not add a mouse on a mouse, erase it first!";
            }
            else if (tool == CAT)	// add directly, cat likes mouse
            {
                addCatAt(pos);
            }
            // other tools come here
        }
        else if (old_item_name == "Cat")
        {
            if (tool == BLOCK)	// move cat
            {
                bool found;
                QPoint npos = findAvatarNewPos(pos, &found);
                if (found)
                {
                    old_item->setPos(npos);
                }
                else
                {
                    eraseCat(old_item);
                }

                addBlockAt(pos);
            }
            else if (tool == CHEESE)	// add directly
            {
                addCheeseAt(pos);
            }
            else if (tool == NAIL)	// add directly
            {
                addNailAt(pos);
            }
            else if (tool == MOUSE)	 // add directly
            {
                addMouseAt(pos);
            }
            else if (tool == CAT)	// do nothing
            {
                qDebug() << "can not add a cat on a cat, erase it first!";
            }
            // other tools come here
        }
        // other tools come here
    }
}

QPoint Scene::findAvatarNewPos(const QPoint &pos, bool *found)
{
    QGraphicsItem *item = NULL;
    QString item_name;
    int half_grid = GRID_SIZE / 2;
    *found = false;
    int x = pos.x();
    int y = pos.y();
    // check if there's space above
    item = this->itemAt(x + half_grid, y - half_grid, QTransform());
    item_name = item->data(0).toString();
    if (item == NULL || item_name == "" || item_name == "Cheese" || item_name == "Nail")	// not exit or invalid or Cheese or Nail, FIXME: Mouse vs Cat
    {
        *found = true;
        return QPoint(pos.x(), pos.y() - GRID_SIZE);
    }

    // check below
    item = this->itemAt(x + half_grid, y + GRID_SIZE + half_grid, QTransform());
    item_name = item->data(0).toString();
    if (item == NULL || item_name == "" || item_name == "Cheese" || item_name == "Nail")	// not exit or invalid or Cheese or Nail
    {
        *found = true;
        return QPoint(pos.x(), pos.y() + GRID_SIZE);
    }

    // check left
    item = this->itemAt(x - half_grid, y + half_grid,QTransform());
    item_name = item->data(0).toString();
    if (item == NULL || item_name == "" || item_name == "Cheese" || item_name == "Nail")	// not exit or invalid or Cheese or Nail
    {
        *found = true;
        return QPoint(pos.x() - GRID_SIZE, pos.y());
    }

    // check right
    item = this->itemAt(x + GRID_SIZE + half_grid, y + half_grid,QTransform());
    item_name = item->data(0).toString();
    if (item == NULL || item_name == "" || item_name == "Cheese" || item_name == "Nail")	// not exit or invalid or Cheese or Nail
    {
        *found = true;
        return QPoint(pos.x() + GRID_SIZE, pos.y());
    }

    // not found
    *found = false;
    return QPoint();
}

Block *Scene::addBlockAt(const QPoint &pos)
{
    Block *block = new Block();
    block->setPos(pos);
    this->addItem(block);

    return block;
}

Cheese *Scene::addCheeseAt(const QPoint &pos)
{
    Cheese *cheese = new Cheese();
    cheese->setPos(pos);
    this->addItem(cheese);

    return cheese;
}

Nail *Scene::addNailAt(const QPoint &pos)
{
    Nail *nail = new Nail();
    nail->setPos(pos);
    this->addItem(nail);

    return nail;
}

Mouse *Scene::addMouseAt(const QPoint &pos)
{
    Mouse *mouse = new Mouse(mouse_id++);
    mouse->setPos(pos);
    this->addItem(mouse);

    mouse->setZValue(1);	// stack before other tools
    mice.append(mouse);
    emit miceNumChanged(miceNum());

    return mouse;
}

Cat *Scene::addCatAt(const QPoint &pos)
{
    Cat *cat = new Cat(cat_id++);
    cat->setPos(pos);
    this->addItem(cat);

    cat->setZValue(2);	// before mouse
    cats.append(cat);
    emit catsNumChanged(catsNum());

    return cat;
}

void Scene::eraseBlock(QGraphicsItem *block)
{
    this->removeItem(block);
}

void Scene::eraseCheese(QGraphicsItem *cheese)
{
    this->removeItem(cheese);
}

void Scene::eraseNail(QGraphicsItem *nail)
{
    this->removeItem(nail);
}

void Scene::eraseMouse(QGraphicsItem *mouse)
{
    this->removeItem(mouse);

    mice.removeOne(dynamic_cast<Mouse *>(mouse));	// remove the mouse from list
    emit miceNumChanged(miceNum());
}

void Scene::eraseCat(QGraphicsItem *cat)
{
    this->removeItem(cat);

    cats.removeOne(dynamic_cast<Cat *>(cat));
    emit catsNumChanged(catsNum());
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
