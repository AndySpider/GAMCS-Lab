#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "setdialog.h"

SetDialog::SetDialog(QString lab, QString oldVal)
{
    label = new QLabel(lab);
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    valText = new QLineEdit(oldVal);
    valText->setFocus();

    QGridLayout *gLayout = new QGridLayout;
    gLayout->setColumnStretch(1, 2);
    gLayout->addWidget(label, 0, 0);
    gLayout->addWidget(valText, 0, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    gLayout->addLayout(buttonLayout, 2, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    setWindowTitle(tr("Set a new value"));
}

QString SetDialog::getValue()
{
    return valText->text();
}
