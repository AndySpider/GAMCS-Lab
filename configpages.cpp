#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include "configpages.h"
#include "configure.h"

ConfigPage::ConfigPage(QWidget *parent) :
    QWidget(parent)
{
}

GeneralPage::GeneralPage(QWidget *parent) :
    ConfigPage(parent)
{
    // note banner
    QLabel *noteLabel = new QLabel(tr("<i>Note:: All the changes are applied only to new created Scenes or Spirits.</i>"));

    // scene size
    QGroupBox *sizeGroup = new QGroupBox(tr("Scene Size"));
    QLabel *widthLabel = new QLabel(tr("width"));
    widthEdit = new QLineEdit(g_config.getValue("Scene/Size/Width").toString());
    widthEdit->setToolTip(tr("int (>0), unit: grid"));
    QLabel *heightLabel = new QLabel(tr("height"));
    heightEdit = new QLineEdit(g_config.getValue("Scene/Size/Height").toString());
    heightEdit->setToolTip(tr("int (>0), unit: grid"));

    QGridLayout *sizeLayout = new QGridLayout;
    sizeLayout->addWidget(widthLabel, 0, 0);
    sizeLayout->addWidget(widthEdit, 0, 1);
    sizeLayout->addWidget(heightLabel, 1, 0);
    sizeLayout->addWidget(heightEdit, 1, 1);
    sizeGroup->setLayout(sizeLayout);

    // hunt diagram
//    QGroupBox *huntGroup = new QGroupBox(tr("Hunting Diagram"));
//    HuntDia *huntDia = new HuntDia(this);

//    QVBoxLayout *huntLayout = new QVBoxLayout;
//    huntLayout->addWidget(huntDia);
//    huntGroup->setLayout(huntLayout);

    QPushButton *defaultButton = new QPushButton(tr("Default"));
    connect(defaultButton, SIGNAL(clicked()), this, SLOT(resetDefault()));
    QHBoxLayout *defaultLayout = new QHBoxLayout;
    defaultLayout->addWidget(defaultButton, 0, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(noteLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(sizeGroup);
//    mainLayout->setSpacing(10);
//    mainLayout->addWidget(huntGroup);
    mainLayout->addStretch(1);
    mainLayout->addLayout(defaultLayout);
    setLayout(mainLayout);
}

void GeneralPage::applyConfig()
{
    bool ok = false;
    int scene_size_width = widthEdit->text().toInt(&ok);
    if (ok)
        g_config.setValue("Scene/Size/Width", scene_size_width);

    int scene_size_height = heightEdit->text().toInt(&ok);
    if (ok)
        g_config.setValue("Scene/Size/Height", scene_size_height);
}

void GeneralPage::resetDefault()
{
    widthEdit->setText("512");
    heightEdit->setText("256");
}

AvatarSpiritPage::AvatarSpiritPage(QWidget *parent) :
    ConfigPage(parent)
{
    // life
    QGroupBox *lifeGroup = new QGroupBox(tr("Default Life Value"));
    QLabel *mouseLifeLabel = new QLabel(tr("Mouse Life"));
    mouseLifeEdit = new QLineEdit(g_config.getValue("AvatarSpirit/Life/Mouse").toString());
    mouseLifeEdit->setToolTip(tr("float (>0)"));
    QLabel *catLifeLabel = new QLabel(tr("Cat Life"));
    catLifeEdit = new QLineEdit(g_config.getValue("AvatarSpirit/Life/Cat").toString());
    catLifeEdit->setToolTip(tr("float (>0)"));
    QLabel *elephantLifeLabel = new QLabel(tr("Elephant Life"));
    elephantLifeEdit = new QLineEdit(g_config.getValue("AvatarSpirit/Life/Elephant").toString());
    elephantLifeEdit->setToolTip(tr("float (>0)"));

    QGridLayout *lifeLayout = new QGridLayout;
    lifeLayout->addWidget(mouseLifeLabel, 0, 0);
    lifeLayout->addWidget(mouseLifeEdit, 0, 1);
    lifeLayout->addWidget(catLifeLabel, 1, 0);
    lifeLayout->addWidget(catLifeEdit, 1, 1);
    lifeLayout->addWidget(elephantLifeLabel, 2, 0);
    lifeLayout->addWidget(elephantLifeEdit, 2, 1);
    lifeGroup->setLayout(lifeLayout);

    // sharing
    QGroupBox *shareGroup = new QGroupBox(tr("Default Sharing Params"));
    QLabel *rangeLabel = new QLabel(tr("Share Range"));
    rangeEdit = new QLineEdit(g_config.getValue("AvatarSpirit/ShareParams/ShareRange").toString());
    rangeEdit->setToolTip(tr("int (>=0, 0 to turn off sharing)"));
    QLabel *freqLabel = new QLabel(tr("Share Freq"));
    freqEdit = new QLineEdit(g_config.getValue("AvatarSpirit/ShareParams/ShareFreq").toString());
    freqEdit->setToolTip(tr("int (>0)"));

    QGridLayout *shareLayout = new QGridLayout;
    shareLayout->addWidget(rangeLabel, 0, 0);
    shareLayout->addWidget(rangeEdit, 0, 1);
    shareLayout->addWidget(freqLabel, 1, 0);
    shareLayout->addWidget(freqEdit, 1, 1);
    shareGroup->setLayout(shareLayout);

    // gamcs params
    QGroupBox *paramsGroup = new QGroupBox(tr("Default GAMCS Parameters"));
    QLabel *drLabel = new QLabel(tr("Discount Rate"));
    drEdit = new QLineEdit(g_config.getValue("AvatarSpirit/GAMCSParams/DiscountRate").toString());
    drEdit->setToolTip(tr("float (>=0, <1)"));
    QLabel *acuLabel = new QLabel(tr("Accuracy"));
    acuEdit = new QLineEdit(g_config.getValue("AvatarSpirit/GAMCSParams/Accuracy").toString());
    acuEdit->setToolTip(tr("float (>=0)"));
    QLabel *lmLabel = new QLabel(tr("Learning Mode"));
    lmCombo = new QComboBox;
    lmCombo->addItem(tr("Online"));
    lmCombo->addItem(tr("Explore"));
    if (g_config.getValue("AvatarSpirit/GAMCSParams/LearningMode").toString() == "Explore")
        lmCombo->setCurrentIndex(1);
    else	// online is the default
        lmCombo->setCurrentIndex(0);

    QGridLayout *paramsLayout = new QGridLayout;
    paramsLayout->addWidget(drLabel, 0, 0);
    paramsLayout->addWidget(drEdit, 0, 1);
    paramsLayout->addWidget(acuLabel, 1, 0);
    paramsLayout->addWidget(acuEdit, 1, 1);
    paramsLayout->addWidget(lmLabel, 2, 0);
    paramsLayout->addWidget(lmCombo, 2, 1);
    paramsGroup->setLayout(paramsLayout);

    QPushButton *defaultButton = new QPushButton(tr("Default"));
    connect(defaultButton, SIGNAL(clicked()), this, SLOT(resetDefault()));
    QHBoxLayout *defaultLayout = new QHBoxLayout;
    defaultLayout->addWidget(defaultButton, 0, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(lifeGroup);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(shareGroup);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(paramsGroup);
    mainLayout->addStretch(1);
    mainLayout->addLayout(defaultLayout);
    setLayout(mainLayout);
}

void AvatarSpiritPage::applyConfig()
{
    bool ok = false;
    float mouse_life = mouseLifeEdit->text().toFloat(&ok);
    if (ok)
        g_config.setValue("AvatarSpirit/Life/Mouse", mouse_life);

    float cat_life = catLifeEdit->text().toFloat(&ok);
    if (ok)
        g_config.setValue("AvatarSpirit/Life/Cat", cat_life);

    float elephant_life = elephantLifeEdit->text().toFloat(&ok);
    if (ok)
        g_config.setValue("AvatarSpirit/Life/Elephant", elephant_life);

    int share_range = rangeEdit->text().toInt(&ok);
    if (ok)
        g_config.setValue("AvatarSpirit/ShareParams/ShareRange", share_range);

    int share_freq = freqEdit->text().toInt(&ok);
    if (ok)
        g_config.setValue("AvatarSpirit/ShareParams/ShareFreq", share_freq);

    float dr = drEdit->text().toFloat(&ok);
    if (ok)
        g_config.setValue("AvatarSpirit/GAMCSParams/DiscountRate", dr);

    float acu = acuEdit->text().toFloat(&ok);
    if (ok)
        g_config.setValue("AvatarSpirit/GAMCSParams/Accuracy", acu);

    QString lm = lmCombo->currentText();
    g_config.setValue("AvatarSpirit/GAMCSParams/LearningMode", lm);
}

void AvatarSpiritPage::resetDefault()
{
    mouseLifeEdit->setText("30");
    catLifeEdit->setText("50");
    elephantLifeEdit->setText("100");
    rangeEdit->setText("5");
    freqEdit->setText("5");
    drEdit->setText("0.9");
    acuEdit->setText("0.001");
    lmCombo->setCurrentIndex(0);
}

StaticSpiritPage::StaticSpiritPage(QWidget *parent) :
    ConfigPage(parent)
{
    // life
    QGroupBox *lifeGroup = new QGroupBox(tr("Default Life Value"));
    QLabel *cheeseLifeLabel = new QLabel(tr("Cheese Life"));
    cheeseLifeEdit = new QLineEdit(g_config.getValue("StaticSpirit/Life/Cheese").toString());
    cheeseLifeEdit->setToolTip(tr("float (>0)"));
    QLabel *nailLifeLabel = new QLabel(tr("Nail Life"));
    nailLifeEdit = new QLineEdit(g_config.getValue("StaticSpirit/Life/Nail").toString());
    nailLifeEdit->setToolTip(tr("float (>0)"));

    QGridLayout *lifeLayout = new QGridLayout;
    lifeLayout->addWidget(cheeseLifeLabel, 0, 0);
    lifeLayout->addWidget(cheeseLifeEdit, 0, 1);
    lifeLayout->addWidget(nailLifeLabel, 1, 0);
    lifeLayout->addWidget(nailLifeEdit, 1, 1);
    lifeGroup->setLayout(lifeLayout);

    QPushButton *defaultButton = new QPushButton(tr("Default"));
    connect(defaultButton, SIGNAL(clicked()), this, SLOT(resetDefault()));
    QHBoxLayout *defaultLayout = new QHBoxLayout;
    defaultLayout->addWidget(defaultButton, 0, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(lifeGroup);
    mainLayout->addStretch(1);
    mainLayout->addLayout(defaultLayout);
    setLayout(mainLayout);
}

void StaticSpiritPage::applyConfig()
{
    bool ok = false;
    float cheese_life = cheeseLifeEdit->text().toFloat(&ok);
    if (ok)
        g_config.setValue("StaticSpirit/Life/Cheese", cheese_life);

    float nail_life = nailLifeEdit->text().toFloat(&ok);
    if (ok)
        g_config.setValue("StaticSpirit/Life/Nail", nail_life);
}

void StaticSpiritPage::resetDefault()
{
    cheeseLifeEdit->setText("15");
    nailLifeEdit->setText("1.5");
}

HuntDia::HuntDia(QWidget *parent) :
    QTableView(parent)
{
}
