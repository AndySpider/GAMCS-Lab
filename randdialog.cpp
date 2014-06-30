#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include "randdialog.h"

RandDialog::RandDialog(QWidget *parent) :
    QDialog(parent)
{
    label = new QLabel(this);
    label->setText(tr("Total number"));
    valText = new QLineEdit(this);
    valText->setFocus();

    checkBlock = new QCheckBox(this);
    checkBlock->setText(tr("Block"));
    checkCheese = new QCheckBox(this);
    checkCheese->setText(tr("Cheese"));
    checkNail = new QCheckBox(this);
    checkNail->setText(tr("Nail"));
    checkBomb = new QCheckBox(this);
    checkBomb->setText((tr("Bomb")));
    checkMouse = new QCheckBox(this);
    checkMouse->setText(tr("Mouse"));
    checkCat = new QCheckBox(this);
    checkCat->setText((tr("Cat")));
    checkElephant = new QCheckBox(this);
    checkElephant->setText(tr("Elephant"));

    cancelButton = new QPushButton("Cancel");
    okButton = new QPushButton("Ok");
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *gLayout = new QHBoxLayout;
    gLayout->addWidget(label);
    gLayout->addWidget(valText);

    QHBoxLayout *checkLayout = new QHBoxLayout;
    checkLayout->addWidget(checkBlock);
    checkLayout->addWidget(checkCheese);
    checkLayout->addWidget(checkNail);
    checkLayout->addWidget(checkBomb);
    checkLayout->addWidget(checkMouse);
    checkLayout->addWidget(checkCat);
    checkLayout->addWidget(checkElephant);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setAlignment(Qt::AlignRight);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    mainLayout->addLayout(checkLayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Generate Spirits Randomly"));
}

int RandDialog::getNum()
{
    bool ok = false;
    int num = valText->text().toInt(&ok);
    int ret;
    if (ok)
    {
        ret = num;
    }
    else
    {
        ret = -1;
        qDebug() << "invalid number:" << valText->text();
    }

    return ret;
}

QList<Spirit::SType> RandDialog::getTypes()
{
    QList<Spirit::SType> types;
    if (checkBlock->isChecked())
        types.append(Spirit::BLOCK);
    if (checkCheese->isChecked())
        types.append(Spirit::CHEESE);
    if (checkNail->isChecked())
        types.append(Spirit::NAIL);
    if (checkBomb->isChecked())
        types.append(Spirit::BOMB);
    if (checkMouse->isChecked())
        types.append(Spirit::MOUSE);
    if (checkCat->isChecked())
        types.append(Spirit::CAT);
    if (checkElephant->isChecked())
        types.append(Spirit::ELEPHANT);

    return types;
}
