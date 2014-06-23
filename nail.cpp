#include <QDebug>
#include "nail.h"

Nail::Nail()
{
   _type = NAIL;
   _color = Qt::black;
   _life = 1.5;
}

Nail::~Nail()
{
}

void Nail::act()
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
                this->injured(0.1);	// blunt by a mouse
            }
            else if (type == CAT)
            {
                this->injured(0.3);	// blunt by a cat
            }
            else if (type == ELEPHANT)
            {
                this->injured(0.5); // blunt by an elephant
            }
        }
    }
}
