#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "mainwindow.h"
#include "configdialog.h"

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
    this->setWindowTitle(QString("%1 v%2").arg(APP).arg(VER));
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
    connect(scene, SIGNAL(paused()), this, SLOT(scenePaused()));
    connect(scene, SIGNAL(resumed()), this, SLOT(sceneResumed()));
    connect(scene, SIGNAL(currentSpeedLevel(int)), this, SLOT(sceneSpeedLevel(int)));

    viewer->setScene(scene);		// attach scene

    // menubar
    // File Menu
    menuBar = new QMenuBar(this);
    menuBar->setObjectName("menuBar");
    menuBar->setGeometry(QRect(0, 0, 800, 25));

    menuScene = new QMenu(menuBar);
    menuScene->setObjectName("menuScene");
    menuScene->setTitle(tr("&Scene"));

    // Scene actions
    actionNew = new QAction(this);
    actionNew->setShortcut(tr("CTRL+N"));
    actionNew->setObjectName(QStringLiteral("actionNew"));
    actionOpen = new QAction(this);
    actionOpen->setShortcut(tr("CTRL+O"));
    actionOpen->setObjectName(QStringLiteral("actionOpen"));
    actionSave = new QAction(this);
    actionSave->setShortcut(tr("CTRL+S"));
    actionSave->setObjectName(QStringLiteral("actionSave"));
    actionSave_as = new QAction(this);
    actionSave_as->setObjectName(QStringLiteral("actionSave_as"));
    actionQuit = new QAction(this);
    actionQuit->setShortcut(tr("CTRL+Q"));
    actionQuit->setObjectName(QStringLiteral("actionQuit"));
    actionNew->setText("&New");
    actionOpen->setText(tr("&Open"));
    actionSave->setText("&Save");
    actionSave_as->setText("Save as");
    actionQuit->setText("&Quit");

    menuDemo = new QMenu(this);
    menuDemo->setTitle(tr("&Demo Scenes"));
    for (int i = 0; i < MaxDemoScenes; ++i)
    {
        demoSceneActs[i] = new QAction(this);
        demoSceneActs[i]->setVisible(false);
        connect(demoSceneActs[i], SIGNAL(triggered()), this, SLOT(openDemoScene()));

        menuDemo->addAction(demoSceneActs[i]);
    }

    updateDemoSceneActions();

    menuScene->addAction(actionNew);
    menuScene->addAction(actionOpen);
    menuScene->addMenu(menuDemo);
    menuScene->addSeparator();
    menuScene->addAction(actionSave);
    menuScene->addAction(actionSave_as);
    menuScene->addSeparator();
    menuScene->addAction(actionQuit);

    // Options Menu
    menuOptions = new QMenu(menuBar);
    menuOptions->setObjectName("menuOptions");
    menuOptions->setTitle(tr("&Options"));

    // Options actions
    actionGenRand = menuOptions->addAction("&Generate 30 Spirits Randomly");
    actionGenRand->setShortcut(tr("CTRL+G"));
    actionGenRand->setObjectName("actionGenRand");

    actionConfigure = menuOptions->addAction(tr("&Configure"));
    actionConfigure->setShortcut(tr("CTRL+C"));
    actionConfigure->setObjectName("actionConfigure");

    // About Menu
    menuAbout = new QMenu(menuBar);
    menuAbout->setObjectName("menuAbout");
    menuAbout->setTitle(tr("&About"));
    // About actions
    actionAbout_App = new QAction(this);
    actionAbout_App->setObjectName(QStringLiteral("actionAbout_App"));
    actionAbout_GAMCS = new QAction(this);
    actionAbout_GAMCS->setObjectName(QStringLiteral("actionAbout_GAMCS"));
    actionAbout_App->setText(QString("About %1").arg(APP));
    actionAbout_GAMCS->setText("About GAMCS");

    menuAbout->addAction(actionAbout_App);
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

void MainWindow::scenePaused()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionPause_Resume->setIcon(icon);
    actionPause_Resume->setChecked(true);
}

