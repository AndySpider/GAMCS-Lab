#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QFile>
#include <assert.h>
#include "avatarspirit.h"
#include "block.h"
#include "cheese.h"
#include "nail.h"
#include "mouse.h"
#include "cat.h"
#include "elephant.h"
#include "scene.h"
#include "config.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent), cur_tool(T_NONE), mouse_id(0), cat_id(0), elephant_id(0), timer_interval(100),
    num_blocks(0), num_cheeses(0), num_nails(0), num_mice(0), num_cats(0), num_elephants(0), game_mode(DEAD)
{
    setItemIndexMethod(BspTreeIndex);   // NoIndex is slow!

    this->setSceneRect(-10, -10, SCENE_WIDTH *GRID_SIZE + 20, SCENE_HEIGHT * GRID_SIZE + 20);
    this->init();

    // initialize channel
    channel = new Channel();

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));
    timer->start(timer_interval);
}

Scene::~Scene()
{
    delete timer;
    delete channel;
}

void Scene::step()
{
    QList<Spirit *> dead_spirits;

    for (QList<Spirit *>::iterator it = spirits.begin(); it != spirits.end(); ++it)
    {
        (*it)->act();

        if (game_mode == DEAD)  // collect dead bodies
        {
            if ((*it)->life() <= 0)	// remove spirit if it's dead
            {
                dead_spirits.append(*it);   // record dead spirits
            }
        }
    }

    // dispatch messages (before any spirit is removed)
    channel->dispatchMsg();

    // remove dead spirits
    for (QList<Spirit *>::iterator it = dead_spirits.begin(); it != dead_spirits.end(); ++it)
    {
        qDebug() << "dead spirit to be removed!";
        removeSpirit(*it);
    }

    // post actions: eg. the position are changed after all spirits have taken actions
    for (QList<Spirit *>::iterator it = spirits.begin(); it != spirits.end(); ++it)
    {
        (*it)->postAct();
    }

    update();	// update scene
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
    num_elephants = 0;

    elephant_id = 0;
    mouse_id = 0;
    cat_id = 0;

    // build scene
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

void Scene::setGameMode(GameMode mode)
{
    game_mode = mode;
    qDebug() << "+++ Scene - change game mode to " << game_mode;
}

void Scene::genRandSpirit(int num)
{
    bool timer_status = timer->isActive();  // save status
    pause();    // pause the scene

    while (num > 0)
    {
        // find a random grid position inside SCENE
        int gx = qrand() % SCENE_WIDTH;
        int gy = qrand() % SCENE_HEIGHT;
        int type = qrand() % Spirit::SPIRIT_NUM;

        addSpiritAt((Spirit::SType) type, QPoint(gx, gy));
        num--;
    }

    if (timer_status == true)   // restore status
        resume();
}

QList<SpiritInfo> Scene::statistics()
{
    QList<SpiritInfo> infos;

    SpiritInfo blocks_info;
    SpiritInfo cheeses_info;
    SpiritInfo nails_info;
    SpiritInfo mice_info;
    SpiritInfo cats_info;
    SpiritInfo elephants_info;

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

    elephants_info.name = "Elephant";
    elephants_info.num = num_elephants;
    infos.append(elephants_info);

    return infos;
}

void Scene::useToolAt(const QPoint &grid_pos)
{
    if (cur_tool == T_ERASER)
    {
        removeSpiritAt(grid_pos);
    }
    else if (cur_tool == T_BLOCK)
    {
        addSpiritAt(Spirit::BLOCK, grid_pos);
    }
    else if (cur_tool == T_CHEESE)
    {
        addSpiritAt(Spirit::CHEESE, grid_pos);
    }
    else if (cur_tool == T_NAIL)
    {
        addSpiritAt(Spirit::NAIL, grid_pos);
    }
    else if (cur_tool == T_MOUSE)
    {
        addSpiritAt(Spirit::MOUSE, grid_pos);
    }
    else if (cur_tool == T_CAT)
    {
        addSpiritAt(Spirit::CAT, grid_pos);
    }
    else if (cur_tool == T_ELEPHANT)
    {
        addSpiritAt(Spirit::ELEPHANT, grid_pos);
    }
    else if (cur_tool == T_NONE)
    {
        //
    }
}

void Scene::removeSpiritAt(const QPoint &grid_pos)
{
    Spirit *spt = this->getSpiritAt(grid_pos.x(), grid_pos.y());

    if (spt == NULL) // not exist
    {
        return;
    }
    else    // found a tool
    {
        removeSpirit(spt);
    }
}

void Scene::addSpiritAt(Spirit::SType type, const QPoint &grid_pos)
{
    // check if there's some type already at pos
    Spirit *old_spirit = getSpiritAt(grid_pos.x(), grid_pos.y());
    if (old_spirit == NULL) // no spirit at pos, add directly
    {
        newSpiritAt(type, grid_pos);
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

                newSpiritAt(Spirit::BLOCK, grid_pos);
            }
            else if (type == Spirit::CHEESE)	// supply
            {
                Spirit *spt = newSpiritAt(Spirit::CHEESE, grid_pos);
                spt->healed(old_spirit->life());
                removeSpirit(old_spirit);
            }
            else if (type == Spirit::NAIL)	// erase cheese
            {
                removeSpirit(old_spirit);
                newSpiritAt(Spirit::NAIL, grid_pos);
            }
            else if (type == Spirit::MOUSE)	// add without erase
            {
                newSpiritAt(Spirit::MOUSE, grid_pos);
            }
            else if (type == Spirit::CAT)	// add direct
            {
                newSpiritAt(Spirit::CAT, grid_pos);
            }
            else if (type == Spirit::ELEPHANT)	// add directly
            {
                newSpiritAt(Spirit::ELEPHANT, grid_pos);
            }
            // other types come here
        }
        else if (old_spirit_type == Spirit::NAIL)
        {
            if (type == Spirit::BLOCK)	// erase nail
            {
                removeSpirit(old_spirit);
                newSpiritAt(Spirit::BLOCK, grid_pos);
            }
            else if (type == Spirit::CHEESE)	// erase nail
            {
                removeSpirit(old_spirit);
                newSpiritAt(Spirit::CHEESE, grid_pos);
            }
            else if (type == Spirit::NAIL)	// supply
            {
                Spirit *spt = newSpiritAt(Spirit::NAIL, grid_pos);
                spt->healed(old_spirit->life());
                removeSpirit(old_spirit);
            }
            else if (type == Spirit::MOUSE) // add directly
            {
                newSpiritAt(Spirit::MOUSE, grid_pos);
            }
            else if (type == Spirit::CAT)	// add directly
            {
                newSpiritAt(Spirit::CAT, grid_pos);
            }
            else if (type == Spirit::ELEPHANT)	// add directly
            {
                newSpiritAt(Spirit::ELEPHANT, grid_pos);
            }
            // other types come here
        }
        else if (old_spirit_type == Spirit::MOUSE || old_spirit_type == Spirit::CAT || old_spirit_type == Spirit::ELEPHANT)
        {
            if (type == Spirit::BLOCK)	// move avatar
            {
                bool found;
                QPoint npos = findSpiritNewPos(grid_pos, &found);
                if (found)
                {
                    old_spirit->setPos(npos * GRID_SIZE);   // move to the new pos
                }
                else
                {
                    removeSpirit(old_spirit);
                }

                newSpiritAt(Spirit::BLOCK, grid_pos);
            }
            else	// add directly
            {
                newSpiritAt(type, grid_pos);
            }
        }
        // other types come here
    }
}

