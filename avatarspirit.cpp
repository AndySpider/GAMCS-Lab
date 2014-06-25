#include <QObject>
#include <QDebug>
#include <QList>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <gamcs/Sqlite.h>   // FIXME: check existing
#include <gamcs/StateInfoParser.h>
#include "avatarspirit.h"
#include "scene.h"
#include "config.h"
#include "setdialog.h"
#include "memhandler.h"

AvatarSpirit::AvatarSpirit(int id) : Avatar(id), mychannel(NULL), memhandler(NULL), myagent(NULL),
    learning_mode(Agent::ONLINE), tmp_delta_grid_x(0), tmp_delta_grid_y(0),
    radar_range(5), is_awake(true), is_sending(false), storage(""), com_freq(5),
    com_count(qrand() % (com_freq * 2))
{
    _category = AVATARSPIRIT;
    myagent = new CSOSAgent(id, 0.95, 0.001);
    myagent->setMode(learning_mode);
    connectAgent(myagent);
}

AvatarSpirit::~AvatarSpirit()
{
    delete myagent;
}

void AvatarSpirit::setId(int i)
{
    id = i;
}

int AvatarSpirit::getId()
{
    return id;
}

void AvatarSpirit::setRadarRange(int range)
{
    radar_range = range;
}

int AvatarSpirit::getRadarRange()
{
    return radar_range;
}

void AvatarSpirit::setAwake(bool val)
{
    is_awake = val;
}

bool AvatarSpirit::isAwake()
{
    return is_awake;
}

void AvatarSpirit::setLearningMode(Agent::Mode lm)
{
    learning_mode = lm;
    myagent->setMode(learning_mode);
}

Agent::Mode AvatarSpirit::getLearningMode()
{
    return learning_mode;
}

void AvatarSpirit::setChannel(Channel *c)
{
    mychannel = c;
}

void AvatarSpirit::setComFreq(int f)
{
    com_freq = f;
}

int AvatarSpirit::getComFreq()
{
    return com_freq;
}

QList<AvatarSpirit *> AvatarSpirit::getNeighbors()
{
    if (radar_range <= 0)	// radar is off
        return QList<AvatarSpirit *>();

    QList<AvatarSpirit *>neighs;
    qreal pix_rang = (qreal) radar_range * GRID_SIZE;
    QRectF neigh_area = this->mapRectToScene(-pix_rang, -pix_rang, pix_rang * 2 + GRID_SIZE, pix_rang * 2 + GRID_SIZE);

    QList<QGraphicsItem *> items = myscene->items(neigh_area, Qt::IntersectsItemShape, Qt::DescendingOrder);

    if (!items.empty())
    {
        for (QList<QGraphicsItem *>::iterator iit = items.begin(); iit != items.end(); ++iit)
        {
            if ((*iit) == this)	// exclude self
                continue;

            AvatarSpirit *spirit = qgraphicsitem_cast<AvatarSpirit *>(*iit);
            if (spirit != NULL && spirit->spiritType() == this->spiritType() && spirit->isAwake())        // only the same type && awake spirit can be a neighbor
            {
                neighs.append(spirit);
            }
        }
    }

    return neighs;
}

void AvatarSpirit::moveUp()
{
    // do not cross the limit line
    if (grid_y <= 0)
        return;

    // check if there are blocks at upper grid
    Spirit *spt = myscene->getSpiritAt(grid_x + tmp_delta_grid_x, grid_y + tmp_delta_grid_y - 1);
    if (spt == NULL || spt->spiritType() != BLOCK)
    {
        tmp_delta_grid_x += 0;   // use tmp pos
        tmp_delta_grid_y += -1;
    }
}

void AvatarSpirit::moveDown()
{
    if (grid_y >= SCENE_HEIGHT - 1) // on the below limit line
        return;

    // check if there are blocks below
    Spirit *spt = myscene->getSpiritAt(grid_x + tmp_delta_grid_x, grid_y + tmp_delta_grid_y + 1);
    if (spt == NULL || spt->spiritType() != BLOCK)
    {
        tmp_delta_grid_x += 0;
        tmp_delta_grid_y += 1;
    }
}

void AvatarSpirit::moveLeft()
{
    if (grid_x <= 0)
        return;

    // check if there are blocks
    Spirit *spt = myscene->getSpiritAt(grid_x + tmp_delta_grid_x - 1, grid_y + tmp_delta_grid_y);
    if (spt == NULL || spt->spiritType() != BLOCK)
    {
        tmp_delta_grid_x += -1;
        tmp_delta_grid_y += 0;
    }
}

