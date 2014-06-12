#include <QMouseEvent>
#include <QDebug>
#include "viewer.h"
#include "scene.h"
#include "config.h"

Viewer::Viewer(QWidget *parent) : QGraphicsView(parent), scene(NULL)
{
    setMouseTracking(true);	// mouse track on
}

void Viewer::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void Viewer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        scene->useTool(gridPoint(event->pos()));
    }

    QGraphicsView::mousePressEvent(event);
}

void Viewer::attachScene(Scene *s)
{
    setScene(s);
    scene = s;	// save the scene
}

/**
 * @brief Snap to grid.
 * @param pos
 * @return
 */
QPoint Viewer::gridPoint(const QPoint &pos)
{
    int x = pos.x();
    int y = pos.y();

    int half_grid = GRID_SIZE / 2;

    int dx = x % GRID_SIZE;
    if (dx > half_grid)
        x = x - dx + GRID_SIZE;
    else
        x -= dx;

    int dy = y % GRID_SIZE;
    if (dy > half_grid)
        y = y - dy + GRID_SIZE;
    else
        y -= dy;

    qDebug() << "Grid from " << pos << "to " << x << y;

    return QPoint(x, y);
}
