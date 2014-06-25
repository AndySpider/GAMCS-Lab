#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include "configpages.h"


GeneralPage::GeneralPage(QWidget *parent) :
    QWidget(parent)
{
    // scene size
    QGroupBox *sizeGroup = new QGroupBox(tr("Scene Size"));
    QLabel *widthLabel = new QLabel(tr("width"));
    QLineEdit *widthEdit = new QLineEdit(); // TODO
    QLabel *heightLabel = new QLabel(tr("height"));
    QLineEdit *heightEdit = new QLineEdit();

    QGridLayout *sizeLayout = new QGridLayout;
    sizeLayout->addWidget(widthLabel, 0, 0);
    sizeLayout->addWidget(widthEdit, 0, 1);
    sizeLayout->addWidget(heightLabel, 1, 0);
    sizeLayout->addWidget(heightEdit, 1, 1);
    sizeGroup->setLayout(sizeLayout);

    // hunt diagram
    QGroupBox *huntGroup = new QGroupBox(tr("Hunting Diagram"));
    HuntDia *huntDia = new HuntDia(this);

    QVBoxLayout *huntLayout = new QVBoxLayout;
    huntLayout->addWidget(huntDia);
    huntGroup->setLayout(huntLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(sizeGroup);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(huntGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

HuntDia::HuntDia(QWidget *parent) :
    QTableView(parent)
{

}

AvatarSpiritPage::AvatarSpiritPage(QWidget *parent) :
    QWidget(parent)
{
    // life
    QGroupBox *lifeGroup = new QGroupBox(tr("Default Life Value"));
    QLabel *mouseLifeLabel = new QLabel(tr("Mouse Life"));
    QLineEdit *mouseLifeEdit = new QLineEdit();
    QLabel *catLifeLabel = new QLabel(tr("Cat Life"));
    QLineEdit *catLifeEdit = new QLineEdit();
    QLabel *elephantLifeLabel = new QLabel(tr("Elephant Life"));
    QLineEdit *elephantLifeEdit = new QLineEdit();

    QGridLayout *lifeLayout = new QGridLayout;
    lifeLayout->addWidget(mouseLifeLabel, 0, 0);
    lifeLayout->addWidget(mouseLifeEdit, 0, 1);
    lifeLayout->addWidget(catLifeLabel, 1, 0);
    lifeLayout->addWidget(catLifeEdit, 1, 1);
    lifeLayout->addWidget(elephantLifeLabel, 2, 0);
    lifeLayout->addWidget(elephantLifeEdit, 2, 1);
    lifeGroup->setLayout(lifeLayout);

    // communication
    QGroupBox *comGroup = new QGroupBox(tr("Default Communication Params"));
    QLabel *rangeLabel = new QLabel(tr("Radar Range"));
    QLineEdit *rangeEdit = new QLineEdit();
    QLabel *freqLabel = new QLabel(tr("Com Freq"));
    QLineEdit *freqEdit = new QLineEdit();

    QGridLayout *comLayout = new QGridLayout;
    comLayout->addWidget(rangeLabel, 0, 0);
    comLayout->addWidget(rangeEdit, 0, 1);
    comLayout->addWidget(freqLabel, 1, 0);
    comLayout->addWidget(freqEdit, 1, 1);
    comGroup->setLayout(comLayout);

    // gamcs params
    QGroupBox *paramsGroup = new QGroupBox(tr("Default GAMCS Parameters"));
    QLabel *drLabel = new QLabel(tr("Discount Rate"));
    QLineEdit *drEdit = new QLineEdit();
    QLabel *acuLabel = new QLabel(tr("Accuracy"));
    QLineEdit *acuEdit = new QLineEdit();
    QLabel *lmLabel = new QLabel(tr("Learning Mode"));
    QComboBox *lmCombo = new QComboBox;
    lmCombo->addItem(tr("Explore"));
    lmCombo->addItem(tr("Online"));

    QGridLayout *paramsLayout = new QGridLayout;
    paramsLayout->addWidget(drLabel, 0, 0);
    paramsLayout->addWidget(drEdit, 0, 1);
    paramsLayout->addWidget(acuLabel, 1, 0);
    paramsLayout->addWidget(acuEdit, 1, 1);
    paramsLayout->addWidget(lmLabel, 2, 0);
    paramsLayout->addWidget(lmCombo, 2, 1);
    paramsGroup->setLayout(paramsLayout);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(lifeGroup);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(comGroup);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(paramsGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

StaticSpiritPage::StaticSpiritPage(QWidget *parent) :
    QWidget(parent)
{
    // life
    QGroupBox *lifeGroup = new QGroupBox(tr("Default Life Value"));
    QLabel *cheeseLifeLabel = new QLabel(tr("Cheese Life"));
    QLineEdit *cheeseLifeEdit = new QLineEdit();
    QLabel *nailLifeLabel = new QLabel(tr("Nail Life"));
    QLineEdit *nailLifeEdit = new QLineEdit();

    QGridLayout *lifeLayout = new QGridLayout;
    lifeLayout->addWidget(cheeseLifeLabel, 0, 0);
    lifeLayout->addWidget(cheeseLifeEdit, 0, 1);
    lifeLayout->addWidget(nailLifeLabel, 1, 0);
    lifeLayout->addWidget(nailLifeEdit, 1, 1);
    lifeGroup->setLayout(lifeLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(lifeGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
