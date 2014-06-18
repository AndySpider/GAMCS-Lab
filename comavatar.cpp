#include <gamcs/StateInfoParser.h>
#include <math.h>
#include "comavatar.h"
#include "scene.h"

ComAvatar::ComAvatar(int id) : Avatar(id), learning_mode(Agent::ONLINE), com_count(5)
{
    myagent = new CSOSAgent(id, 0.9, 0.01);
    myagent->setMode(learning_mode);
    connectAgent(myagent);
}

ComAvatar::~ComAvatar()
{
    delete myagent;
}

void ComAvatar::sendMsg(ComAvatar *receiver, State_Info_Header *stif)
{
    Scene *myscene;	// FIXME
    myscene->putMsg(this, receiver, stif);

    // set a new com_count
    com_count = qrand() % 10;	// [0, 10)
}

void ComAvatar::recvMsg(const State_Info_Header *stif)
{
    // do the merge
    State_Info_Header *mystif = myagent->getStateInfo(stif->st);

    if (mystif == NULL)
    {
        myagent->addStateInfo(stif);
        return;
    }
    else
    {
        // merge mystif with stif

        // copy the incoming stif
        char stif_buf[stif->size];
        memcpy(stif_buf, stif, stif->size);
        State_Info_Header *copy_stif = (State_Info_Header *) stif;

        char act_buffer[mystif->act_num + copy_stif->act_num][mystif->size
                + copy_stif->size]; //buffer for manipulaing act infos, make sure it's big enough
        int act_num = 0;

        /****** halve eat count first ********/
        Action_Info_Header *achd = NULL;
        EnvAction_Info *eaif = NULL;

        StateInfoParser myparser(mystif);
        achd = myparser.firstAct();
        while (achd != NULL)
        {
            eaif = myparser.firstEat();
            while (eaif != NULL)
            {
                eaif->count = round(eaif->count / 2.0);
                eaif = myparser.nextEat();
            }

            achd = myparser.nextAct();
        }

        // tmp_stif, copy all acts to buffer
        StateInfoParser copyparser(copy_stif);
        achd = copyparser.firstAct();
        while (achd != NULL)
        {
            eaif = copyparser.firstEat();
            while (eaif != NULL)
            {
                eaif->count = round(eaif->count / 2.0);
                eaif = copyparser.nextEat();
            }
            memcpy(act_buffer[act_num], achd, sizeof(Action_Info_Header) + achd->eat_num * sizeof(EnvAction_Info));
            act_num++;	// increase act count
            achd = copyparser.nextAct();
        }
        /*************************************/

        unsigned char *buf_acpt = NULL;
        Action_Info_Header *buf_achd = NULL;
        unsigned char *buf_eapt = NULL;
        EnvAction_Info *buf_eaif = NULL;
        // compare each act from tmp_origsthd with acts from tmp_recvsthd
        int tmp_act_num = act_num; // act_num will be changed
        achd = myparser.firstAct();
        while (achd != NULL)
        {
            int i;
            // traverse all acts in buffer
            for (i = 0; i < tmp_act_num; i++)
            {
                buf_acpt = (unsigned char *) act_buffer[i];
                buf_achd = (Action_Info_Header *) buf_acpt;
                if (buf_achd->act == achd->act)
                {
                    // compare each eat from tmp_origsthd with eats from tmp_recvsthd
                    int tmp_eat_num = buf_achd->eat_num; // eat_num will be changed
                    eaif = myparser.firstEat();
                    while (eaif != NULL)
                    {
                        buf_eapt = (buf_acpt + sizeof(Action_Info_Header)); // move to the first eat
                        buf_eaif = (EnvAction_Info *) buf_eapt;
                        int j;
                        // traverse all eats of current act in buffer
                        for (j = 0; j < tmp_eat_num; j++)
                        {
                            if (buf_eaif->eat == eaif->eat)
                            {
                                buf_eaif->count += eaif->count; // add up eat count
                                break;
                            }
                            buf_eapt += sizeof(EnvAction_Info); // next eat info
                        }
                        if (j >= tmp_eat_num) // eat not found, it's a new eat in mystif
                        {
                            // append it to current act buffer
                            memcpy(
                                        buf_acpt + sizeof(Action_Info_Header)
                                        + buf_achd->eat_num
                                        * sizeof(EnvAction_Info), eaif,
                                        sizeof(EnvAction_Info));
                            buf_achd->eat_num++;
                        }
                        eaif = myparser.nextEat();
                    }
                    break;
                }
            }
            if (i >= tmp_act_num) // act not found, it's a new act in mystif
            {
                // append it to act buffer
                memcpy(act_buffer[act_num], achd,
                       sizeof(Action_Info_Header)
                       + achd->eat_num * sizeof(EnvAction_Info));
                act_num++;
            }
            achd = myparser.nextAct();
        }
        // get total sthd_size
        unsigned int sthd_size = 0;
        sthd_size += sizeof(State_Info_Header);
        for (int i = 0; i < act_num; i++)
        {
            buf_achd = (Action_Info_Header *) act_buffer[i];
            sthd_size += sizeof(Action_Info_Header)
                    + buf_achd->eat_num * sizeof(EnvAction_Info);
        }
        State_Info_Header *sthd = (State_Info_Header *) malloc(sthd_size);
        // fill the header
        sthd->st = copy_stif->st;
        sthd->act_num = act_num;
        sthd->count = round((mystif->count + copy_stif->count) / 2.0);
        sthd->payoff = copy_stif->payoff;
        sthd->original_payoff = copy_stif->original_payoff;
        sthd->size = sthd_size;
        // copy act info from buffer
        unsigned char *ptr = (unsigned char *) sthd;
        ptr += sizeof(State_Info_Header); // point to the first act
        unsigned int act_size = 0;
        for (int i = 0; i < act_num; i++)
        {
            buf_achd = (Action_Info_Header *) act_buffer[i];
            act_size = sizeof(Action_Info_Header)
                    + buf_achd->eat_num * sizeof(EnvAction_Info);
            memcpy(ptr, buf_achd, act_size);
            ptr += act_size;
        }

        // get sthd
        myagent->updateStateInfo(sthd);
    }
}

bool ComAvatar::timeToCom()
{
    return --com_count <= 0;
}
