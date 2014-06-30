#ifndef RANDDIALOG_H
#define RANDDIALOG_H
#include <QDialog>
#include <QList>
#include <spirit.h>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QCheckBox;
class QPushButton;
QT_END_NAMESPACE

class RandDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RandDialog(QWidget *parent = 0);

    int getNum();
    QList<Spirit::SType> getTypes();

signals:

public slots:

private:
    QLabel *label;
    QLineEdit *valText;
    QCheckBox *checkBlock;
    QCheckBox *checkCheese;
    QCheckBox *checkNail;
    QCheckBox *checkBomb;
    QCheckBox *checkMouse;
    QCheckBox *checkCat;
    QCheckBox *checkElephant;
    QPushButton *cancelButton;
    QPushButton *okButton;
};

#endif // RANDDIALOG_H
