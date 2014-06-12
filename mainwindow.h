#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include "viewer.h"
#include "scene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // tool actions
    QAction *actionCursor;
    QAction *actionEraser;
    QAction *actionBlock;
    QAction *actionCheese;
    QAction *actionNail;
    QAction *actionMouse;

    QActionGroup *toolGroup;

    // control actions
    QAction *actionPause_Resume;

    // menubar, toolbar, statusbar
    QMenuBar *menuBar;
    QMenu *menuNew;
    QMenu *menuQuit;

    QToolBar *toolBar;
    QStatusBar *statusBar;

    // viewer and scene
    QWidget *centralWidget;
    Viewer *viewer;
    Scene *scene;

    void initUi();

private slots:
    void on_actionCursor_triggered();
    void on_actionEraser_triggered();
    void on_actionBlock_triggered();
    void on_actionCheese_triggered();
    void on_actionNail_triggered();
    void on_actionMouse_triggered();

    void on_actionPause_Resume_toggled(bool arg1);

};

#endif // MAINWINDOW_H
