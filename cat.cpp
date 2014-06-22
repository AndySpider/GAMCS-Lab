#include <QDebug>
#include "cat.h"

Cat::Cat(int id) : AvatarSpirit(id)
{
    _type = CAT;
    _color = QColor(250, 81, 143);
    _life = 50;
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
