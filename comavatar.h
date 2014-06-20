#ifndef COMAVATAR_H
#define COMAVATAR_H
#include <gamcs/Avatar.h>
#include <gamcs/CSOSAgent.h>

class Channel;

using namespace gamcs;

class ComAvatar : public Avatar
{
public:
    ComAvatar(int id);
    virtual ~ComAvatar();

    void setChannel(Channel *c);
    void setAvgFreq(int f);
    int getAvgFreq();

    void sendMsg(ComAvatar *receiver, State_Info_Header *stif);
    void recvMsg(const State_Info_Header *recstif);

protected:    
    bool timeToCom();

    Channel *mychannel;
    CSOSAgent *myagent;
    Agent::Mode learning_mode;

private:
    int com_freq;
    long com_count;
};

#endif // COMAVATAR_H
