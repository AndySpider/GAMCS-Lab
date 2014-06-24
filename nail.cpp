#include <QDebug>
#include "nail.h"
#include "avatarspirit.h"

Nail::Nail()
{
   _type = NAIL;
   _color = QColor(89, 89, 100);
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
            if (type == MOUSE && dynamic_cast<AvatarSpirit *>(*it)->isAwake())
            {
                this->injured(0.1);	// blunt by a mouse
            }
            else if (type == CAT && dynamic_cast<AvatarSpirit *>(*it)->isAwake())
            {
                this->injured(0.3);	// blunt by a cat
            }
            else if (type == ELEPHANT && dynamic_cast<AvatarSpirit *>(*it)->isAwake())
            {
                this->injured(0.5); // blunt by an elephant
            }
        }
    }
}
