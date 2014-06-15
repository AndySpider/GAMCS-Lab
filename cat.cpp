#include <QDebug>
#include <QList>
#include <QGraphicsSceneMouseEvent>
#include "scene.h"
#include "config.h"
#include "cat.h"

Cat::Cat(int id) : Avatar(id)
{
    _type = CAT;
    _color = QColor(250, 81, 143);
    _life = 50;

    myagent = new CSOSAgent(id, 0.9, 0.01);
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

void Cat::act()
{
    Avatar::step();
}

Agent::State Cat::perceiveState()
{
    int st = grid_x;
    st += grid_y * SCENE_WIDTH;

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
