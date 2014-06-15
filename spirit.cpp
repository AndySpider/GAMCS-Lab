#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QTextStream>
#include <QDebug>
#include "scene.h"
#include "spirit.h"
#include "config.h"

Spirit::Spirit() : _life(1), _type(BLOCK), _color(Qt::black), myscene(NULL), grid_x(-1), grid_y(-1)
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
        QPoint pos = myscene->gridPoint(this->scenePos());	// set the initialized grid pos
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

QList<Spirit *> Spirit::collidingSpirits()
{
    QList<Spirit *> colliding_spirits;

    QList<QGraphicsItem *> colliding_items = this->collidingItems(Qt::IntersectsItemShape);

    if (!colliding_items.empty())
    {
        for (QList<QGraphicsItem *>::iterator itemit = colliding_items.begin(); itemit != colliding_items.end(); ++itemit)
        {
            Spirit *spirit = qgraphicsitem_cast<Spirit *>(*itemit);
            if (spirit != NULL)		// not a spirit if it's 0
                colliding_spirits.append(spirit);
        }
    }

    return colliding_spirits;
}

void Spirit::act()
{
    // do nothing by default
}

void Spirit::moveUp()
{
    // check if there are blocks
    Spirit *spt = myscene->getSpiritAt(grid_x, grid_y - GRID_SIZE);
    if (spt == NULL || spt->spiritType() != BLOCK)
    {
        grid_y -= GRID_SIZE;	// update grid pos
    }

    this->setPos(grid_x, grid_y);
}

void Spirit::moveDown()
{
    // check if there are blocks
    Spirit *spt = myscene->getSpiritAt(grid_x, grid_y + GRID_SIZE);
    if (spt == NULL || spt->spiritType() != BLOCK)
    {
        grid_y += GRID_SIZE;
    }

    this->setPos(grid_x, grid_y);
}

void Spirit::moveLeft()
{
    // check if there are blocks
    Spirit *spt = myscene->getSpiritAt(grid_x - GRID_SIZE, grid_y);
    if (spt == NULL || spt->spiritType() != BLOCK)
    {
        grid_x -= GRID_SIZE;
    }

    this->setPos(grid_x, grid_y);
}

void Spirit::moveRight()
{
    // check if there are blocks
    Spirit *spt = myscene->getSpiritAt(grid_x + GRID_SIZE, grid_y);
    if (spt == NULL || spt->spiritType() != BLOCK)
    {
        grid_x += GRID_SIZE;
    }

    this->setPos(grid_x, grid_y);
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

    QColor fillColor;
    if (option->state & QStyle::State_MouseOver)
        fillColor = _color.lighter();
    else
        fillColor = _color;

    painter->setBrush(fillColor);
    painter->drawRect(0, 0, GRID_SIZE, GRID_SIZE);

    return;
}

int Spirit::type() const
{
    // Enable the use of qgraphicsitem_cast with this item.
    return Type;
}

void Spirit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Spirit::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint pos = myscene->gridPoint(event->scenePos());

    // update grid pos
    grid_x = pos.x();
    grid_y = pos.y();
    this->setPos(grid_x, grid_y);

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
}

