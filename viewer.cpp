#include <QMouseEvent>
#include <QDebug>
#include <QPixmap>
#include "viewer.h"
#include "scene.h"

Viewer::Viewer(QWidget *parent) : QGraphicsView(parent)
{
    setBackgroundBrush(QPixmap(":/images/grid.png"));
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setMouseTracking(true);	// mouse track on
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(FullViewportUpdate);
}

void Viewer::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // scale the view / do the zoom
    double scaleFactor = 1.15;
    if (event->delta() > 0)
    {
        // zoom in
        scale(scaleFactor, scaleFactor);
    }
    else
    {
        // zoom out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}
