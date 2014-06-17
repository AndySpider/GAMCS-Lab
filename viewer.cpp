#include <QMouseEvent>
#include <QDebug>
#include "viewer.h"
#include "scene.h"
#include "config.h"

Viewer::Viewer(QWidget *parent) : QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setMouseTracking(true);	// mouse track on
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(FullViewportUpdate);
}
