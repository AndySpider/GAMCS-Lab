#include <QTextStream>
#include <QList>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "scene.h"
#include "config.h"
#include "mouse.h"

Mouse::Mouse(int id) : Avatar(id)
{
    _type = MOUSE;
    _color = QColor(89, 255, 89);
    _life = 30;

    myagent = new CSOSAgent(id, 0.9, 0.01);
    connectAgent(myagent);
}

Mouse::~Mouse()
{
    delete myagent;
}

void Mouse::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    QString tips;
    QTextStream(&tips) << "id:" << id << ", life:" << _life;
    setToolTip(tips);
}

void Mouse::act()
{
    Avatar::step();
}

Agent::State Mouse::perceiveState()
{
    int st = grid_x;
    st += grid_y * SCENE_WIDTH;

    return st;
}

void Mouse::performAction(Agent::Action act)
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

OSpace Mouse::availableActions(Agent::State st)
{
    Q_UNUSED(st);

    OSpace acts;
    acts.add(1, 5, 1);
    return acts;
}

float Mouse::originalPayoff(Agent::State st)
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
                qDebug() << "Mouse" << id << ": Wow! cheese!";
                this->healed(0.5);
                pf += 1.0;
            }
            else if ((*it)->spiritType() == NAIL)
            {
                qDebug() << "Mouse" << id << ": Oops! nail!";
                this->injured(0.5);
                pf += -1.0;
            }
            else if ((*it)->spiritType() == CAT)
            {
                qDebug() << "Mouse" << id << ": Cat! My GOD!";
                this->injured(1);
                pf += -2.0;
            }
            else
            {
                qDebug() << "Mouse" << id << ": What's this, get out of my way!";
                pf += 0.0;
            }
        }
    }

    return pf;
}
