#ifndef MEMHANDLER_H
#define MEMHANDLER_H
#include <QThread>

class AvatarSpirit;
namespace gamcs {
class CSOSAgent;
}

class MemHandler : public QThread
{
    Q_OBJECT

public:
    MemHandler(AvatarSpirit *avspt, gamcs::CSOSAgent *agent, const QString &storage, int op);
    ~MemHandler();

private:
    void run();

    AvatarSpirit *_avspt;
    gamcs::CSOSAgent *_agent;
    QString _storage;
    int _operation;     // 0 for loading, 1 for saving
};

#endif // MEMHANDLER_H
