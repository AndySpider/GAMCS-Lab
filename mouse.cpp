#include <QDebug>
#include "mouse.h"
#include "configure.h"

Mouse::Mouse(int id) : AvatarSpirit(id)
{
    _type = MOUSE;
    _color = QColor(89, 255, 89);

    bool ok;
    float fval = g_config.getValue("AvatarSpirit/Life/Mouse").toFloat(&ok);
    if (ok)
        _life = fval;
    else
    {
        _life = 30;		// default
        g_config.setValue("AvatarSpirit/Life/Mouse", _life);
    }
}

Mouse::~Mouse()
{
}

float Mouse::originalPayoff(Agent::State st)
{
//    Q_UNUSED(st);

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
                pf += 0.5;
            }
            else if ((*it)->spiritType() == NAIL)
            {
                qDebug() << "Mouse" << id << ": Oops! nail!";
                this->injured(0.5);
                pf += -0.5;
            }
            else if ((*it)->spiritType() == BOMB)
            {
                qDebug() << "Mouse" << id << ": BOMB, dead!";
                this->injured(this->life());    // instant death
                this->setAwake(false);  // be blown up unconscious
                pf += -99.0;
            }
            else if ((*it)->spiritType() == CAT && dynamic_cast<AvatarSpirit *>(*it)->isAwake())    // a sleeping cat is harmless
            {
                qDebug() << "Mouse" << id << ": Cat! My GOD!";
                this->injured(1);
                pf += -1.0;
            }
            else if ((*it)->spiritType() == ELEPHANT && dynamic_cast<AvatarSpirit *>(*it)->isAwake())   // no attack to a sleeping poor animal
            {
                qDebug() << "Mouse" << id << ": Elephant! my delicious!";
                this->healed(1);
                pf += 1.0;
            }
            else
            {
                pf += 0.0;
            }
        }
    }

    if (pf == 0.0)	// curiosity counts only when no direct payoffs
    {
        bool experienced = myagent->hasState(st);
        if (experienced)
        {
            State_Info_Header *stif = myagent->getStateInfo(st);
            pf = -0.05 * stif->count;
            if (pf <= -1.0) pf = -1.0;	// minimum
            qDebug() << id << ":" << st << "is experienced" << stif->count << "times";
            free(stif);
        }
    }

    return pf;
}
