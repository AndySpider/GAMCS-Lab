#include <QMouseEvent>
#include <QDebug>
#include "viewer.h"
#include "scene.h"
#include "config.h"

Viewer::Viewer(QWidget *parent) : QGraphicsView(parent)
{
    setMouseTracking(true);	// mouse track on
}
