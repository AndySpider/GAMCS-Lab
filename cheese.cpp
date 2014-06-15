#include "cheese.h"

Cheese::Cheese()
{
    _type = CHEESE;
    _color = QColor(255,204,0);
    _life = 10;
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
            if ((*it)->spiritType() == MOUSE)
            {
                this->injured(1);	// eaten by a mouse
            }
            else if ((*it)->spiritType() == CAT)
            {
                this->injured(2);	// eaten by a cat
            }
        }
    }

}
