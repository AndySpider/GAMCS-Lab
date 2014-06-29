#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QDebug>
#include <QList>
#include "scene.h"
#include "spirit.h"
#include "configure.h"

Spirit::Spirit() : _life(1), _type(BLOCK), _color(Qt::black), myscene(NULL), grid_x(-1), grid_y(-1),
    is_marked(false)
{
    setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges);
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

Spirit::SType Spirit::spiritType() const
{
    return _type;
}

Spirit::SCategory Spirit::spiritCategory() const
{
    return _category;
}

float Spirit::life() const
{
    return _life;
}

void Spirit::setLife(float l)
{
    _life = l;
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

QPoint Spirit::gridPos() const
{
    return QPoint(grid_x, grid_y);
}

void Spirit::setMarked(bool val)
{
    is_marked = val;
}

bool Spirit::isMarked() const
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

QString Spirit::stypeToString(SType stype)
{
    QString str;

    switch (stype)
    {
    case BLOCK:
        str = "Block";
        break;
    case CHEESE:
        str = "Cheese";
        break;
    case NAIL:
        str = "Nail";
        break;
    case BOMB:
        str = "Bomb";
        break;
    case MOUSE:
        str = "Mouse";
        break;
    case CAT:
        str = "Cat";
        break;
    case ELEPHANT:
        str = "Elephant";
        break;
    default:
        qDebug() << "unknown spirit type:" << stype;
        break;
    }

    return str;
}

Spirit::SType Spirit::stringToSType(const QString &str)
{
    Spirit::SType type;
    if (str == "Block")
        type = BLOCK;
    else if (str == "Cheese")
        type = CHEESE;
    else if (str == "Nail")
        type = NAIL;
    else if (str == "Bomb")
        type = BOMB;
    else if (str == "Mouse")
        type = MOUSE;
    else if (str == "Cat")
        type = CAT;
    else if (str == "Elephant")
        type = ELEPHANT;
    else
        qDebug() << "unknown Spirit string" << str;

    return type;
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
    if (isSelected())
        painter->setPen(Qt::red);

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

QVariant Spirit::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && myscene)
    {
        QPointF newPos = value.toPointF();	// in item coord
        newPos += QPointF(GRID_SIZE / 2, GRID_SIZE / 2);	// move pos to item center

        QPoint gridPos = myscene->calGridPos(newPos);
        if (myscene->outOfLimitLine(gridPos))	// out of limit line
        {
            // keep spirit inside the scene
            gridPos.setX(qMin(myscene->width() - 1, qMax(gridPos.x(), 0)));
            gridPos.setY(qMin(myscene->height() - 1, qMax(gridPos.y(), 0)));
        }

        return gridPos * GRID_SIZE;
    }

    return QGraphicsItem::itemChange(change, value);
}
