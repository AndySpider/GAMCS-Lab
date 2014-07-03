#include <QDebug>
#include "cat.h"
#include "configure.h"

Cat::Cat(int id) : AvatarSpirit(id)
{
    _type = CAT;
    _color = QColor(250, 81, 143);

    bool ok;
    float fval = g_config.getValue("AvatarSpirit/Life/Cat").toFloat(&ok);
    if (ok)
        _life = fval;
    else
    {
        _life = 50;		// default
        g_config.setValue("AvatarSpirit/Life/Cat", _life);
    }
}

Cat::~Cat()
{
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
                pf += 0.3;
            }
            else if ((*it)->spiritType() == NAIL)
            {
                qDebug() << "Cat" << id << ": Oops! nail!";
                this->injured(0.3);
                pf += -0.3;
            }
            else if ((*it)->spiritType() == BOMB)
            {
                qDebug() << "Cat" << id << ": BOMB, dead!";
                this->injured(this->life());    // instant death
                this->setAwake(false);  // be blown up unconscious
                pf += -99.0;
            }
            else if ((*it)->spiritType() == MOUSE && dynamic_cast<AvatarSpirit *>(*it)->isAwake())
            {
                qDebug() << "Cat" << id << ": Mouse! My favorite!";
                this->healed(1);
                pf += 1.0;
            }
            else if ((*it)->spiritType() == ELEPHANT && dynamic_cast<AvatarSpirit *>(*it)->isAwake())
            {
                qDebug() << "Cat" << id << ": Elephant! OMG, run away!";
                this->injured(1);
                pf += -1.0;
            }
            else
            {
                pf += 0.0;
            }
        }
    }

    if (pf == 0.0)	// curiosity counts only when no direct payoffs
    {
        pf = payoffByCuriosity(st);
    }

    return pf;
}
