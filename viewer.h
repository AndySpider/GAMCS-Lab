#ifndef VIEWER_H
#define VIEWER_H
#include <QGraphicsView>

class Scene;

class Viewer : public QGraphicsView
{
    Q_OBJECT

public:
    Viewer(QWidget *parent);

private:
};

#endif // VIEWER_H
