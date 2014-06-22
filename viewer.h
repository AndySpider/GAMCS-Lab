#ifndef VIEWER_H
#define VIEWER_H
#include <QGraphicsView>

class Scene;

class Viewer : public QGraphicsView
{
    Q_OBJECT

public:
    Viewer(QWidget *parent);

protected:
    virtual void wheelEvent(QWheelEvent *event);

private:
};

#endif // VIEWER_H
