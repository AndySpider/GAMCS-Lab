#include <avatarspirit.h>
#include <gamcs/CSOSAgent.h>
#include <gamcs/Sqlite.h>
#include <QDebug>
#include "memhandler.h"

using namespace gamcs;

MemHandler::MemHandler(AvatarSpirit *avspt, gamcs::CSOSAgent *agent, const QString &storage, int op) :
    _avspt(avspt), _agent(agent), _storage(storage), _operation(op)
{
}

MemHandler::~MemHandler()
{
}

void MemHandler::run()
{
    // put avatar to sleep while loading/saving
    bool status = _avspt->isAwake();
    _avspt->setAwake(false);

    Sqlite db(_storage.toStdString());
    if (_operation == 0)    // loading
    {
        qDebug() << "+++ loading memory...";
        _agent->loadMemoryFromStorage(&db);
    }
    else if (_operation == 1)   // saving
    {
        qDebug() << "+++ saving memory...";
        _agent->dumpMemoryToStorage(&db);
    }

    // restore
    _avspt->setAwake(status);
    qDebug() << "--- memory done!";
}
