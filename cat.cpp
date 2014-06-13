#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPointF>
#include <QTransform>
#include <QDebug>
#include <QList>
#include <QGraphicsSceneMouseEvent>
#include "mouse.h"
#include "scene.h"
#include "cheese.h"
#include "nail.h"
#include "cat.h"
#include "config.h"

Cat::Cat(int id) : Avatar(id)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setData(0, "Cat");

    myagent = new CSOSAgent(id, 0.8, 0.001);
    connectAgent(myagent);

    amount = new Amount(this, 50);
}

Cat::~Cat()
{
    delete myagent;
    delete amount;
}

QRectF Cat::boundingRect() const
{
    return QRectF(0, 0, GRID_SIZE, GRID_SIZE);    // the size of mouse
}

QPainterPath Cat::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, GRID_SIZE, GRID_SIZE);
    return path;
}

void Cat::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = QColor(250, 81, 143);
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.lighter();

    painter->setBrush(fillColor);
    painter->drawRect(0, 0, GRID_SIZE, GRID_SIZE);

    return;
}

void Cat::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Cat::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Scene *scene = dynamic_cast<Scene *>(this->scene());
    QPoint pos = scene->gridPoint(event->scenePos());
    this->setPos(pos);
    update();
}

void Cat::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void Cat::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    QString tips;
    QTextStream(&tips) << "id:" << id << "amount:" << amount->amount();
    setToolTip(tips);
}

Agent::State Cat::perceiveState()
{
    qreal st = this->x() / GRID_SIZE;
    st += (this->y() * SCENE_WIDTH) / GRID_SIZE;

    return (long) st;
}

void Cat::performAction(Agent::Action act)
{
    qreal x = this->x();
    qreal y = this->y();

    QGraphicsScene *scene = this->scene();
    QGraphicsItem *item = NULL;

    int half_grid = GRID_SIZE / 2;

    switch (act)
    {
    case 1: // move up
        // check if there're blocks
        item = scene->itemAt(x + half_grid, y - half_grid, QTransform());
        if (item == NULL || item->data(0).toString() != "Block")    // only Block can block
            y -= GRID_SIZE;
        break;
    case 2: // move down
        item = scene->itemAt(x + half_grid, y + GRID_SIZE + half_grid, QTransform());
        if (item == NULL || item->data(0).toString() != "Block")
            y += GRID_SIZE;
        break;
    case 3: // move left
        item = scene->itemAt(x - half_grid, y + half_grid,QTransform());
        if (item == NULL || item->data(0).toString() != "Block")
            x -= GRID_SIZE;
        break;
    case 4: // move right
        item = scene->itemAt(x + GRID_SIZE + half_grid, y + half_grid,QTransform());
        if (item == NULL || item->data(0).toString() != "Block")
            x += GRID_SIZE;
        break;
    case 5: // no move
        break;
    }

    this->setPos(x, y);
}

OSpace Cat::availableActions(Agent::State st)
{
    Q_UNUSED(st);

    OSpace acts;
    acts.add(1, 5, 1);
    return acts;
}

float Cat::originalPayoff(Agent::State st)
{
    Q_UNUSED(st);

    float pf = 0.0;
    QList<QGraphicsItem *> colliding_items = this->collidingItems(Qt::IntersectsItemShape);

    if (colliding_items.empty())
    {
        pf = 0.0;
    }
    else
    {
        // find the first valid tool
        QString item_name = "";
        QList<QGraphicsItem *>::iterator it;
        for (it = colliding_items.begin(); it != colliding_items.end(); ++it)
        {
            item_name = (*it)->data(0).toString();
            if (item_name == "")    // not a valid tool, continue
                continue;
            else
                break;      // break if found
        }

        if (it == colliding_items.end())    // no valid tool found
        {
            pf = 0.0;
        }
        else
        {
            if (item_name == "Cheese")
            {
                qDebug() << "Cat" << id << ": Wow! cheese!";
                Cheese *cheese = dynamic_cast<Cheese *>(*it);
                pf = 1;
                cheese->amount->decrease(1);
                this->amount->increase(0.3);
            }
            else if (item_name == "Nail")
            {
                qDebug() << "Cat" << id << ": Oops! nail!";
                Nail *nail = dynamic_cast<Nail *>(*it);
                pf = - (nail->amount->amount());
                this->amount->decrease(0.3);
            }
            else if (item_name == "Mouse")
            {
                qDebug() << "Cat" << id << ": Mouse! my favorite!";
                Mouse *mouse = dynamic_cast<Mouse *>(*it);
                pf = 5;	// mouse is very delicious
                mouse->amount->decrease(1);
                this->amount->increase(0.5);
            }
            else    // some tool get in the way
            {
                pf = 0.0;
            }
        }
    }

        return pf;
}
