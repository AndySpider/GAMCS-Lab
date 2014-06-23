#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
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

    actionBomb = new QAction(this);
    actionBomb->setText("Bomb");
    actionBomb->setObjectName("actionBomb");
    actionBomb->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_6));
    QIcon iconbomb;
    iconbomb.addFile(QStringLiteral(":/images/bomb.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionBomb->setIcon(iconbomb);
    actionBomb->setCheckable(true);
    toolGroup->addAction(actionBomb);

    actionMouse = new QAction(this);
    actionMouse->setText("Mouse");
    actionMouse->setObjectName("actionMouse");
    actionMouse->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_7));
    QIcon icon5;
    icon5.addFile(QStringLiteral(":/images/mouse.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionMouse->setIcon(icon5);
    actionMouse->setCheckable(true);
    toolGroup->addAction(actionMouse);

    actionCat = new QAction(this);
    actionCat->setText("Cat");
    actionCat->setObjectName("actionCat");
    actionCat->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_8));
    QIcon iconCat;
    iconCat.addFile(QStringLiteral(":/images/cat.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionCat->setIcon(iconCat);
    actionCat->setCheckable(true);
    toolGroup->addAction(actionCat);

    actionElephant = new QAction(this);
    actionElephant->setText("Elephant");
    actionElephant->setObjectName("actionElephant");
    actionElephant->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_9));
    QIcon iconEle;
    iconEle.addFile(QStringLiteral(":/images/elephant.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionElephant->setIcon(iconEle);
    actionElephant->setCheckable(true);
    toolGroup->addAction(actionElephant);

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
    connect(scene, SIGNAL(spiritsNumChanged(int)), this, SLOT(spiritsNumChanged(int)));

    viewer->setScene(scene);		// attach scene

    // menubar
    // File Menu
    menuBar = new QMenuBar(this);
    menuBar->setObjectName("menuBar");
    menuBar->setGeometry(QRect(0, 0, 800, 25));

    menuScene = new QMenu(menuBar);
    menuScene->setObjectName("menuScene");
    menuScene->setTitle("Scene");

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
    actionRecent_Scenes = new QAction(this);
    actionRecent_Scenes->setObjectName(QStringLiteral("actionRecent_Files"));
    actionNew->setText("New");
    actionOpen->setText("Open");
    actionSave->setText("Save");
    actionSave_as->setText("Save as");
    actionQuit->setText("Quit");
    actionRecent_Scenes->setText("Recent Scenes");

    menuScene->addAction(actionNew);
    menuScene->addAction(actionOpen);
    menuScene->addAction(actionRecent_Scenes);
    menuScene->addSeparator();
    menuScene->addAction(actionSave);
    menuScene->addAction(actionSave_as);
    menuScene->addSeparator();
    menuScene->addAction(actionQuit);

    // Options Menu
    menuOptions = new QMenu(menuBar);
    menuOptions->setObjectName("menuOptions");
    menuOptions->setTitle("Options");

    menuDeadMode = menuOptions->addMenu("Dead Mode");
    // Options actions
    actionDeadMode_Dead = menuDeadMode->addAction("Dead");
    actionDeadMode_Dead->setObjectName("actionDeadMode_Dead");
    actionDeadMode_Dead->setCheckable(true);
    actionDeadMode_Dead->setChecked(true);
    actionDeadMode_Undead = menuDeadMode->addAction("Undead");
    actionDeadMode_Undead->setObjectName("actionDeadMode_Undead");
    actionDeadMode_Undead->setCheckable(true);

    actionGenRand = menuOptions->addAction("Generate 30 Spirits Randomly");
    actionGenRand->setObjectName("actionGenRand");

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
    menuBar->addAction(menuScene->menuAction());
    menuBar->addAction(menuOptions->menuAction());
    menuBar->addAction(menuAbout->menuAction());
    this->setMenuBar(menuBar);

    // toolbar
    toolBar = new QToolBar(this);
    toolBar->setObjectName("toolBar");
    toolBar->setMovable(true);
    toolBar->setFloatable(true);

    toolBar->addAction(actionCursor);
    toolBar->addAction(actionEraser);
    toolBar->addSeparator();
    toolBar->addAction(actionBlock);
    toolBar->addAction(actionCheese);
    toolBar->addAction(actionNail);
    toolBar->addAction(actionBomb);
    toolBar->addAction(actionMouse);
    toolBar->addAction(actionCat);
    toolBar->addAction(actionElephant);
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

void MainWindow::spiritsNumChanged(int num)
{
    QList<SpiritInfo> spirits_info = scene->statistics();

    QString msg;
    QTextStream(&msg) << "Spirits number: " << num;
    for (QList<SpiritInfo>::iterator it = spirits_info.begin(); it != spirits_info.end(); ++it)
    {
        QTextStream(&msg) << ", " << it->name << ":" << it->num;
    }

    statusBar->showMessage(msg);
}

void MainWindow::on_actionCursor_triggered()
{
    scene->setCurTool(Scene::T_NONE);
}

void MainWindow::on_actionEraser_triggered()
{
    scene->setCurTool(Scene::T_ERASER);
}

void MainWindow::on_actionBlock_triggered()
{
    scene->setCurTool(Scene::T_BLOCK);
}

void MainWindow::on_actionCheese_triggered()
{
    scene->setCurTool(Scene::T_CHEESE);
}

void MainWindow::on_actionNail_triggered()
{
    scene->setCurTool(Scene::T_NAIL);
}

void MainWindow::on_actionBomb_triggered()
{
    scene->setCurTool(Scene::T_BOMB);
}

void MainWindow::on_actionMouse_triggered()
{
    scene->setCurTool(Scene::T_MOUSE);
}

void MainWindow::on_actionCat_triggered()
{
    scene->setCurTool(Scene::T_CAT);
}

void MainWindow::on_actionElephant_triggered()
{
    scene->setCurTool(Scene::T_ELEPHANT);
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

void MainWindow::on_actionNew_triggered()
{
    if (confirmClose())
    {
        scene->init();
        setCurrentFileName(QString());
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open Scene..."), QString(),
                                              tr("Scene Files (*.scene);;All Files(*)"));
    if (!fn.isEmpty())
    {
        int re = scene->load(fn);
        if (re == 0)    // load successfully
            setCurrentFileName(fn);
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (filename.isEmpty())
        on_actionSave_as_triggered();
    else
        scene->save(filename);
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save Scene..."), QString(),
                                              tr("Scene Files (*.scene);;All Files(*)"));
    if (!fn.isEmpty())
    {
       if (!fn.endsWith(".scene", Qt::CaseInsensitive))
           fn += ".scene";  // default
    }

    scene->save(fn);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionRecent_Files_triggered()
{

}

void MainWindow::on_actionAbout_Simulated_Mice_triggered()
{
    QMessageBox::about(this, tr("About"), tr("This application demonstrates the use of GAMCS to create multipul autonomaous avatars."));
}

void MainWindow::on_actionAbout_GAMCS_triggered()
{
    QString translatedTextAboutGAMCSCaption;
    translatedTextAboutGAMCSCaption = QMessageBox::tr(
                "<h3> About GAMCS</h3>"
                "<p> This program uses GAMCS version 1.3.4");

    QString translatedTextAboutGAMCSText;
    translatedTextAboutGAMCSText = QMessageBox::tr(
                "<p> GAMCS is a blabla.</p>"
                "<p> GAMCS provides blabla. </p>"
                "<p> gamcs licensed under blabla </p>"
                "<p> here is the link: <a href=\"http://gamcs.andy87.com\">gamcs</a> for an overview of gamcs </p>"
                "<p> copyright </p>"
                );
    QMessageBox *aboutBox = new QMessageBox(this);
    aboutBox->setAttribute(Qt::WA_DeleteOnClose);
    aboutBox->setWindowTitle(tr("About GAMCS"));
    aboutBox->setText(translatedTextAboutGAMCSCaption);
    aboutBox->setInformativeText(translatedTextAboutGAMCSText);

    QPixmap logo(QStringLiteral(":/images/gamcs-logo-small.png"));
    aboutBox->setIconPixmap(logo);
    aboutBox->setDefaultButton(aboutBox->addButton(QMessageBox::Ok));

    aboutBox->show();
    aboutBox->exec();
}

bool MainWindow::confirmClose()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Confirm Close?"),
                                        tr("The Scene and all the Avatar Memories will be lost!"),
                                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
    {
        on_actionSave_triggered();
    }
    else if (ret == QMessageBox::Cancel)
    {
        return false;
    }

    return true;    // saved
}

void MainWindow::setCurrentFileName(const QString &file)
{
    this->filename = file;

    QString showName;
    if (filename.isEmpty())
        showName = "untitled.scene";
    else
        showName = QFileInfo(filename).fileName();

    setWindowTitle(tr("%1[*] [%2]").arg(showName).arg(tr("Simulated Mice --- powered by GAMCS")));
    setWindowModified(false);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (confirmClose())
        e->accept();
    else
        e->ignore();
}

void MainWindow::on_actionDeadMode_Dead_triggered()
{
    scene->setGameMode(Scene::DEAD);
    actionDeadMode_Dead->setChecked(true);
    actionDeadMode_Undead->setChecked(false);
}

void MainWindow::on_actionDeadMode_Undead_triggered()
{
    scene->setGameMode(Scene::UNDEAD);
    actionDeadMode_Undead->setChecked(true);
    actionDeadMode_Dead->setChecked(false);
}

void MainWindow::on_actionGenRand_triggered()
{
    scene->genRandSpirit(30);
}
