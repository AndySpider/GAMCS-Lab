#ifndef CHANNEL_H
#define CHANNEL_H
#include <QList>
#include "avatarspirit.h"

struct Message;

using namespace gamcs;

class Channel
{
public:
    Channel();

    void putMsg(AvatarSpirit *sender, AvatarSpirit *receiver, State_Info_Header *stif);
    void dispatchMsg();

private:
    QList<Message> msg_pool;
};

struct Message
{
    AvatarSpirit *sender;
    AvatarSpirit *receiver;
    char *data;
};

#endif // CHANNEL_H