void AvatarSpirit::moveRight()
{
    if (grid_x >= SCENE_WIDTH - 1)
        return;

    // check if there are blocks
    Spirit *spt = myscene->getSpiritAt(grid_x + tmp_delta_grid_x + 1, grid_y + tmp_delta_grid_y);
    if (spt == NULL || spt->spiritType() != BLOCK)
    {
        tmp_delta_grid_x += 1;
        tmp_delta_grid_y += 0;
    }
}

QPoint AvatarSpirit::changePos()
{
    grid_x += tmp_delta_grid_x;
    grid_y += tmp_delta_grid_y;

    QPoint new_grid_pos = QPoint(grid_x, grid_y);

    this->setPos(new_grid_pos * GRID_SIZE);

    tmp_delta_grid_x = tmp_delta_grid_y = 0;    // clear tmp values

    return new_grid_pos;
}

void AvatarSpirit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Spirit::paint(painter, option, widget);

    if (!is_awake)  // sleep
    {
        painter->setPen(Qt::darkGray);
        QPainterPath path(QPointF(GRID_SIZE * 0.25, GRID_SIZE * 0.25));
        path.lineTo(GRID_SIZE * 0.75, GRID_SIZE * 0.25);
        path.lineTo(GRID_SIZE * 0.25, GRID_SIZE * 0.75);
        path.lineTo(GRID_SIZE * 0.75, GRID_SIZE * 0.75);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(path);
    }

    if (is_sending && radar_range > 0)	// draw radar range for avatars that are sending msg
    {
        QPen radar_pen(_color);
        radar_pen.setStyle(Qt::DashLine);
        painter->setPen(radar_pen);
        qreal pix_rang = (qreal) radar_range * GRID_SIZE;
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(-pix_rang, -pix_rang, pix_rang * 2 + GRID_SIZE, pix_rang * 2 + GRID_SIZE);

        is_sending = false;		// reset
    }

    return;
}

void AvatarSpirit::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);

    QString tips;
    QTextStream(&tips) << "id:" << id << ", life:" << _life;
    setToolTip(tips);
    update();
}

