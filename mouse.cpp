#include <QDebug>
#include "mouse.h"

Mouse::Mouse(int id) : AvatarSpirit(id)
{
    _type = MOUSE;
    _color = QColor(89, 255, 89);
#ifdef SURVIVE_MODE
    _life = 0.5;	// instant death
#else
    _life = 30;
#endif
}

Mouse::~Mouse()
{
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
                pf += -9999999.0;
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

#ifdef SURVIVE_MODE
    return 1.0;
#else
    return pf;
#endif
}
