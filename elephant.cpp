#include <QDebug>
#include "elephant.h"

Elephant::Elephant(int id) : AvatarSpirit(id)
{
    _type = ELEPHANT;
    _color = QColor(152, 163, 188);
    _life = 100;
}

Elephant::~Elephant()
{
}

float Elephant::originalPayoff(Agent::State st)
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
                qDebug() << "Elephant" << id << ": Wow! cheese!";
                this->healed(0.1);
                pf += 0.1;
            }
            else if ((*it)->spiritType() == NAIL)
            {
                qDebug() << "Elephant" << id << ": Oops! nail!";
                this->injured(0.1);
                pf += -0.1;
            }
            else if ((*it)->spiritType() == BOMB)
            {
                qDebug() << "Elephant" << id << ": BOMB, dead!";
                this->injured(this->life());    // instant death
                this->setAwake(false);  // be blown up unconscious
                pf += -9999999.0;
            }
            else if ((*it)->spiritType() == CAT && dynamic_cast<AvatarSpirit *>(*it)->isAwake())
            {
                qDebug() << "Elephant" << id << ": Cat~ I'll squeese you!";
                this->healed(1);
                pf += 1.0;
            }
            else if ((*it)->spiritType() == MOUSE && dynamic_cast<AvatarSpirit *>(*it)->isAwake())
            {
                qDebug() << "Elephant" << id << ": Mouse! OMG! stay away from my nose!";
                this->injured(1);
                pf += -1.0;
            }
            else
            {
                pf += 0.0;
            }
        }
    }

    return pf;
}
