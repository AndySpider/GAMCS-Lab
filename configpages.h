#ifndef CONFIGPAGES_H
#define CONFIGPAGES_H
#include <QWidget>
#include <QTableView>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QComboBox;
QT_END_NAMESPACE

class Configure;

class ConfigPage : public QWidget
{
    Q_OBJECT

public:
    ConfigPage(QWidget *parent = 0);

    virtual void applyConfig() = 0;

public slots:
    virtual void resetDefault() = 0;
};

class GeneralPage : public ConfigPage
{
    Q_OBJECT

public:
    GeneralPage(QWidget *parent = 0);

    void applyConfig();

public slots:
    void resetDefault();

private:
    QLineEdit *widthEdit;
    QLineEdit *heightEdit;
};

class AvatarSpiritPage : public ConfigPage
{
    Q_OBJECT

public:
    AvatarSpiritPage(QWidget *parent = 0);

    void applyConfig();

public slots:
    void resetDefault();

private:
    QLineEdit *mouseLifeEdit;
    QLineEdit *catLifeEdit;
    QLineEdit *elephantLifeEdit;
    QLineEdit *rangeEdit;
    QLineEdit *freqEdit;
    QLineEdit *drEdit;
    QLineEdit *acuEdit;
    QComboBox *lmCombo;
};

class StaticSpiritPage : public ConfigPage
{
    Q_OBJECT

public:
    StaticSpiritPage(QWidget *parent = 0);

    void applyConfig();

public slots:
    void resetDefault();

private:
    QLineEdit *cheeseLifeEdit;
    QLineEdit *nailLifeEdit;
};

class HuntDia : public QTableView
{
public:
    HuntDia(QWidget *parent = 0);
};

#endif // CONFIGPAGES_H
