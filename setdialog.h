#ifndef SETDIALOG_H
#define SETDIALOG_H
#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

class SetDialog : public QDialog
{
    Q_OBJECT

public:
    SetDialog(QString lab, QString oldVal);

    QString getValue();

private:
    QLabel *label;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLineEdit *valText;
};

#endif // SETDIALOG_H
