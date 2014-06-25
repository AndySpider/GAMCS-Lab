#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE
class Configure;
class GeneralPage;
class AvatarSpiritPage;
class StaticSpiritPage;

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = 0);

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void apply();
    void cancel();
    void ok();

signals:

private:
    void createIcons();

    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;

    GeneralPage *generalPage;
    AvatarSpiritPage *avatarSpiritPage;
    StaticSpiritPage *staticSpiritPage;

};

#endif // CONFIGDIALOG_H
