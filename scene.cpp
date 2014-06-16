#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QFile>
#include "block.h"
#include "cheese.h"
#include "nail.h"
#include "mouse.h"
#include "cat.h"
#include "scene.h"
#include "config.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent), cur_tool(T_NONE), mouse_id(0), cat_id(0), timer_interval(100),
    num_blocks(0), num_cheeses(0), num_nails(0), num_mice(0), num_cats(0)
{
    setItemIndexMethod(BspTreeIndex);   // NoIndex is slow!

    this->setSceneRect(-10, -10, SCENE_WIDTH *GRID_SIZE + 20, SCENE_HEIGHT * GRID_SIZE + 20);
    this->init();

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
    QList<Spirit *> dead_spirits;

    for (QList<Spirit *>::iterator it = spirits.begin(); it != spirits.end(); ++it)
    {
        (*it)->act();

        if ((*it)->life() <= 0)	// remove spirit if it's dead
        {
            qDebug() << "spirit died!";
            dead_spirits.append(*it);
        }
    }

    // remove dead spirits
    for (QList<Spirit *>::iterator it = dead_spirits.begin(); it != dead_spirits.end(); ++it)
    {
        removeSpirit(*it);
    }

    timer->start(timer_interval);
}

int Scene::load(const QString &file)
{
    if (!QFile::exists(file))
        return -1;


    return 0;
}

void Scene::save(const QString &file)
{

}