void MainWindow::sceneResumed()
{
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/resume.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionPause_Resume->setIcon(icon);
}

void MainWindow::sceneSpeedLevel(int level)
{
    if (level == -1)  // reach minima
    {
        actionSpeedDown->setEnabled(false);
    }
    else if (level == 1)  // reach maxima
    {
        actionSpeedUp->setEnabled(false);
    }
    else
    {
        actionSpeedUp->setEnabled(true);
        actionSpeedDown->setEnabled(true);
    }
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
    if (arg1)
    {
        scene->pause();
    }
    else
    {
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
        scene->build();
        setCurrentFileName(QString());
    }
}

void MainWindow::on_actionOpen_triggered()
{
    on_actionNew_triggered();	// clear the scene first

    QString fn = QFileDialog::getOpenFileName(this, tr("Open Scene..."), QString(),
                                              tr("Scene Files (*.scene);;All Files(*)"));
    if (!fn.isEmpty())
    {
        openScene(fn);
    }
}

void MainWindow::openScene(const QString &file)
{
    int ret = scene->load(file);
    if (ret == 0)   // load successfully
        setCurrentFileName(file);
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

       saveScene(fn);
    }
}

void MainWindow::saveScene(const QString &file)
{
    scene->save(file);
    filename = file;
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionAbout_App_triggered()
{
    QMessageBox::about(this, tr("About"), tr("<p> This application demonstrates the using of GAMCS to create autonomous avatars, "
                                             "providing a yard and several basic spirits for you to experiment with. </p>"
                                             "<p> Just play around and watch the magic things that can happen. </p>"));
}

void MainWindow::on_actionAbout_GAMCS_triggered()
{
    QString translatedTextAboutGAMCSCaption;
    translatedTextAboutGAMCSCaption = QMessageBox::tr(
                "<h3> About GAMCS</h3>"
                "<p> The avatars in this program are powered by GAMCS.");

    QString translatedTextAboutGAMCSText;
    translatedTextAboutGAMCSText = QMessageBox::tr(
                "<p> GAMCS is a computer implementation of <b>GAM</b>, which is a new, generalized (machine) learning algorithm..</p>"
                "<p> GAMCS provides a very easy to use as well as complete interface. It is very fast and stable. </p>"
                "<p> GAMCS is licensed under Mozilla Public License (MPL). </p>"
                "<p> Please see <a href=\"http://gamcs.andy87.com\">gamcs.andy87.com</a> for more details about GAMCS. </p>"
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
    if (scene->empty())
        return true;

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

    setWindowTitle(tr("%1[*] [%2]").arg(showName).arg(tr("%1 v%2").arg(APP).arg(VER)));
    setWindowModified(false);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (confirmClose())
        e->accept();
    else
        e->ignore();
}

void MainWindow::on_actionGenRand_triggered()
{
    scene->genRandSpirit(30);
}

void MainWindow::on_actionConfigure_triggered()
{
    ConfigDialog dialog;
    dialog.exec();
}

void MainWindow::updateDemoSceneActions()
{
    // get file list in the demo directory
    QDir demo_dir;
    demo_dir.setPath("./demos");
    if (!demo_dir.exists())
    {
        qWarning() << "Demo path" << demo_dir.path() << "not exists!";
        return;
    }

    QStringList filter;
    filter << "*.scene" << "*.sce";
    QStringList scene_files;
    scene_files = demo_dir.entryList(filter, QDir::Files);

    int num_demo_scenes = scene_files.size() > MaxDemoScenes ? MaxDemoScenes : scene_files.size();
    for (int i = 0; i < num_demo_scenes; ++i)
    {
        QString text = tr("%1").arg(QFileInfo(scene_files.at(i)).baseName());
        demoSceneActs[i]->setText(text);
        demoSceneActs[i]->setData(demo_dir.absoluteFilePath(scene_files.at(i)));
        demoSceneActs[i]->setVisible(true);
    }
}

void MainWindow::openDemoScene()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        openScene(action->data().toString());
}
