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
#include "config.h"

// FIXME
const int SCENE_WIDTH = 800;
const int SCENE_HEIGHT = 600;
const int GRID_SIZE = 10;

Mouse::Mouse()
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
    setData(0, "Mouse");

    myagent = new CSOSAgent(id, 0.9, 0.01);
    connectAgent(myagent);
}

Mouse::~Mouse()
{
    delete myagent;
}

QRectF Mouse::boundingRect() const
{
    return QRectF(0, 0, 10, 10);    // the size of mouse
}

QPainterPath Mouse::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 10, 10);
    return path;
}

void Mouse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = QColor(89, 255, 89);
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.lighter();

    painter->setBrush(fillColor);
    painter->drawRect(0, 0, 10, 10);

    return;
}

void Mouse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void Mouse::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Scene *scene = dynamic_cast<Scene *>(this->scene());
    QPoint pos = scene->gridPoint(event->scenePos());
    this->setPos(pos);
    update();
}

void Mouse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

void Mouse::hoverEnterEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "mouse hover!!";
    QString tips;
    QTextStream(&tips) << "Id: " << id;
    setToolTip(tips);
}

Agent::State Mouse::perceiveState()
{
    // x : [0, SCENE_WIDTH / GRID_SIZE], y: [0, SCENE_HEIGHT / GRID_SIZE]
    qreal st = this->x() / GRID_SIZE;
    st += (this->y() * SCENE_WIDTH) / (GRID_SIZE * GRID_SIZE);

    return (long) st;
}

void Mouse::performAction(Agent::Action act)
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
        if (item == NULL || item->data(0).toString() != "Block")
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
    }

    this->setPos(x, y);
}

OSpace Mouse::availableActions(Agent::State st)
{
    Q_UNUSED(st);

    OSpace acts;
    acts.add(1, 4, 1);
    return acts;
}

float Mouse::originalPayoff(Agent::State st)
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
    // only one can collide, check the first
        QString item_name = colliding_items[0]->data(0).toString();
        if (item_name == "Cheese")
        {
            qDebug() << id << ": Wow! cheese!";
            pf = 1.0;
        }
        else if (item_name == "Nail")
        {
            qDebug() << id << ": Oops! nail!";
            pf = -1.0;
        }
        else
        {
            qDebug() << "+++ what's this, get out of my way!";
            pf = 0.0;
        }
    }

    return pf;
}
