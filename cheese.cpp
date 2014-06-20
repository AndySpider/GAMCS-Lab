#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include "cheese.h"
#include "setdialog.h"

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

void Cheese::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   QMenu menu;

   // life
   QAction *setlife = menu.addAction("Set life");

   // show the menu
   QAction *selectedAction = menu.exec(event->screenPos());

   // judge the selected action
   if (selectedAction == setlife)
   {
        SetDialog dialog("Life:", QString::number(_life));
        if (dialog.exec())
        {
            bool ok;
            float new_life = dialog.getValue().toFloat(&ok);
            if (ok)
            {
                this->_life = new_life;
                qDebug() << "+++ life set to" << new_life;
            }
            else
            {
                qDebug() << "--- invalid life value, should be float!";
            }
        }
   }

    update();
}
