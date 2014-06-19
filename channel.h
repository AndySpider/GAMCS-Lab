#ifndef CHANNEL_H
#define CHANNEL_H
#include <QList>
#include "comavatar.h"

struct Message;

using namespace gamcs;

class Channel
{
public:
    Channel();

    void putMsg(ComAvatar *sender, ComAvatar *receiver, State_Info_Header *stif);
    void dispatchMsg();

private:
    QList<Message> msg_pool;
};

struct Message
{
    ComAvatar *sender;
    ComAvatar *receiver;
    char *data;
};

#endif // CHANNEL_H
