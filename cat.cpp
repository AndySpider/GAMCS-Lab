#include <QDebug>
#include <QList>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include "scene.h"
#include "config.h"
#include "cat.h"

Cat::Cat(int id) : Avatar(id), learning_mode(Agent::ONLINE)
{
    _type = CAT;
    _color = QColor(250, 81, 143);
    _life = 50;

    myagent = new CSOSAgent(id, 0.9, 0.01);
    myagent->setMode(learning_mode);
    connectAgent(myagent);
}

Cat::~Cat()
{
    delete myagent;
}

void Cat::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    QString tips;
    QTextStream(&tips) << "id:" << id << ", life:" << _life;
    setToolTip(tips);
}

// popup menu
void Cat::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;

    QMenu *lmode = menu.addMenu("Learning Mode");
    QAction *online = lmode->addAction("Online mode");
    online->setCheckable(true);
    QAction *explore = lmode->addAction("Explore mode");
    explore->setCheckable(true);
    if (learning_mode == Agent::ONLINE)
        online->setChecked(true);
    else
        explore->setChecked(true);

    QAction *setting = menu.addAction("Setting");

    QAction *selectedAction = menu.exec(event->screenPos());

    // judge selected action
    if (selectedAction == online)
    {
        qDebug() << "mouse change mode to ONLINE";
        myagent->setMode(Agent::ONLINE);
        learning_mode = Agent::ONLINE;
    }
    else if (selectedAction == explore)
    {
        qDebug() << "mouse change mode to EXPLORE";
        myagent->setMode(Agent::EXPLORE);
        learning_mode = Agent::EXPLORE;
    }
    else if (selectedAction == setting)
    {
        qDebug() << "Setting dialog...";
    }
}

void Cat::act()
{
    Avatar::step();
}

Agent::State Cat::perceiveState()
{
//    int st = grid_x;
//    st += grid_y * SCENE_WIDTH;

//    // perceive the spirits at four directions
    int stype0 = 0, stype1 = 0, stype2 = 0, stype3 = 0, stype4 = 0;

    // current pos
    QList<Spirit *> colliding_spirits = collidingSpirits();
    if (colliding_spirits.empty())
        stype0 = 0;
    else if (colliding_spirits[0]->spiritType() == BLOCK)
        stype0 = 1;
    else if (colliding_spirits[0]->spiritType() == CHEESE)
        stype0 = 2;
    else if (colliding_spirits[0]->spiritType() == NAIL)
        stype0 = 3;
    else if (colliding_spirits[0]->spiritType() == MOUSE)
        stype0 = 4;
    else if (colliding_spirits[0]->spiritType() == CAT)
        stype0 = 5;

    // above
    Spirit *spt = myscene->getSpiritAt(grid_x, grid_y - GRID_SIZE);
    if (spt == NULL)
        stype1 = 0;
    else if (spt->spiritType() == BLOCK)
        stype1 = 1;
    else if (spt->spiritType() == CHEESE)
        stype1 = 2;
    else if (spt->spiritType() == NAIL)
        stype1 = 3;
    else if (spt->spiritType() == MOUSE)
        stype1 = 4;
    else if (spt->spiritType() == CAT)
        stype1 = 5;

    // down
    spt = myscene->getSpiritAt(grid_x, grid_y + GRID_SIZE);
    if (spt == NULL)
        stype2 = 0;
    else if (spt->spiritType() == BLOCK)
        stype2 = 1;
    else if (spt->spiritType() == CHEESE)
        stype2 = 2;
    else if (spt->spiritType() == NAIL)
        stype2 = 3;
    else if (spt->spiritType() == MOUSE)
        stype2 = 4;
    else if (spt->spiritType() == CAT)
        stype2 = 5;

    // left
    spt = myscene->getSpiritAt(grid_x - GRID_SIZE, grid_y);
    if (spt == NULL)
        stype3 = 0;
    else if (spt->spiritType() == BLOCK)
        stype3 = 1;
    else if (spt->spiritType() == CHEESE)
        stype3 = 2;
    else if (spt->spiritType() == NAIL)
        stype3 = 3;
    else if (spt->spiritType() == MOUSE)
        stype3 = 4;
    else if (spt->spiritType() == CAT)
        stype3 = 5;

    // right
    spt = myscene->getSpiritAt(grid_x + GRID_SIZE, grid_y);
    if (spt == NULL)
        stype4 = 0;
    else if (spt->spiritType() == BLOCK)
        stype4 = 1;
    else if (spt->spiritType() == CHEESE)
        stype4 = 2;
    else if (spt->spiritType() == NAIL)
        stype4 = 3;
    else if (spt->spiritType() == MOUSE)
        stype4 = 4;
    else if (spt->spiritType() == CAT)
        stype4 = 5;

    int st = stype0 + stype1 * 6 + stype2 * 36 + stype3 * 216 + stype4 * 1296;

    qDebug() << "State:" << st;
    return st;
}

void Cat::performAction(Agent::Action act)
{
    switch (act)
    {
    case 1: // move up
        moveUp();
        break;
    case 2: // move down
        moveDown();
        break;
    case 3: // move left
        moveLeft();
        break;
    case 4: // move right
        moveRight();
        break;
    case 5: // no move
        break;
    }
}

OSpace Cat::availableActions(Agent::State st)
{
    Q_UNUSED(st);

    OSpace acts;
    acts.add(1, 5, 1);
    return acts;
}

float Cat::originalPayoff(Agent::State st)
{
    Q_UNUSED(st);

    float pf = 0.0;
    QList<Spirit *> colliding_spirits = collidingSpirits();
    if (colliding_spirits.empty())
    {
        pf = 0.0;
    }
    else	// integrated all the colliding spirits
    {
        for (QList<Spirit *>::iterator it = colliding_spirits.begin(); it != colliding_spirits.end(); ++it)
        {
            if ((*it)->spiritType() == CHEESE)
            {
                qDebug() << "Cat" << id << ": Wow! cheese!";
                this->healed(0.3);
                pf += 1;
            }
            else if ((*it)->spiritType() == NAIL)
            {
                qDebug() << "Cat" << id << ": Oops! nail!";
                this->injured(0.3);
                pf += -1;
            }
            else if ((*it)->spiritType() == MOUSE)
            {
                qDebug() << "Cat" << id << ": Mouse! My favorite!";
                this->healed(0.5);
                pf += 2.0;
            }
            else
            {
                qDebug() << "Cat" << id << ": What's this, get out of my way!";
                pf += 0.0;
            }
        }
    }

    return pf;
}
