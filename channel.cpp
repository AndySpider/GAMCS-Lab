#include <QDebug>
#include <string.h>
#include "channel.h"

Channel::Channel()
{
}

void Channel::putMsg(AvatarSpirit *sender, AvatarSpirit *receiver, State_Info_Header *stif)
{
    struct Message msg;
    msg.sender = sender;
    msg.receiver = receiver;
    msg.data = (char *) malloc(stif->size);
    memcpy(msg.data, stif, stif->size);

    msg_pool.append(msg);
}

void Channel::dispatchMsg()
{
//    qDebug() << msg_pool.size() << "msgs to be dispatched...";
    for (QList<Message>::iterator mit = msg_pool.begin(); mit != msg_pool.end(); ++mit)
    {
        AvatarSpirit *recver = mit->receiver;
        recver->recvMsg((State_Info_Header *) mit->data);

        // free data
        free(mit->data);
    }

    // clear msgs
    msg_pool.clear();
}
