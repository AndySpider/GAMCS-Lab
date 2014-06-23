#include <QDebug>
#include "cheese.h"

Cheese::Cheese()
{
    _type = CHEESE;
    _color = QColor(255,204,0);
    _life = 15;
}

Cheese::~Cheese()
{
}

void Cheese::act()
{
    QList<Spirit *> colliding_spirits = collidingSpirits();
    if (colliding_spirits.empty())
    {
        return;
    }
    else
    {
        for (QList<Spirit *>::iterator it = colliding_spirits.begin(); it != colliding_spirits.end(); ++it)
        {
            Spirit::SType type = (*it)->spiritType();
            if (type == MOUSE)
            {
                this->injured(1);	// eaten by a mouse
            }
            else if (type == CAT)
            {
                this->injured(3);	// eaten by a cat
            }
            else if (type == ELEPHANT)
            {
                this->injured(5);   // eaten by an elephant
            }
        }
    }
}
