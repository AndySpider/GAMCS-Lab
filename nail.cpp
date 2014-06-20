#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include "nail.h"
#include "setdialog.h"

Nail::Nail()
{
   _type = NAIL;
   _color = Qt::black;
   _life = 1;
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
            if ((*it)->spiritType() == MOUSE)
            {
                this->injured(0.1);	// blunt by a mouse
            }
            else if ((*it)->spiritType() == CAT)
            {
                this->injured(0.3);	// blunt by a cat
            }
        }
    }
}

void Nail::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
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