// popup menu
void AvatarSpirit::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;

    // mark
    QAction *toggle_mark;
    if (isMarked())
        toggle_mark = menu.addAction("Unmark");
    else
        toggle_mark = menu.addAction("Mark");

    // awake
    QAction *toggle_awake;
    if (isAwake())
        toggle_awake = menu.addAction("Sleep");
    else
        toggle_awake = menu.addAction("Wake up");

    // radar
    QMenu *radar = menu.addMenu("Radar");
    QAction *radar_range = radar->addAction("Set Range");
    QAction *radar_freq = radar->addAction("Set Freq");

    // learning mode
    QMenu *lmode = menu.addMenu("Learning Mode");
    QAction *online = lmode->addAction("Online mode");
    online->setCheckable(true);
    QAction *explore = lmode->addAction("Explore mode");
    explore->setCheckable(true);
    if (learning_mode == Agent::ONLINE)
        online->setChecked(true);
    else
        explore->setChecked(true);

    // memory
    QMenu *memory = menu.addMenu("Memory");
    QAction *save = memory->addAction("Save...");
    QAction *save_as = memory->addAction("Save As...");
    QAction *load = memory->addAction("Load...");

    // life
    QAction *setlife = menu.addAction("Set life");

    // show the menu
    QAction *selectedAction = menu.exec(event->screenPos());

    // judge selected action
    if (selectedAction == toggle_mark)
    {
        this->setMarked(!this->isMarked());
    }
    else if (selectedAction == toggle_awake)
    {
        this->setAwake(!this->isAwake());
    }
    else if (selectedAction == radar_range)
    {
        SetDialog dialog("Radar Range", QString::number(getRadarRange()));
        if (dialog.exec())
        {
            bool ok;
            int new_radar_range = dialog.getValue().toInt(&ok, 10);
            if (ok)
            {
                this->setRadarRange(new_radar_range);
                qDebug() << "+++ Radar range set to" << new_radar_range;
            }
            else
            {
                qDebug() << "--- invalid radar range, should be integers!";
            }
        }
    }
    else if (selectedAction == radar_freq)
    {
        SetDialog dialog("Radar Freq", QString::number(getComFreq()));
        if (dialog.exec())
        {
            bool ok;
            int new_radar_freq = dialog.getValue().toInt(&ok, 10);
            if (ok && new_radar_freq > 0)
            {
                this->setComFreq(new_radar_freq);
                qDebug() << "+++ Average freq set to" << new_radar_freq;
            }
            else
            {
                qDebug() << "--- invalid radar freq, should be intergers > 0!";
            }
        }
    }
    else if (selectedAction == online)
    {
        qDebug() << "mouse change mode to ONLINE";
        myagent->setMode(Agent::ONLINE);
        learning_mode = Agent::ONLINE;
    }
    else if (selectedAction == explore)
    {
        qDebug() << "mouse change mode to EXPLORE";
        myagent->setMode(Agent::EXPLORE);
        learning_mode = Agent::EXPLORE;
    }
    else if (selectedAction == save)
    {
        if (storage.isEmpty())
        {
            storage = QFileDialog::getSaveFileName(NULL, QObject::tr("Save Avatar Memory..."), QString(),
                                                   QObject::tr("Avatar Memory (*.mem);; All Files(*)"));
        }

        if (!storage.endsWith(".mem", Qt::CaseInsensitive))
            storage += ".mem";

        // save memory to storage
        memhandler = new MemHandler(this, myagent, storage, 1);
        QObject::connect(memhandler, SIGNAL(finished()), memhandler, SLOT(deleteLater()));
        memhandler->start();    // It may take a while, start a thread to handle memory
    }
    else if (selectedAction == save_as)
    {
        storage = QFileDialog::getSaveFileName(NULL, QObject::tr("Save Avatar Memory..."), QString(),
                                               QObject::tr("Avatar Memory (*.mem);; All Files(*)"));
        if (!storage.endsWith(".mem", Qt::CaseInsensitive))
            storage += ".mem";

        // save memory to storage
        memhandler = new MemHandler(this, myagent, storage, 1);
        QObject::connect(memhandler, SIGNAL(finished()), memhandler, SLOT(deleteLater()));
        memhandler->start();
    }
    else if (selectedAction == load)
    {
        storage = QFileDialog::getOpenFileName(NULL, QObject::tr("Load Avatar Memory..."), QString(),
                                               QObject::tr("Avatar Memory (*.mem);; All Files(*)"));

        if (!storage.isEmpty())
        {
            // load memory from storage
            memhandler = new MemHandler(this, myagent, storage, 0);
            QObject::connect(memhandler, SIGNAL(finished()), memhandler, SLOT(deleteLater()));
            memhandler->start();
        }
    }
    else if (selectedAction == setlife)
    {
        SetDialog dialog("Life:", QString::number(_life));
        if (dialog.exec())
        {
            bool ok;
            float new_life = dialog.getValue().toFloat(&ok);
            if (ok)
            {
                this->_life = new_life;
                qDebug() << "+++ life set to" << new_life;
            }
            else
            {
                qDebug() << "--- invalid life value, should be float!";
            }
        }
    }

    update();
}

Agent::State AvatarSpirit::perceiveState()
{
#ifdef GLOBAL_SENSE
    int st = grid_x + grid_y * SCENE_WIDTH;

#else
    // perceive the spirits at four directions
    int stype0 = 0, stype1 = 0, stype2 = 0, stype3 = 0, stype4 = 0;

    // current pos
    QList<Spirit *> colliding_spirits = collidingSpirits();
    if (colliding_spirits.empty())
        stype0 = 0;
    else if (colliding_spirits[0]->spiritType() == BLOCK)
        stype0 = 1;
    else if (colliding_spirits[0]->spiritType() == CHEESE)
        stype0 = 2;
    else if (colliding_spirits[0]->spiritType() == NAIL)
        stype0 = 3;
    else if (colliding_spirits[0]->spiritType() == MOUSE)
        stype0 = 4;
    else if (colliding_spirits[0]->spiritType() == CAT)
        stype0 = 5;

    // above
    Spirit *spt = myscene->getSpiritAt(grid_x, grid_y - 1);
    if (spt == NULL)
        stype1 = 0;
    else if (spt->spiritType() == BLOCK)
        stype1 = 1;
    else if (spt->spiritType() == CHEESE)
        stype1 = 2;
    else if (spt->spiritType() == NAIL)
        stype1 = 3;
    else if (spt->spiritType() == MOUSE)
        stype1 = 4;
    else if (spt->spiritType() == CAT)
        stype1 = 5;

    // down
    spt = myscene->getSpiritAt(grid_x, grid_y + 1);
    if (spt == NULL)
        stype2 = 0;
    else if (spt->spiritType() == BLOCK)
        stype2 = 1;
    else if (spt->spiritType() == CHEESE)
        stype2 = 2;
    else if (spt->spiritType() == NAIL)
        stype2 = 3;
    else if (spt->spiritType() == MOUSE)
        stype2 = 4;
    else if (spt->spiritType() == CAT)
        stype2 = 5;

    // left
    spt = myscene->getSpiritAt(grid_x - 1, grid_y);
    if (spt == NULL)
        stype3 = 0;
    else if (spt->spiritType() == BLOCK)
        stype3 = 1;
    else if (spt->spiritType() == CHEESE)
        stype3 = 2;
    else if (spt->spiritType() == NAIL)
        stype3 = 3;
    else if (spt->spiritType() == MOUSE)
        stype3 = 4;
    else if (spt->spiritType() == CAT)
        stype3 = 5;

    // right
    spt = myscene->getSpiritAt(grid_x + 1, grid_y);
    if (spt == NULL)
        stype4 = 0;
    else if (spt->spiritType() == BLOCK)
        stype4 = 1;
    else if (spt->spiritType() == CHEESE)
        stype4 = 2;
    else if (spt->spiritType() == NAIL)
        stype4 = 3;
    else if (spt->spiritType() == MOUSE)
        stype4 = 4;
    else if (spt->spiritType() == CAT)
        stype4 = 5;

    int st = stype0 + stype1 * 6 + stype2 * 36 + stype3 * 216 + stype4 * 1296;
#endif

    return st;
}

