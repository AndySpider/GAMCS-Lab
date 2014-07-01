#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QTextStream>
#include <QGraphicsSceneContextMenuEvent>
#include "staticspirit.h"
#include "setdialog.h"

StaticSpirit::StaticSpirit()
{
    _category = STATICSPIRIT;
}

StaticSpirit::~StaticSpirit()
{
}

void StaticSpirit::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    QString tips;
    QTextStream(&tips) << "life:" << _life;
    setToolTip(tips);

    update();
}

void StaticSpirit::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
   QMenu menu;

   // mark
   QAction *toggle_mark;
   if (isMarked())
       toggle_mark = menu.addAction("Unmark");
   else
       toggle_mark = menu.addAction("Mark");

   // life
   QAction *setlife = menu.addAction("Set life...");

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
   else if (selectedAction == toggle_mark)
   {
       this->setMarked(!this->isMarked());
   }

    update();
}
