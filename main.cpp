#include <QApplication>
#include "mainwindow.h"
#include "configure.h"

Configure g_config;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // load configure;
    g_config.setConfigFile("./GAMCS-Lab.conf");
    g_config.load();

    MainWindow w;
    w.show();

    return a.exec();
}
