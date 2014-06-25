#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include "configdialog.h"
#include "configpages.h"
#include "configure.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent)
{
    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::ListMode);
    contentsWidget->setMaximumWidth(128);
    contentsWidget->setSpacing(10);

    pagesWidget = new QStackedWidget;
    generalPage = new GeneralPage();
    pagesWidget->addWidget(generalPage);
    avatarSpiritPage = new AvatarSpiritPage();
    pagesWidget->addWidget(avatarSpiritPage);
    staticSpiritPage = new StaticSpiritPage();
    pagesWidget->addWidget(staticSpiritPage);

    QPushButton *applyeButton = new QPushButton(tr("Apply"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    QPushButton *okButton = new QPushButton(tr("Ok"));
    connect(applyeButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(ok()));

    createIcons();
    contentsWidget->setCurrentRow(0);

    QHBoxLayout *horizonalLayout = new QHBoxLayout;
    horizonalLayout->addWidget(contentsWidget);
    horizonalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(applyeButton);
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(okButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizonalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Configure Dialog"));
}

void ConfigDialog::createIcons()
{
    QListWidgetItem *generalButton = new QListWidgetItem(contentsWidget);
    generalButton->setIcon(QIcon(":/images/config_general.png"));
    generalButton->setText(tr("General"));
    generalButton->setTextAlignment(Qt::AlignHCenter);
    generalButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *avatarspiritButton = new QListWidgetItem(contentsWidget);
    avatarspiritButton->setIcon(QIcon(":/images/config_avatarspirit.png"));
    avatarspiritButton->setText(tr("AvatarSpirit"));
    avatarspiritButton->setTextAlignment(Qt::AlignHCenter);
    avatarspiritButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *staticspiritButton = new QListWidgetItem(contentsWidget);
    staticspiritButton->setIcon(QIcon(":/images/config_staticspirit.png"));
    staticspiritButton->setText(tr("StaticSpirit"));
    staticspiritButton->setTextAlignment(Qt::AlignHCenter);
    staticspiritButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void ConfigDialog::apply()
{
    generalPage->applyConfig();
    avatarSpiritPage->applyConfig();
    staticSpiritPage->applyConfig();

    g_config.save();
}

void ConfigDialog::cancel()
{
    close();
}

void ConfigDialog::ok()
{
    apply();
    close();
}
