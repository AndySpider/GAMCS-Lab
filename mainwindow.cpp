#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "spirit.h"
#include "block.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), scene(NULL), viewer(NULL), tool(CURSOR)
{
    ui->setupUi(this);

    initScene();

    viewer = ui->graphicsView;
    viewer->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete viewer;
}

int MainWindow::initScene()
{
    scene = new QGraphicsScene(this);

    // add spirit
    Spirit *spirit = new Spirit();
    scene->addItem(spirit);

    // add the 4 walls
    int num;
    // the left wall, [5, 5] ~ [5, h-5]
    num = (this->height() -5 - 5) / 10;
    for (int i =0; i< num; i++)
    {
        Block *block = new Block();
        block->setPos(5 , i*10 + 5);
        scene->addItem(block);
    }

    // the right wall, [w-5, 5] ~ [w-5, h-5]
    for (int i=0; i< num; i++)
    {
        Block *block = new Block();
        block->setPos(this->width()-5, i*10+5);
        scene->addItem(block);
    }

    // the up wall, [15, 5] ~ [w-15, 5]
    num = (this->width() - 5 - 5) / 10;
    for (int i=0; i < num; i++)
    {
        Block *block = new Block();
        block->setPos(i * 10 + 15, 5);
        scene->addItem(block);
    }

    // the bottom wall, [15, h-5] ~ [w-15, h-5]
    for (int i = 0; i < num; i++)
    {
        Block *block = new Block();
        block->setPos(i * 10 + 15, this->height() - 5);
        scene->addItem(block);
    }

    return 0;
}

void MainWindow::on_actionEraser_toggled(bool arg1)
{
    if (arg1 == true)
        tool = ERASER;
}

void MainWindow::on_actionNail_toggled(bool arg1)
{
    if (arg1 == true)
        tool = NAIL;
}

void MainWindow::on_actionCheese_toggled(bool arg1)
{
    if (arg1 == true)
        tool = CHEESE;
}

void MainWindow::on_actionObstacle_toggled(bool arg1)
{
    if (arg1 == true)
        tool = BLOCK;
}
