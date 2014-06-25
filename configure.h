#ifndef CONFIGURE_H
#define CONFIGURE_H
#include <QString>
#include <QVariant>
#include <QMap>

const int GRID_SIZE = 10;

class Configure
{
public:
    Configure();

    void setConfigFile(const QString &file);
    void load();
    void save();

    QVariant getValue(const QString &what);
    void setValue(const QString &what, const QVariant &value);

private:
    QString config_file;
    QMap<QString, QVariant> config_map;
};

// global configure
extern Configure g_config;

#endif // CONFIGURE_H
