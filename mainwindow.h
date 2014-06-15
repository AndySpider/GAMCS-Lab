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
#include <QVBoxLayout>
#include "viewer.h"
#include "scene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void spiritsNumChanged(int);

private:
    // tool actions
    QAction *actionCursor;
    QAction *actionEraser;
    QAction *actionBlock;
    QAction *actionCheese;
    QAction *actionNail;
    QAction *actionMouse;
    QAction *actionCat;

    QActionGroup *toolGroup;

    // control actions
    QAction *actionPause_Resume;
    QAction *actionSpeedUp;
    QAction *actionSpeedDown;

    // menubar, toolbar, statusbar
    QMenuBar *menuBar;

    QMenu *menuFile;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionQuit;
    QAction *actionRecent_Files;

    QMenu *menuEdit;

    QMenu *menuAbout;
    QAction *actionAbout_Simulated_Mice;
    QAction *actionAbout_GAMCS;

    QToolBar *toolBar;
    QStatusBar *statusBar;

    // viewer and scene
    QVBoxLayout *verticalLayout;
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
    void on_actionCat_triggered();

    void on_actionPause_Resume_toggled(bool arg1);
    void on_actionSpeedUp_triggered();
    void on_actionSpeedDown_triggered();

};

#endif // MAINWINDOW_H
