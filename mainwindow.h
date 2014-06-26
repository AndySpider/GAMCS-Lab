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
    void scenePaused();
    void sceneResumed();
    void sceneSpeedLevel(int level);

private:
    // tool actions
    QAction *actionCursor;
    QAction *actionEraser;
    QAction *actionBlock;
    QAction *actionCheese;
    QAction *actionNail;
    QAction *actionBomb;
    QAction *actionMouse;
    QAction *actionCat;
    QAction *actionElephant;

    QActionGroup *toolGroup;

    // control actions
    QAction *actionPause_Resume;
    QAction *actionSpeedUp;
    QAction *actionSpeedDown;

    // menubar, toolbar, statusbar
    QMenuBar *menuBar;

    QMenu *menuScene;
    QAction *actionNew;
    QAction *actionOpen;
    QMenu *menuDemo;
    enum { MaxDemoScenes = 10};
    QAction *demoSceneActs[MaxDemoScenes];
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionQuit;

    QMenu *menuOptions;
    QAction *actionGenRand;
    QAction *actionConfigure;

    QMenu *menuAbout;
    QAction *actionAbout_App;
    QAction *actionAbout_GAMCS;

    QToolBar *toolBar;
    QStatusBar *statusBar;

    // viewer and scene
    QVBoxLayout *verticalLayout;
    QWidget *centralWidget;
    Viewer *viewer;
    Scene *scene;

    void initUi();

protected:
    virtual void closeEvent(QCloseEvent *e);

private slots:
    void on_actionCursor_triggered();
    void on_actionEraser_triggered();
    void on_actionBlock_triggered();
    void on_actionCheese_triggered();
    void on_actionNail_triggered();
    void on_actionBomb_triggered();
    void on_actionMouse_triggered();
    void on_actionCat_triggered();
    void on_actionElephant_triggered();

    void on_actionPause_Resume_toggled(bool arg1);
    void on_actionSpeedUp_triggered();
    void on_actionSpeedDown_triggered();

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionQuit_triggered();

    void on_actionGenRand_triggered();
    void on_actionConfigure_triggered();

    void on_actionAbout_App_triggered();
    void on_actionAbout_GAMCS_triggered();

    void openDemoScene();

private:
    QString filename;

    bool confirmClose();
    void openScene(const QString &file);
    void saveScene(const QString &file);
    void setCurrentFileName(const QString &file);
    void updateDemoSceneActions();

};

#endif // MAINWINDOW_H
