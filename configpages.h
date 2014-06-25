#ifndef CONFIGPAGES_H
#define CONFIGPAGES_H
#include <QWidget>
#include <QTableView>

class GeneralPage : public QWidget
{
public:
    GeneralPage(QWidget *parent = 0);
};

class HuntDia : public QTableView
{
public:
    HuntDia(QWidget *parent = 0);
};

class AvatarSpiritPage : public QWidget
{
public:
    AvatarSpiritPage(QWidget *parent = 0);
};

class StaticSpiritPage : public QWidget
{
public:
    StaticSpiritPage(QWidget *parent = 0);
};

#endif // CONFIGPAGES_H
