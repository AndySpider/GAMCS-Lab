#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "scene.h"
#include "block.h"
#include "config.h"

Block::Block()
{
    _type = BLOCK;
    _color = QColor(186, 95, 38);
    _life = 1;
}