QPoint Scene::findSpiritNewPos(const QPoint &grid_pos, bool *found)
{
    Spirit *spt = NULL;
    *found = false;
    // check if there's space above
    spt = this->getSpiritAt(grid_pos.x(), grid_pos.y() - 1);
    if (spt == NULL || spt->spiritType() != Spirit::BLOCK)	// not exit or not block
    {
        *found = true;
        return QPoint(grid_pos.x(), grid_pos.y() - 1);
    }

    // check right
    spt = this->getSpiritAt(grid_pos.x() + 1, grid_pos.y());
    if (spt == NULL || spt->spiritType() != Spirit::BLOCK)	// not exit or not block
    {
        *found = true;
        return QPoint(grid_pos.x() + 1, grid_pos.y());
    }

    // check below
    spt = this->getSpiritAt(grid_pos.x(), grid_pos.y() + 1);
    if (spt == NULL || spt->spiritType() != Spirit::BLOCK)	// not exit or not block
    {
        *found = true;
        return QPoint(grid_pos.x(), grid_pos.y() + 1);
    }

    // check left
    spt = this->getSpiritAt(grid_pos.x() - 1, grid_pos.y());
    if (spt == NULL || spt->spiritType() != Spirit::BLOCK)	// not exit or not block
    {
        *found = true;
        return QPoint(grid_pos.x() - 1, grid_pos.y());
    }

    // not found
    *found = false;
    return QPoint();
}

Spirit *Scene::newSpiritAt(Spirit::SType type, const QPoint &grid_pos)
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
    else if (type == Spirit::ELEPHANT)
    {
        spt = new Elephant(elephant_id);
        num = &num_elephants;
        spt->setZValue(Spirit::ELEPHANT_Z);
    }

    spt->setPos(grid_pos.x() * GRID_SIZE, grid_pos.y() * GRID_SIZE);
    this->addItem(spt);
    int i = spt->initialize();		// initialize spirit
    if (i != 0)	// init failed
    {
        qDebug() << "Scene: failed to new Spirit at" << grid_pos;
        this->removeItem(spt);
        return NULL;
    }

    // avatars use channel to communicate
    if (spt->spiritType() == Spirit::MOUSE || spt->spiritType() == Spirit::CAT || spt->spiritType() == Spirit::ELEPHANT)
    {
        AvatarSpirit *ava = dynamic_cast<AvatarSpirit *>(spt);
        assert(ava != NULL);
        ava->setChannel(channel);    // set channel
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
    else if (type == Spirit::ELEPHANT)
    {
        num = &num_elephants;
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
QPoint Scene::calGridPos(const QPointF &pos)
{
    int x = (int) pos.x();
    int y = (int) pos.y();

    x /= GRID_SIZE;
    y /= GRID_SIZE;

    return QPoint(x, y);
}

Spirit *Scene::getSpiritAt(int grid_x, int grid_y)
{
    qreal x = (grid_x + 0.5) * GRID_SIZE;
    qreal y = (grid_y + 0.5) * GRID_SIZE;

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
        this->useToolAt(calGridPos(event->scenePos()));
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() == Qt::CTRL)    // ctrl + move
    {
        this->useToolAt(calGridPos(event->scenePos()));
    }

    QGraphicsScene::mouseMoveEvent(event);
}
