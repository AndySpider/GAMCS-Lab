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
    QString key = "Scene/Size/Width";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
    key = "Scene/Size/Height";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
    key = "AvatarSpirit/Life/Mouse";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
    key = "AvatarSpirit/Life/Cat";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
    key = "AvatarSpirit/Life/Elephant";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
    key = "AvatarSpirit/ComParams/RadarRange";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
    key = "AvatarSpirit/ComParams/ComFreq";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
    key = "AvatarSpirit/GAMCSParams/DiscountRate";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
    key = "AvatarSpirit/GAMCSParams/Accuracy";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
    key = "AvatarSpirit/GAMCSParams/LearningMode";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
    key = "StaticSpirit/Life/Cheese";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
    key = "StaticSpirit/Life/Nail";
    qDebug() << "load " << key << settings.value(key).toString();
    config_map.insert(key, settings.value(key));
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
