#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QTextStream>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QList>
#include "scene.h"
#include "spirit.h"
#include "config.h"
#include "setdialog.h"

Spirit::Spirit() : _life(1), _type(BLOCK), _color(Qt::black), myscene(NULL), grid_x(-1), grid_y(-1),
    is_marked(false)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

Spirit::~Spirit()
{
}

// this must be called after spirit be added to scene
int Spirit::initialize()
{
    myscene = dynamic_cast<Scene *>(this->scene());
    if (myscene == NULL)
    {
        qDebug() << "!!! Spirit: get scene failed, make sure it's add to a scene!";
        return -1;
    }
    else
    {
        QPoint pos = myscene->calGridPos(this->scenePos());	// set the initialized grid pos
        grid_x = pos.x();
        grid_y = pos.y();

        return 0;
    }
}

Spirit::SType Spirit::spiritType()
{
    return _type;
}

float Spirit::life()
{
    return _life;
}

void Spirit::injured(float l)
{
    _life_mutex.lock();
    _life -= l;
    _life_mutex.unlock();
}

void Spirit::healed(float l)
{
    _life_mutex.lock();
    _life += l;
    _life_mutex.unlock();
}

QPoint Spirit::gridPos()
{
    return QPoint(grid_x, grid_y);
}

void Spirit::setMarked(bool val)
{
    is_marked = val;
}

bool Spirit::isMarked()
{
    return is_marked;
}

QList<Spirit *> Spirit::collidingSpirits()
{
    QList<Spirit *> colliding_spirits;

    QList<QGraphicsItem *> colliding_items = this->collidingItems(Qt::IntersectsItemShape);

    if (!colliding_items.empty())
    {
        for (QList<QGraphicsItem *>::iterator itemit = colliding_items.begin(); itemit != colliding_items.end(); ++itemit)
        {
            Spirit *spirit = qgraphicsitem_cast<Spirit *>(*itemit);
            if (spirit != NULL)		// should be a spirit
                colliding_spirits.append(spirit);
        }
    }

    return colliding_spirits;
}

void Spirit::act()
{
    // do nothing by default
}

void Spirit::postAct()
{
    // do nothing by default
}

QRectF Spirit::boundingRect() const
{
    return QRectF(0, 0, GRID_SIZE, GRID_SIZE);    // the size of Spirit
}

QPainterPath Spirit::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, GRID_SIZE, GRID_SIZE);
    return path;
}

void Spirit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    // draw the spirit
    QColor fillColor;
    if (option->state & QStyle::State_MouseOver)
        fillColor = _color.lighter();
    else
        fillColor = _color;

    painter->setBrush(fillColor);
    painter->drawRect(0, 0, GRID_SIZE, GRID_SIZE);

    // draw decorations
    if (is_marked)
    {
        int len = GRID_SIZE / 2;
        painter->setBrush(Qt::yellow);
        static const QPointF points[3] = {  // triangle
            QPointF(0, -len),
            QPointF(0.87*len, 0.5*len),
            QPointF(-0.87*len, 0.5*len)
        };

        painter->drawConvexPolygon(points, 3);   // draw at origin point
    }

    return;
}

int Spirit::type() const
{
    // Enable the use of qgraphicsitem_cast with this item.
    return Type;
}

bool Spirit::collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const
{
//    qDebug() << "call collidesWithItem!!";
    Q_UNUSED(mode);

    const Spirit *spt = qgraphicsitem_cast<const Spirit *>(other);
    if (spt == NULL)
        return false;   // only collide with another spirit
    else
        return (spt->grid_x == this->grid_x && spt->grid_y == this->grid_y); // collide if at the same grid
}

void Spirit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Spirit::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint pos = myscene->calGridPos(event->scenePos());

    // update grid pos
    grid_x = pos.x();
    grid_y = pos.y();

    this->setPos(grid_x * GRID_SIZE, grid_y * GRID_SIZE);
    update();
}

void Spirit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void Spirit::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    QString tips;
    QTextStream(&tips) << "life:" << _life;
    setToolTip(tips);

    update();
}

void Spirit::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
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
