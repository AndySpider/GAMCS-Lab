#include <QSettings>
#include <QDebug>
#include "configure.h"

Configure::Configure() : config_file("./GAMCS-Lab.conf")
{
}

void Configure::setConfigFile(const QString &file)
{
    config_file = file;
}

void Configure::load()
{
    config_map.clear();

    QSettings settings(config_file, QSettings::IniFormat);
    QString key;
    QVariant val;
    key = "Scene/Size/Width";
    val = settings.value(key);
    if (!val.isNull())  // only check null, may be invalid
        config_map.insert(key, val);
    else
        config_map.insert(key, 512);

    key = "Scene/Size/Height";
    val = settings.value(key);
    if (!val.isNull())
        config_map.insert(key, val);
    else
        config_map.insert(key, 256);

    key = "AvatarSpirit/Life/Mouse";
    val = settings.value(key);
    if (!val.isNull())
        config_map.insert(key, val);
    else
        config_map.insert(key, 30);

    key = "AvatarSpirit/Life/Cat";
    val = settings.value(key);
    if (!val.isNull())
        config_map.insert(key, val);
    else
        config_map.insert(key, 50);

    key = "AvatarSpirit/Life/Elephant";
    val = settings.value(key);
    if (!val.isNull())
        config_map.insert(key, val);
    else
        config_map.insert(key, 100);

    key = "AvatarSpirit/ComParams/RadarRange";
    val = settings.value(key);
    if (!val.isNull())
        config_map.insert(key, val);
    else
        config_map.insert(key, 0);

    key = "AvatarSpirit/ComParams/ComFreq";
    val = settings.value(key);
    if (!val.isNull())
        config_map.insert(key, val);
    else
        config_map.insert(key, 5);

    key = "AvatarSpirit/GAMCSParams/DiscountRate";
    val = settings.value(key);
    if (!val.isNull())
        config_map.insert(key, val);
    else
        config_map.insert(key, 0.9);

    key = "AvatarSpirit/GAMCSParams/Accuracy";
    val = settings.value(key);
    if (!val.isNull())
        config_map.insert(key, val);
    else
        config_map.insert(key, 0.001);

    key = "AvatarSpirit/GAMCSParams/LearningMode";
    val = settings.value(key);
    if (!val.isNull())
        config_map.insert(key, val);
    else
        config_map.insert(key, QString("Online"));

    key = "StaticSpirit/Life/Cheese";
    val = settings.value(key);
    if (!val.isNull())
        config_map.insert(key, val);
    else
        config_map.insert(key, 15);

    key = "StaticSpirit/Life/Nail";
    val = settings.value(key);
    if (!val.isNull())
        config_map.insert(key, val);
    else
        config_map.insert(key, 1.5);
}

void Configure::save()
{
    QSettings settings(config_file, QSettings::IniFormat);
    for (QMap<QString, QVariant>::iterator it = config_map.begin(); it != config_map.end(); ++it)
    {
        qDebug() << "save " << it.key() << ":" << it.value().toString();
        settings.setValue(it.key(), it.value());
    }
}

QVariant Configure::getValue(const QString &what)
{
    return config_map.value(what, QVariant());
}

void Configure::setValue(const QString &what, const QVariant &value)
{
    config_map.insert(what, value);
}
