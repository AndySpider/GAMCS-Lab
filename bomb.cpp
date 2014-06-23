#include "bomb.h"

Bomb::Bomb()
{
    _type = BOMB;
    _color = Qt::black;
    _life = 1;
}

Bomb::~Bomb()
{
}

void Bomb::act()
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
            if (type == MOUSE || type == CAT || type == ELEPHANT)
            {
                this->injured(1.0);    // blow up a bomb
            }
        }
    }
}
