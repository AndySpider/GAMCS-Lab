#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QGraphicsView;
class QTimer;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

namespace gamcs {
class CSOSAgent;
}

class Mouse;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum Tool
    {
        CURSOR = 0,
        ERASER,
        NAIL,
        CHEESE,
        BLOCK
    };

private slots:
    void on_actionEraser_toggled(bool arg1);

    void on_actionNail_toggled(bool arg1);

    void on_actionCheese_toggled(bool arg1);

    void on_actionObstacle_toggled(bool arg1);

    void step();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *viewer;
    Tool tool;
    Mouse *mouse;
    gamcs::CSOSAgent *agent;
    QTimer *timer;

private:
    int initScene();
};

#endif // MAINWINDOW_H
