#ifndef COMAVATAR_H
#define COMAVATAR_H
#include <gamcs/Avatar.h>
#include <gamcs/CSOSAgent.h>

using namespace gamcs;

class ComAvatar : public Avatar
{
public:
    ComAvatar(int id);
    virtual ~ComAvatar();

    void sendMsg(ComAvatar *receiver, State_Info_Header *stif);
    void recvMsg(const State_Info_Header *stif);

    bool timeToCom();

protected:
    CSOSAgent *myagent;
    Agent::Mode learning_mode;
    unsigned long com_count;
};

#endif // COMAVATAR_H
