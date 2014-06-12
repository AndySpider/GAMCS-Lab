#include <QDebug>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initUi()
{
    // titles
    this->setWindowTitle("Simulated Mice");
    this->setObjectName("MainWindow");

    // size
    this->resize(900, 700);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    this->setSizePolicy(sizePolicy);

    // actions
    toolGroup = new QActionGroup(this);

    actionCursor = new QAction(this);
    actionCursor->setText("cursor");
    actionCursor->setObjectName("actionCursor");
//    actionCursor->setShortcut("Ctrl+U");
    QIcon icon0;
    icon0.addFile(QStringLiteral(":/cursor.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionCursor->setIcon(icon0);
    actionCursor->setCheckable(true);
    actionCursor->setChecked(true);
    toolGroup->addAction(actionCursor);

    actionEraser = new QAction(this);
    actionEraser->setText("eraser");
    actionEraser->setObjectName("actionEraser");
//    actionCursor->setShortcut("Ctrl+E");
    QIcon icon1;
    icon1.addFile(QStringLiteral(":/eraser.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionEraser->setIcon(icon1);
    actionEraser->setCheckable(true);
    toolGroup->addAction(actionEraser);

    actionBlock = new QAction(this);
    actionBlock->setText("block");
    actionBlock->setObjectName("actionBlock");
//    actionCursor->setShortcut("Ctrl+B");
    QIcon icon2;
    icon2.addFile(QStringLiteral(":/block.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionBlock->setIcon(icon2);
    actionBlock->setCheckable(true);
    toolGroup->addAction(actionBlock);

    actionCheese = new QAction(this);
    actionCheese->setText("cheese");
    actionCheese->setObjectName("actionCheese");
//    actionCursor->setShortcut("Ctrl+C");
    QIcon icon3;
    icon3.addFile(QStringLiteral(":/cheese.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionCheese->setIcon(icon3);
    actionCheese->setCheckable(true);
    toolGroup->addAction(actionCheese);

    actionNail = new QAction(this);
    actionNail->setText("nail");
    actionNail->setObjectName("actionNail");
//    actionCursor->setShortcut("Ctrl+N");
    QIcon icon4;
    icon4.addFile(QStringLiteral(":/nail.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionNail->setIcon(icon4);
    actionNail->setCheckable(true);
    toolGroup->addAction(actionNail);

    actionMouse = new QAction(this);
    actionMouse->setText("Mouse");
    actionMouse->setObjectName("actionMouse");
//    actionCursor->setShortcut("Ctrl+M");
    QIcon icon5;
    icon5.addFile(QStringLiteral(":/mouse.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionMouse->setIcon(icon5);
    actionMouse->setCheckable(true);
    toolGroup->addAction(actionMouse);

    actionPause_Resume = new QAction(this);
    actionPause_Resume->setText("Pause/Resume");
    actionPause_Resume->setObjectName("actionPause_Resume");
//    actionCursor->setShortcut("Ctrl+P");
    QIcon icon6;
    icon6.addFile(QStringLiteral(":/pause_resume.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionPause_Resume->setCheckable(true);
    actionPause_Resume->setIcon(icon6);

    // viewer
    centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    centralWidget->resize(900, 700);
    this->setCentralWidget(centralWidget);

    viewer = new Viewer(centralWidget);
    viewer->setObjectName("viewer");
    viewer->setGeometry(QRect(0, 0, 800, 600));
    viewer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    viewer->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);

    // scene
    scene = new Scene(this);
    scene->setObjectName("scene");
    viewer->attachScene(scene);		// attach scene

    // menubar
    menuBar = new QMenuBar(this);
    menuBar->setObjectName("menuBar");
    menuBar->setGeometry(QRect(0, 0, 800, 25));

    menuNew = new QMenu(menuBar);
    menuNew->setObjectName("menuNew");
    menuNew->setTitle("New");
    menuQuit = new QMenu(menuBar);
    menuQuit->setObjectName("menuQuit");
    menuQuit->setTitle("Quit");

    menuBar->addAction(menuNew->menuAction());
    menuBar->addAction(menuQuit->menuAction());
    this->setMenuBar(menuBar);

    // toolbar
    toolBar = new QToolBar(this);
    toolBar->setObjectName("toolBar");
    toolBar->setMovable(true);
    toolBar->setAllowedAreas(Qt::LeftToolBarArea|Qt::RightToolBarArea|Qt::TopToolBarArea);
    toolBar->setFloatable(true);

    toolBar->addAction(actionCursor);
    toolBar->addAction(actionEraser);
    toolBar->addAction(actionBlock);
    toolBar->addAction(actionCheese);
    toolBar->addAction(actionNail);
    toolBar->addAction(actionMouse);
    toolBar->addSeparator();
    toolBar->addAction(actionPause_Resume);
    this->addToolBar(Qt::RightToolBarArea, toolBar);

    // statusBar
    statusBar = new QStatusBar(this);
    statusBar->setObjectName("statusBar");
    this->setStatusBar(statusBar);

    // slots
    QMetaObject::connectSlotsByName(this);
}

void MainWindow::on_actionCursor_triggered()
{
    scene->setCurTool(Scene::NONE);
}

void MainWindow::on_actionEraser_triggered()
{
    scene->setCurTool(Scene::ERASER);
}

void MainWindow::on_actionBlock_triggered()
{
    scene->setCurTool(Scene::BLOCK);
}

void MainWindow::on_actionCheese_triggered()
{
    scene->setCurTool(Scene::CHEESE);
}

void MainWindow::on_actionNail_triggered()
{
    scene->setCurTool(Scene::NAIL);
}

void MainWindow::on_actionMouse_triggered()
{
    scene->setCurTool(Scene::MOUSE);
}

void MainWindow::on_actionPause_Resume_toggled(bool arg1)
{
    if (arg1)
    {
        qDebug() << "+++ pause mice";
        scene->pauseMice();
    }
    else
    {
        qDebug() << "+++ resume mice";
        scene->resumeMice();
    }
}
