#include <QObject>
#include <QDebug>
#include <QList>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <gamcs/Sqlite.h>
#include <gamcs/StateInfoParser.h>
#include "avatarspirit.h"
#include "scene.h"
#include "configure.h"
#include "setdialog.h"
#include "memhandler.h"

#if defined(_WIN32)
inline double round(double d)
{
return floor(d + 0.5);
}
#endif

AvatarSpirit::AvatarSpirit(int id) : Avatar(id), mychannel(NULL), memhandler(NULL), myagent(NULL),
    learning_mode(Agent::ONLINE), tmp_delta_grid_x(0), tmp_delta_grid_y(0),
    share_range(0), is_awake(true), is_sending(false), storage(""), share_freq(0),
    _counter(0), discount_rate(0), accuracy(0), is_teach(false)
{
    _category = AVATARSPIRIT;

    // read config
    bool ok;
    int val = g_config.getValue("AvatarSpirit/ShareParams/ShareRange").toInt(&ok);
    if (ok)
        share_range = val;
    else    // default value
    {
        share_range = 0;
        g_config.setValue("AvatarSpirit/ShareParams/ShareRange", share_range);
    }

    val = g_config.getValue("AvatarSpirit/ShareParams/ShareFreq").toInt(&ok);
    if (ok)
    {
        share_freq = val;
        _counter = qrand() % (share_freq * 2);
    }
    else    // default value
    {
        share_freq = 5;
        _counter = qrand() % (share_freq * 2);
        g_config.setValue("AvatarSpirit/ShareParams/ShareFreq", share_freq);
    }

    float fval = g_config.getValue("AvatarSpirit/GAMCSParams/DiscountRate").toFloat(&ok);
    if (ok && fval >= 0 && fval < 1)
        discount_rate = fval;
    else    // default value
    {
        discount_rate = 0.9;
        g_config.setValue("AvatarSpirit/GAMCSParams/DiscountRate", discount_rate);
    }

    fval = g_config.getValue("AvatarSpirit/GAMCSParams/Accuracy").toFloat(&ok);
    if (ok && fval >= 0)
        accuracy = fval;
    else    // default value
    {
        accuracy = 0.001;
        g_config.setValue("AvatarSpirit/GAMCSParams/Accuracy", accuracy);
    }

    QString lm = g_config.getValue("AvatarSpirit/GAMCSParams/LearningMode").toString();
    if (lm == "Explore")
        learning_mode = Agent::EXPLORE;
    else // default value, Note: the TEACH mode is not stored in configure, since it needs support from scene
    {
        learning_mode = Agent::ONLINE;
        g_config.setValue("AvatarSpirit/GAMCSParams/LearningMode", QString("Online"));
    }

    myagent = new CSOSAgent(id, discount_rate, accuracy);
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

int AvatarSpirit::getId() const
{
    return id;
}

void AvatarSpirit::setShareRange(int range)
{
    share_range = range;
}

int AvatarSpirit::getShareRange() const
{
    return share_range;
}

void AvatarSpirit::setAwake(bool val)
{
    is_awake = val;
}

bool AvatarSpirit::isAwake() const
{
    return is_awake;
}

void AvatarSpirit::setTeach(bool val)
{
    is_teach = val;
}

bool AvatarSpirit::isTeach()
{
    return is_teach;
}

void AvatarSpirit::setLearningMode(Agent::Mode lm)
{
    learning_mode = lm;
    myagent->setMode(learning_mode);
}

Agent::Mode AvatarSpirit::getLearningMode() const
{
    return learning_mode;
}

void AvatarSpirit::setChannel(Channel *c)
{
    mychannel = c;
}

void AvatarSpirit::setShareFreq(int f)
{
    share_freq = f;
}

int AvatarSpirit::getShareFreq() const
{
    return share_freq;
}

QList<AvatarSpirit *> AvatarSpirit::getNeighbors() const
{
    if (share_range <= 0)	// share is off
        return QList<AvatarSpirit *>();

    QList<AvatarSpirit *>neighs;
    qreal pix_rang = (qreal) share_range * GRID_SIZE;
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
    if (grid_y >= myscene->height() - 1) // on the below limit line
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
    if (grid_x >= myscene->width() - 1)
        return;

    // check if there are blocks
    Spirit *spt = myscene->getSpiritAt(grid_x + tmp_delta_grid_x + 1, grid_y + tmp_delta_grid_y);
    if (spt == NULL || spt->spiritType() != BLOCK)
    {
        tmp_delta_grid_x += 1;
        tmp_delta_grid_y += 0;
    }
}

QPoint AvatarSpirit::updatePos()
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

    if (is_teach)
    {
        QRectF boundary(-GRID_SIZE / 4, -GRID_SIZE / 4, GRID_SIZE / 2, GRID_SIZE / 2);
        painter->setBrush(Qt::red);
        painter->drawEllipse(boundary);
    }

    if (is_sending && share_range > 0)	// draw share range for avatars that are sending msg
    {
        QPen share_pen(_color);
        share_pen.setStyle(Qt::DashLine);
        painter->setPen(share_pen);
        qreal pix_rang = (qreal) share_range * GRID_SIZE;
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

    // teach
    QAction *toggle_teach;
    if (is_teach)
        toggle_teach = menu.addAction("Un-teach");
    else
        toggle_teach = menu.addAction("Teach");

    // life
    QAction *setlife = menu.addAction("Set life...");

    // share
    QMenu *share = menu.addMenu("Share");
    QAction *set_share_range = share->addAction("Set Range...");
    QAction *set_share_freq = share->addAction("Set Freq...");

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
    else if (selectedAction == toggle_teach)
    {
        if (is_teach)
        {
            myscene->clearTeachSpirit();
        }
        else
        {
            myscene->setTeachSpirit(this);
        }
    }
    else if (selectedAction == set_share_range)
    {
        SetDialog dialog("Share Range", QString::number(getShareRange()));
        if (dialog.exec())
        {
            bool ok;
            int new_share_range = dialog.getValue().toInt(&ok, 10);
            if (ok)
            {
                this->setShareRange(new_share_range);
                qDebug() << "+++ Share range set to" << new_share_range;
            }
            else
            {
                qDebug() << "--- invalid share range, should be integers!";
            }
        }
    }
    else if (selectedAction == set_share_freq)
    {
        SetDialog dialog("Share Freq", QString::number(getShareFreq()));
        if (dialog.exec())
        {
            bool ok;
            int new_share_freq = dialog.getValue().toInt(&ok, 10);
            if (ok && new_share_freq > 0)
            {
                this->setShareFreq(new_share_freq);
                qDebug() << "+++ Share freq set to" << new_share_freq;
            }
            else
            {
                qDebug() << "--- invalid share freq, should be intergers > 0!";
            }
        }
    }
    else if (selectedAction == online)
    {
        qDebug() << "mouse change mode to ONLINE";
        setLearningMode(Agent::ONLINE);
    }
    else if (selectedAction == explore)
    {
        qDebug() << "mouse change mode to EXPLORE";
        setLearningMode(Agent::EXPLORE);
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
    int st = grid_x + grid_y * myscene->width();

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
    acts.add(1, 5, 1);	// the actions are always the same, no matter if there're blocks surrounded
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

        // handle sharing
        bool to_share = timeToShare();
        if (to_share && getShareRange() > 0)     // send state info
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
    updatePos();
}

void AvatarSpirit::sendMsg(AvatarSpirit *receiver, const State_Info_Header *stif)
{
//    qDebug() << "Avatar" << id << "send msg to" << receiver->id;
    mychannel->putMsg(this, receiver, stif);
}

/**
 * @brief AvatarSpirit::mergeStateInfo
 * @param stif1 original state info
 * @param stif2 received state info
 * @return merged state info
 */
State_Info_Header *AvatarSpirit::mergeStateInfo(const State_Info_Header *stif1, const State_Info_Header *stif2)
{
    // merge stif1 with stif2, make a copy first
    char *stif1_buf = (char *) malloc(stif1->size);
    char *stif2_buf = (char *) malloc(stif2->size);
    memcpy(stif1_buf, stif1, stif1->size);
    memcpy(stif2_buf, stif2, stif2->size);
    State_Info_Header *copy_stif1 = (State_Info_Header *) stif1;
    State_Info_Header *copy_stif2 = (State_Info_Header *) stif2;

    // malloc a big enough buffer for manipulating actions
    int nrows = copy_stif1->act_num + copy_stif2->act_num;
    int ncolumns = copy_stif1->size + copy_stif2->size;
    char **acts_buffer = (char **) malloc(nrows * sizeof(char *));
    for (int i = 0; i < nrows; i++)
    {
        acts_buffer[i] = (char *)malloc(ncolumns * sizeof(char));
    }

    int act_num = 0;

    /****** halve eat count first ********/
    Action_Info_Header *achd = NULL;
    EnvAction_Info *eaif = NULL;

    StateInfoParser stif1_parser(copy_stif1);
    achd = stif1_parser.firstAct();
    while (achd != NULL)
    {
        eaif = stif1_parser.firstEat();
        while (eaif != NULL)
        {
            eaif->count = round(eaif->count / 2.0);
            eaif = stif1_parser.nextEat();
        }

        achd = stif1_parser.nextAct();
    }

    // tmp_stif, copy all acts to buffer
    StateInfoParser stif2_parser(copy_stif2);
    achd = stif2_parser.firstAct();
    while (achd != NULL)
    {
        eaif = stif2_parser.firstEat();
        while (eaif != NULL)
        {
            eaif->count = round(eaif->count / 2.0);
            eaif = stif2_parser.nextEat();
        }
        memcpy(acts_buffer[act_num], achd, sizeof(Action_Info_Header) + achd->eat_num * sizeof(EnvAction_Info));
        act_num++;	// increase act count
        achd = stif2_parser.nextAct();
    }
    /*************************************/

    unsigned char *buf_acpt = NULL;
    Action_Info_Header *buf_achd = NULL;
    unsigned char *buf_eapt = NULL;
    EnvAction_Info *buf_eaif = NULL;
    // compare each act from tmp_origsthd with acts from tmp_recvsthd
    int tmp_act_num = act_num; // act_num will be changed
    achd = stif1_parser.firstAct();
    while (achd != NULL)
    {
        int i;
        // traverse all acts in buffer
        for (i = 0; i < tmp_act_num; i++)
        {
            buf_acpt = (unsigned char *) acts_buffer[i];
            buf_achd = (Action_Info_Header *) buf_acpt;
            if (buf_achd->act == achd->act)
            {
                // compare each eat from tmp_origsthd with eats from tmp_recvsthd
                int tmp_eat_num = buf_achd->eat_num; // eat_num will be changed

                eaif = stif1_parser.firstEat();
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
                    if (j >= tmp_eat_num) // eat not found, it's a new eat in copy_stif1
                    {
                        // append it to current act buffer
                        memcpy(
                                    buf_acpt + sizeof(Action_Info_Header)
                                    + buf_achd->eat_num
                                    * sizeof(EnvAction_Info), eaif,
                                    sizeof(EnvAction_Info));
                        buf_achd->eat_num++;
                    }
                    eaif = stif1_parser.nextEat();
                }
                break;
            }
        }
        if (i >= tmp_act_num) // act not found, it's a new act in copy_stif1
        {
            // append it to act buffer
            memcpy(acts_buffer[act_num], achd,
                   sizeof(Action_Info_Header)
                   + achd->eat_num * sizeof(EnvAction_Info));
            act_num++;
        }
        achd = stif1_parser.nextAct();
    }
    // get total sthd_size
    unsigned int sthd_size = 0;
    sthd_size += sizeof(State_Info_Header);
    for (int i = 0; i < act_num; i++)
    {
        buf_achd = (Action_Info_Header *) acts_buffer[i];
        sthd_size += sizeof(Action_Info_Header)
                + buf_achd->eat_num * sizeof(EnvAction_Info);
    }
    State_Info_Header *sthd = (State_Info_Header *) malloc(sthd_size);
    // fill the header
    sthd->st = copy_stif2->st;
    sthd->act_num = act_num;
    sthd->count = round((copy_stif1->count + copy_stif2->count) / 2.0);
    sthd->payoff = copy_stif2->payoff;
    sthd->original_payoff = copy_stif2->original_payoff;
    sthd->size = sthd_size;
    // copy act info from buffer
    unsigned char *ptr = (unsigned char *) sthd;
    ptr += sizeof(State_Info_Header); // point to the first act
    unsigned int act_size = 0;
    for (int i = 0; i < act_num; i++)
    {
        buf_achd = (Action_Info_Header *) acts_buffer[i];
        act_size = sizeof(Action_Info_Header)
                + buf_achd->eat_num * sizeof(EnvAction_Info);
        memcpy(ptr, buf_achd, act_size);
        ptr += act_size;
    }

    // free memory
    free(stif1_buf);
    free(stif2_buf);
    for (int i = 0; i < nrows; i++)
    {
        free(acts_buffer[i]);
    }
    free(acts_buffer);

    return sthd;
}

void AvatarSpirit::recvMsg(const State_Info_Header *recstif)
{
    State_Info_Header *mystif = myagent->getStateInfo(recstif->st);

    if (mystif == NULL) // simply add
    {
        myagent->addStateInfo(recstif);
    }
    else if (recstif->count > mystif->count)   // more experienced, merge and accept
    {
        State_Info_Header *merged_stif = mergeStateInfo(mystif, recstif);
        myagent->updateStateInfo(merged_stif);
        free(merged_stif);  // free memory
    }
    else	// less experienced, do not accept
    {

    }

    free(mystif);
}

bool AvatarSpirit::timeToShare()
{
    bool re = false;
    if (_counter <= 0)
    {
        _counter = qrand() % share_freq * 2;	// [0, 2 * com_freq)  restart count
        re = true;
    }
    else
    {
        _counter--;
        re = false;
    }

    return re;
}

float AvatarSpirit::payoffByCuriosity(Agent::State st)
{
    float pf = 0.0;
    bool experienced = myagent->hasState(st);
    if (experienced)
    {
        State_Info_Header *stif = myagent->getStateInfo(st);
        pf = -accuracy * stif->count;
        if (pf <= -100 * accuracy) pf = -100 * accuracy;	// set a minimum
        qDebug() << "On Curiosity, Avatar" << id << ":" << st << "is experienced" << stif->count << "times";
        free(stif);
    }
    else
    {
        pf = 0.0;
    }

    return pf;
}
