#include <QDebug>
#include "cheese.h"
#include "avatarspirit.h"
#include "configure.h"

Cheese::Cheese()
{
    _type = CHEESE;
    _color = QColor(255,204,0);

    bool ok;
    float fval = g_config.getValue("StaticSpirit/Life/Cheese").toFloat(&ok);
    if (ok)
        _life = fval;
    else
    {
        _life = 15;		// default
        g_config.setValue("StaticSpirit/Life/Cheese", _life);
    }
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
            if (type == MOUSE && dynamic_cast<AvatarSpirit *>(*it)->isAwake())
            {
                this->injured(1);	// eaten by a mouse
            }
            else if (type == CAT && dynamic_cast<AvatarSpirit *>(*it)->isAwake())
            {
                this->injured(3);	// eaten by a cat
            }
            else if (type == ELEPHANT && dynamic_cast<AvatarSpirit *>(*it)->isAwake())
            {
                this->injured(5);   // eaten by an elephant
            }
        }
    }
}