void Scene::init()
{
    spirits.clear(); // clear spirits
    this->clear();  // clear items

    num_blocks = 0;
    num_cats = 0;
    num_cheeses = 0;
    num_mice = 0;
    num_nails = 0;

    mouse_id = 0;
    cat_id = 0;

    // build scene
    showGrids();
    buildWalls();
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

QList<SpiritInfo> Scene::statistics()
{
    QList<SpiritInfo> infos;

    SpiritInfo blocks_info;
    SpiritInfo cheeses_info;
    SpiritInfo nails_info;
    SpiritInfo mice_info;
    SpiritInfo cats_info;

    blocks_info.name = "Block";
    blocks_info.num = num_blocks;
    infos.append(blocks_info);

    cheeses_info.name = "Cheese";
    cheeses_info.num = num_cheeses;
    infos.append(cheeses_info);

    nails_info.name = "Nail";
    nails_info.num = num_nails;
    infos.append(nails_info);

    mice_info.name = "Mouse";
    mice_info.num = num_mice;
    infos.append(mice_info);

    cats_info.name = "Cat";
    cats_info.num = num_cats;
    infos.append(cats_info);

    return infos;
}

void Scene::useToolAt(const QPoint &pos)
{
    if (cur_tool == T_ERASER)
    {
        removeSpiritAt(pos);
    }
    else if (cur_tool == T_BLOCK)
    {
        addSpiritAt(Spirit::BLOCK, pos);
    }
    else if (cur_tool == T_CHEESE)
    {
        addSpiritAt(Spirit::CHEESE, pos);
    }
    else if (cur_tool == T_NAIL)
    {
        addSpiritAt(Spirit::NAIL, pos);
    }
    else if (cur_tool == T_MOUSE)
    {
        addSpiritAt(Spirit::MOUSE, pos);
    }
    else if (cur_tool == T_CAT)
    {
        addSpiritAt(Spirit::CAT, pos);
    }
    else if (cur_tool == T_NONE)
    {
        //
    }
}

void Scene::removeSpiritAt(const QPoint &pos)
{
    Spirit *spt = this->getSpiritAt(pos.x(), pos.y());

    if (spt == NULL) // not exist
    {
        return;
    }
    else    // found a tool
    {
        removeSpirit(spt);
    }
}

void Scene::addSpiritAt(Spirit::SType type, const QPoint &pos)
{
    // check if there's some type already at pos
    Spirit *old_spirit = getSpiritAt(pos.x(), pos.y());
    if (old_spirit == NULL) // no spirit at pos, add directly
    {
        if (type == Spirit::BLOCK)
        {
            newSpiritAt(Spirit::BLOCK, pos);
        }
        else if (type == Spirit::CHEESE)
        {
            newSpiritAt(Spirit::CHEESE, pos);
        }
        else if (type == Spirit::NAIL)
        {
            newSpiritAt(Spirit::NAIL, pos);
        }
        else if (type == Spirit::MOUSE)
        {
            newSpiritAt(Spirit::MOUSE, pos);
        }
        else if (type == Spirit::CAT)
        {
            newSpiritAt(Spirit::CAT, pos);
        }
        else
        {
            qWarning() << "Unknown type:" << type;
        }
        // other types come here
    }
    else 	// some type is at pos
    {
        Spirit::SType old_spirit_type = old_spirit->spiritType();
        if (old_spirit_type == Spirit::BLOCK)
        {
            qDebug() << "can not add types on a Block, erase it first!";
        }
        else if (old_spirit_type == Spirit::CHEESE)
        {
            if (type == Spirit::BLOCK)	// erase cheese
            {
                removeSpirit(old_spirit);

                newSpiritAt(Spirit::BLOCK, pos);
            }
            else if (type == Spirit::CHEESE)	// supply
            {
                Spirit *spt = newSpiritAt(Spirit::CHEESE, pos);
                spt->healed(old_spirit->life());
                removeSpirit(old_spirit);
            }
            else if (type == Spirit::NAIL)	// erase cheese
            {
                removeSpirit(old_spirit);
                newSpiritAt(Spirit::NAIL, pos);
            }
            else if (type == Spirit::MOUSE)	// add without erase
            {
                newSpiritAt(Spirit::MOUSE, pos);
            }
            else if (type == Spirit::CAT)	// add direct
            {
                newSpiritAt(Spirit::CAT, pos);
            }
            // other types come here
        }
        else if (old_spirit_type == Spirit::NAIL)
        {
            if (type == Spirit::BLOCK)	// erase nail
            {
                removeSpirit(old_spirit);
                newSpiritAt(Spirit::BLOCK, pos);
            }
            else if (type == Spirit::CHEESE)	// erase nail
            {
                removeSpirit(old_spirit);
                newSpiritAt(Spirit::CHEESE, pos);
            }
            else if (type == Spirit::NAIL)	// supply
            {
                Spirit *spt = newSpiritAt(Spirit::NAIL, pos);
                spt->healed(old_spirit->life());
                removeSpirit(old_spirit);
            }
            else if (type == Spirit::MOUSE) // add directly
            {
                newSpiritAt(Spirit::MOUSE, pos);
            }
            else if (type == Spirit::CAT)	// add directly
            {
                newSpiritAt(Spirit::CAT, pos);
            }
            // other types come here
        }
        else if (old_spirit_type == Spirit::MOUSE)
        {
            if (type == Spirit::BLOCK)	// move mouse
            {
                bool found;
                QPoint npos = findSpiritNewPos(pos, &found);
                if (found)
                {
                    old_spirit->setPos(npos);
                }
                else
                {
                    removeSpirit(old_spirit);
                }

                newSpiritAt(Spirit::BLOCK, pos);
            }
            else if (type == Spirit::CHEESE)	// add directly
            {
                newSpiritAt(Spirit::CHEESE, pos);
            }
            else if (type == Spirit::NAIL)	// add directly
            {
                newSpiritAt(Spirit::NAIL, pos);
            }
            else if (type == Spirit::MOUSE)	// do nothing
            {
                qDebug() << "can not add a mouse on a mouse, erase it first!";
            }
            else if (type == Spirit::CAT)	// add directly, cat likes mouse
            {
                newSpiritAt(Spirit::CAT, pos);
            }
            // other types come here
        }
        else if (old_spirit_type == Spirit::CAT)
        {
            if (type == Spirit::BLOCK)	// move cat
            {
                bool found;
                QPoint npos = findSpiritNewPos(pos, &found);
                if (found)
                {
                    old_spirit->setPos(npos);
                }
                else
                {
                    removeSpirit(old_spirit);
                }

                newSpiritAt(Spirit::BLOCK, pos);
            }
            else if (type == Spirit::CHEESE)	// add directly
            {
                newSpiritAt(Spirit::CHEESE, pos);
            }
            else if (type == Spirit::NAIL)	// add directly
            {
                newSpiritAt(Spirit::NAIL, pos);
            }
            else if (type == Spirit::MOUSE)	 // add directly
            {
                newSpiritAt(Spirit::MOUSE, pos);
            }
            else if (type == Spirit::CAT)	// do nothing
            {
                qDebug() << "can not add a cat on a cat, erase it first!";
            }
            // other types come here
        }
        // other types come here
    }
}

QPoint Scene::findSpiritNewPos(const QPoint &pos, bool *found)
{
    Spirit *spt = NULL;
    *found = false;
    // check if there's space above
    spt = this->getSpiritAt(pos.x(), pos.y() - GRID_SIZE);
    if (spt == NULL || spt->spiritType() != Spirit::BLOCK)	// not exit or not block
    {
        *found = true;
        return QPoint(pos.x(), pos.y() - GRID_SIZE);
    }

    // check right
    spt = this->getSpiritAt(pos.x() + GRID_SIZE, pos.y());
    if (spt == NULL || spt->spiritType() != Spirit::BLOCK)	// not exit or not block
    {
        *found = true;
        return QPoint(pos.x() + GRID_SIZE, pos.y());
    }

    // check below
    spt = this->getSpiritAt(pos.x(), pos.y() + GRID_SIZE);
    if (spt == NULL || spt->spiritType() != Spirit::BLOCK)	// not exit or not block
    {
        *found = true;
        return QPoint(pos.x(), pos.y() + GRID_SIZE);
    }

    // check left
    spt = this->getSpiritAt(pos.x() - GRID_SIZE, pos.y());
    if (spt == NULL || spt->spiritType() != Spirit::BLOCK)	// not exit or not block
    {
        *found = true;
        return QPoint(pos.x() - GRID_SIZE, pos.y());
    }

    // not found
    *found = false;
    return QPoint();
}

Spirit *Scene::newSpiritAt(Spirit::SType type, const QPoint &pos)
{
    Spirit *spt = NULL;
    int *num = NULL;

    if (type == Spirit::BLOCK)
    {
        spt = new Block();
        spt->setZValue(Spirit::BLOCK_Z);
        num = &num_blocks;
    }
    else if (type == Spirit::CHEESE)
    {
        spt = new Cheese();
        spt->setZValue(Spirit::CHEESE_Z);
        num = &num_cheeses;
    }
    else if (type == Spirit::NAIL)
    {
        spt = new Nail();
        spt->setZValue(Spirit::NAIL_Z);
        num = &num_nails;
    }
    else if (type == Spirit::MOUSE)
    {
        spt = new Mouse(mouse_id++);
        num = &num_mice;
        spt->setZValue(Spirit::MOUSE_Z);
    }
    else if (type == Spirit::CAT)
    {
        spt = new Cat(cat_id);
        num = &num_cats;
        spt->setZValue(Spirit::CAT_Z);
    }

    spt->setPos(pos);
    this->addItem(spt);
    int i = spt->initialize();		// initialize spirit
    if (i != 0)	// init failed
    {
        qDebug() << "Scene: failed to new Spirit at" << pos;
        this->removeItem(spt);
        return NULL;
    }

    spirits.append(spt);
    (*num)++;
    emit spiritsNumChanged(spirits.size());

    return spt;
}

void Scene::removeSpirit(Spirit *spt)
{
    Spirit::SType type = spt->spiritType();
    int *num = NULL;

    if (type == Spirit::BLOCK)
    {
        num = &num_blocks;
    }
    else if (type == Spirit::CHEESE)
    {
        num = &num_cheeses;
    }
    else if (type == Spirit::NAIL)
    {
        num = &num_nails;
    }
    else if (type == Spirit::MOUSE)
    {
        num = &num_mice;
    }
    else if (type == Spirit::CAT)
    {
        num = &num_cats;
    }

    this->removeItem(spt);
    spirits.removeOne(spt);
    (*num)--;
    emit spiritsNumChanged(spirits.size());
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

Spirit *Scene::getSpiritAt(int grid_x, int grid_y)
{
    qreal half_grid = GRID_SIZE / 2;
    qreal x = grid_x + half_grid;
    qreal y = grid_y + half_grid;

    QGraphicsItem *item = this->itemAt(x, y, QTransform());	 // the topmost item
    if (item == NULL)
    {
        return NULL;
    }
    else
    {
        Spirit *spt = qgraphicsitem_cast<Spirit *>(item);	// 0 if it's not a Spirit
        return spt;
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->useToolAt(gridPoint(event->scenePos()));
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() == Qt::CTRL)    // ctrl + move
    {
        this->useToolAt(gridPoint(event->scenePos()));
    }

    QGraphicsScene::mouseMoveEvent(event);
}
