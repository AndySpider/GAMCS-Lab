#include <QDebug>
#include "mainwindow.h"
#include "config.h"

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
    // mainwindow
    this->setWindowTitle("Simulated Mice --- powered by GAMCS");
    this->setObjectName("MainWindow");
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/mouse.png"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);

    this->resize(800, 600);
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    this->setSizePolicy(sizePolicy);

    // actions
    toolGroup = new QActionGroup(this);

    actionCursor = new QAction(this);
    actionCursor->setText("Cursor");
    actionCursor->setObjectName("actionCursor");
    actionCursor->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
    QIcon icon0;
    icon0.addFile(QStringLiteral(":/images/cursor.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionCursor->setIcon(icon0);
    actionCursor->setCheckable(true);
    actionCursor->setChecked(true);
    toolGroup->addAction(actionCursor);

    actionEraser = new QAction(this);
    actionEraser->setText("Eraser");
    actionEraser->setObjectName("actionEraser");
    actionEraser->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
    QIcon icon1;
    icon1.addFile(QStringLiteral(":/images/eraser.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionEraser->setIcon(icon1);
    actionEraser->setCheckable(true);
    toolGroup->addAction(actionEraser);

    actionBlock = new QAction(this);
    actionBlock->setText("Block");
    actionBlock->setObjectName("actionBlock");
    actionBlock->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_3));
    QIcon icon2;
    icon2.addFile(QStringLiteral(":/images/block.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionBlock->setIcon(icon2);
    actionBlock->setCheckable(true);
    toolGroup->addAction(actionBlock);

    actionCheese = new QAction(this);
    actionCheese->setText("Cheese");
    actionCheese->setObjectName("actionCheese");
    actionCheese->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_4));
    QIcon icon3;
    icon3.addFile(QStringLiteral(":/images/cheese.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionCheese->setIcon(icon3);
    actionCheese->setCheckable(true);
    toolGroup->addAction(actionCheese);

    actionNail = new QAction(this);
    actionNail->setText("Nail");
    actionNail->setObjectName("actionNail");
    actionNail->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_5));
    QIcon icon4;
    icon4.addFile(QStringLiteral(":/images/nail.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionNail->setIcon(icon4);
    actionNail->setCheckable(true);
    toolGroup->addAction(actionNail);

    actionMouse = new QAction(this);
    actionMouse->setText("Mouse");
    actionMouse->setObjectName("actionMouse");
    actionMouse->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_6));
    QIcon icon5;
    icon5.addFile(QStringLiteral(":/images/mouse.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionMouse->setIcon(icon5);
    actionMouse->setCheckable(true);
    toolGroup->addAction(actionMouse);

    actionPause_Resume = new QAction(this);
    actionPause_Resume->setText("Pause/Resume");
    actionPause_Resume->setObjectName("actionPause_Resume");
    actionPause_Resume->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    QIcon icon6;
    icon6.addFile(QStringLiteral(":/images/resume.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionPause_Resume->setIcon(icon6);
    actionPause_Resume->setCheckable(true);

    actionSpeedUp = new QAction(this);
    actionSpeedUp->setText("Speed Up");
    actionSpeedUp->setObjectName("actionSpeedUp");
    actionSpeedUp->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
    QIcon icon7;
    icon7.addFile(QStringLiteral(":/images/speedup.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSpeedUp->setIcon(icon7);

    actionSpeedDown = new QAction(this);
    actionSpeedDown->setText("Speed Down");
    actionSpeedDown->setObjectName("actionSpeedDown");
    actionSpeedDown->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
    QIcon icon8;
    icon8.addFile(QStringLiteral(":/images/speeddown.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSpeedDown->setIcon(icon8);

    // viewer
    centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    this->setCentralWidget(centralWidget);

    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

    viewer = new Viewer(centralWidget);
    viewer->setObjectName(QStringLiteral("viewer"));

    verticalLayout->addWidget(viewer);

    // scene
    scene = new Scene(this);
    scene->setObjectName("scene");
    connect(scene, SIGNAL(miceNumChanged(int)), this, SLOT(miceNumChanged(int)));

    viewer->setScene(scene);		// attach scene

    // menubar
    // File Menu
    menuBar = new QMenuBar(this);
    menuBar->setObjectName("menuBar");
    menuBar->setGeometry(QRect(0, 0, 800, 25));

    menuFile = new QMenu(menuBar);
    menuFile->setObjectName("menuFile");
    menuFile->setTitle("File");

    // File actions
    actionNew = new QAction(this);
    actionNew->setObjectName(QStringLiteral("actionNew"));
    actionOpen = new QAction(this);
    actionOpen->setObjectName(QStringLiteral("actionOpen"));
    actionSave = new QAction(this);
    actionSave->setObjectName(QStringLiteral("actionSave"));
    actionSave_as = new QAction(this);
    actionSave_as->setObjectName(QStringLiteral("actionSave_as"));
    actionQuit = new QAction(this);
    actionQuit->setObjectName(QStringLiteral("actionQuit"));
    actionRecent_Files = new QAction(this);
    actionRecent_Files->setObjectName(QStringLiteral("actionRecent_Files"));
    actionNew->setText("New");
    actionOpen->setText("Open");
    actionSave->setText("Save");
    actionSave_as->setText("Save as");
    actionQuit->setText("Quit");
    actionRecent_Files->setText("Recent Files");

    menuFile->addAction(actionNew);
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionRecent_Files);
    menuFile->addSeparator();
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSave_as);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);

    // Edit Menu
    menuEdit = new QMenu(menuBar);
    menuEdit->setObjectName("menuEdit");
    menuEdit->setTitle("Edit");
    // Edit actions

    // About Menu
    menuAbout = new QMenu(menuBar);
    menuAbout->setObjectName("menuAbout");
    menuAbout->setTitle("About");
    // About actions
    actionAbout_Simulated_Mice = new QAction(this);
    actionAbout_Simulated_Mice->setObjectName(QStringLiteral("actionAbout_Simulated_Mice"));
    actionAbout_GAMCS = new QAction(this);
    actionAbout_GAMCS->setObjectName(QStringLiteral("actionAbout_GAMCS"));
    actionAbout_Simulated_Mice->setText("About Simulated Mice");
    actionAbout_GAMCS->setText("About GAMCS");

    menuAbout->addAction(actionAbout_Simulated_Mice);
    menuAbout->addAction(actionAbout_GAMCS);

    // add to menuBar
    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuEdit->menuAction());
    menuBar->addAction(menuAbout->menuAction());
    this->setMenuBar(menuBar);

    // toolbar
    toolBar = new QToolBar(this);
    toolBar->setObjectName("toolBar");
    toolBar->setMovable(true);
    toolBar->setFloatable(true);

    toolBar->addAction(actionCursor);
    toolBar->addAction(actionEraser);
    toolBar->addAction(actionBlock);
    toolBar->addAction(actionCheese);
    toolBar->addAction(actionNail);
    toolBar->addAction(actionMouse);
    toolBar->addSeparator();
    toolBar->addAction(actionPause_Resume);
    toolBar->addAction(actionSpeedUp);
    toolBar->addAction(actionSpeedDown);
    this->addToolBar(Qt::RightToolBarArea, toolBar);

    // statusBar
    statusBar = new QStatusBar(this);
    statusBar->setObjectName("statusBar");
    this->setStatusBar(statusBar);

    // slots
    QMetaObject::connectSlotsByName(this);
}

void MainWindow::miceNumChanged(int num)
{
    QString msg;
    QTextStream(&msg) << "Mice number: " << num;
    statusBar->showMessage(msg);
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
    QIcon icon;

    if (arg1)
    {
        qDebug() << "+++ pause mice";
        icon.addFile(QStringLiteral(":/images/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause_Resume->setIcon(icon);
        scene->pause();
    }
    else
    {
        qDebug() << "+++ resume mice";
        icon.addFile(QStringLiteral(":/images/resume.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPause_Resume->setIcon(icon);
        scene->resume();
    }
}

void MainWindow::on_actionSpeedUp_triggered()
{
    scene->speedUp();
}

void MainWindow::on_actionSpeedDown_triggered()
{
    scene->speedDown();
}