void AvatarSpirit::performAction(Agent::Action act)
{
    switch (act)
    {
    case 1: // move up
        moveUp();
        break;
    case 2: // move down
        moveDown();
        break;
    case 3: // move left
        moveLeft();
        break;
    case 4: // move right
        moveRight();
        break;
    case 5: // no move
        break;
    }
}

OSpace AvatarSpirit::availableActions(Agent::State st)
{
    Q_UNUSED(st);

    OSpace acts;

    Spirit *spt;
    // check upper grid
    spt = myscene->getSpiritAt(grid_x, grid_y -1);
    if (spt == NULL || spt->spiritType() != BLOCK)
        acts.add(1);

    // down
    spt = myscene->getSpiritAt(grid_x, grid_y + 1);
    if (spt == NULL || spt->spiritType() != BLOCK)
        acts.add(2);

    // left
    spt = myscene->getSpiritAt(grid_x - 1, grid_y);
    if (spt == NULL || spt->spiritType() != BLOCK)
        acts.add(3);

    // right
    spt = myscene->getSpiritAt(grid_x + 1, grid_y);
    if (spt == NULL || spt->spiritType() != BLOCK)
        acts.add(4);

    // stop
    acts.add(5);
    return acts;
}

float AvatarSpirit::originalPayoff(Agent::State st)
{
    Q_UNUSED(st);
    return 1.0;
}

void AvatarSpirit::act()
{
    if (isAwake())
    {
        Avatar::step();

        // handle communication
        bool com = timeToCom();
        if (com && getRadarRange() > 0)     // send state info
        {
            Agent::State st = myagent->nextState();
            if (st == Agent::INVALID_STATE)	// wrap
                st = myagent->firstState();

            State_Info_Header *stif = myagent->getStateInfo(st);

            // get neighbors
            QList<AvatarSpirit *> neighs = getNeighbors();
            for (QList<AvatarSpirit *>::iterator nit = neighs.begin(); nit != neighs.end(); ++ nit)
            {
                sendMsg((*nit), stif);
            }

            free(stif); // free memory
            is_sending = true;
        }
    }
    else
    {
        // do nothing when not awake
    }
}

void AvatarSpirit::postAct()
{
    changePos();
}

void AvatarSpirit::sendMsg(AvatarSpirit *receiver, State_Info_Header *stif)
{
//    qDebug() << "Avatar" << id << "send msg to" << receiver->id;
    mychannel->putMsg(this, receiver, stif);
}

void AvatarSpirit::recvMsg(const State_Info_Header *recstif)
{
    // do the merge
    State_Info_Header *mystif = myagent->getStateInfo(recstif->st);

    if (mystif == NULL)
    {
        myagent->addStateInfo(recstif);
        return;
    }
    else
    {
        // merge mystif with restif
        // copy the incoming restif
        char stif_buf[recstif->size];
        memcpy(stif_buf, recstif, recstif->size);
        State_Info_Header *copy_stif = (State_Info_Header *) recstif;

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
                            buf_eaif = (EnvAction_Info *) buf_eapt;
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
        free(mystif);   // free memory
        free(sthd);
    }
}

bool AvatarSpirit::timeToCom()
{
    bool re = false;
    if (com_count <= 0)
    {
        com_count = qrand() % com_freq * 2;	// [0, 2 * com_freq)    // set a new value
        re = true;
    }
    else
    {
        com_count--;
        re = false;
    }

    return re;
}
