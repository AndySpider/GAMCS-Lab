#ifndef VIEWER_H
#define VIEWER_H
#include <QGraphicsView>

class Scene;

class Viewer : public QGraphicsView
{
public:
    Viewer(QWidget *parent);
    void attachScene(Scene *);

private:
    Scene *scene;

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);

    QPoint gridPoint(const QPoint &);
};

#endif // VIEWER_H
