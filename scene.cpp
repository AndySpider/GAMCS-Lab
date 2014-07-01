#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QFile>
#include <QDomDocument>
#include <assert.h>
#include "avatarspirit.h"
#include "block.h"
#include "cheese.h"
#include "nail.h"
#include "bomb.h"
#include "mouse.h"
#include "cat.h"
#include "elephant.h"
#include "scene.h"
#include "configure.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent), cur_tool(T_NONE), teach_spirit(NULL), mouse_id(0), cat_id(0), elephant_id(0), timer_interval(100),
    num_blocks(0), num_cheeses(0), num_nails(0), num_bombs(0), num_mice(0), num_cats(0), num_elephants(0),
    channel(NULL), _width(0), _height(0)
{
    setItemIndexMethod(BspTreeIndex);   // NoIndex is slow!

    // build scene
    this->build();

    // initialize channel
    channel = new Channel();

    // start timer
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));
    timer->start(timer_interval);
}

Scene::~Scene()
{
    delete timer;
    delete channel;
}

void Scene::step()
{
    QList<Spirit *> dead_spirits;

    for (QList<Spirit *>::iterator it = spirits.begin(); it != spirits.end(); ++it)
    {
        (*it)->act();

        if ((*it)->life() <= 0)	// remove spirit if it's dead
        {
            dead_spirits.append(*it);   // record dead spirits
        }
    }

    // dispatch messages (before any spirit is removed)
    channel->dispatchMsg();

    // remove dead spirits
    for (QList<Spirit *>::iterator it = dead_spirits.begin(); it != dead_spirits.end(); ++it)
    {
        qDebug() << "dead spirit to be removed!";
        removeSpirit(*it);
    }

    // post actions: eg. the position are changed after all spirits have taken actions
    for (QList<Spirit *>::iterator it = spirits.begin(); it != spirits.end(); ++it)
    {
        (*it)->postAct();
    }

    update();	// update scene
    timer->start(timer_interval);
}

int Scene::load(const QString &file)
{
    // pause the scene when and after load scene
    pause();

    QFile infile(file);
    if (!infile.open(QFile::Text | QFile::ReadOnly))
    {
        qDebug() << "Error: cannot open file for loading scene" << file;
        return -1;
    }

    QDomDocument doc("Scene");
    QString errMsg;
    int errLine, errCol;
    if (!doc.setContent(&infile, false, &errMsg, &errLine, &errCol))
    {
        qDebug() << "Error: cannot set content of xml to the file" << file << errMsg <<
                    "at" << errLine << "," << errCol;
        infile.close();
        return -1;
    }
    infile.close();

    // show busy cursor
    QApplication::setOverrideCursor(Qt::BusyCursor);

    QDomElement root = doc.documentElement();
    int scene_width = root.attribute("width").toInt(NULL);
    int scene_height = root.attribute("height").toInt(NULL);

    this->build(scene_width, scene_height);		// clear scene and resize

    // scene meta-info
    QDomNode first_node = root.firstChild();
    if (!first_node.isElement())	// until find the first element
        first_node = first_node.nextSibling();

    QDomElement scene_info = first_node.toElement();
    Q_UNUSED(scene_info);

    // spirit
    QDomNode node = first_node.nextSibling();
    while (!node.isNull())
    {
        Spirit::SType type;
        int life = 0;
        QPoint grid_pos;
        bool is_marked = true;
        int id = -1;
        int share_range = 5;
        bool is_awake = false;
        int share_freq = 5;
        Agent::Mode lm = Agent::ONLINE;
        bool ok = false;
        bool is_avatar = false;

        if (node.isElement())
        {
            QDomElement spt = node.toElement();
            type = Spirit::stringToSType(spt.attribute("type"));

            // traverse all the child nodes
            QDomNodeList list = spt.childNodes();

            for (int i = 0; i < list.count(); i++)
            {
                QDomNode childnode = list.at(i);
                if (childnode.isElement())
                {
                    QDomElement ele = childnode.toElement();
                    if (ele.tagName() == "life")
                    {
                        life = ele.text().toFloat(&ok);
                        assert(ok);
                    }
                    else if (ele.tagName() == "xpos")
                    {
                        int xpos = ele.text().toInt(&ok, 10);
                        assert(ok);
                        grid_pos.setX(xpos);
                    }
                    else if (ele.tagName() == "ypos")
                    {
                        int ypos = ele.text().toInt(&ok, 10);
                        assert(ok);
                        grid_pos.setY(ypos);
                    }
                    else if (ele.tagName() == "isMarked")
                    {
                        if (ele.text() == "true")
                            is_marked = true;
                        else
                            is_marked = false;
                    }	// for avatars below
                    else if (ele.tagName() == "id")
                    {
                        id = ele.text().toInt(&ok, 10);
                        assert(ok);
                        is_avatar = true;
                    }
                    else if (ele.tagName() == "Share_Range")
                    {
                        share_range = ele.text().toInt(&ok, 10);
                        assert(ok);
                    }
                    else if (ele.tagName() == "isAwake")
                    {
                        if (ele.text() == "true")
                            is_awake = true;
                        else
                            is_awake = false;
                    }
                    else if (ele.tagName() == "Share_Freq")
                    {
                        share_freq = ele.text().toInt(&ok, 10);
                        assert(ok);
                    }
                    else if (ele.tagName() == "Learning_Mode")
                    {
                        if (ele.text() == "Explore")
                            lm = Agent::EXPLORE;
                        else
                            lm = Agent::ONLINE;
                    }
                }
            }
        }

        Spirit *nspt = newSpiritAt(type, grid_pos);
        if (nspt != NULL)
        {
            nspt->setLife(life);
            nspt->setMarked(is_marked);

            // for avatars
            if (is_avatar)
            {
                AvatarSpirit *avaspt = dynamic_cast<AvatarSpirit *>(nspt);
                assert(avaspt != NULL);

                avaspt->setId(id);
                avaspt->setShareRange(share_range);
                avaspt->setAwake(is_awake);
                avaspt->setShareFreq(share_freq);
                avaspt->setLearningMode(lm);
            }
        }

        node = node.nextSibling();
    }

    QApplication::restoreOverrideCursor();  // restore cursor
    return 0;
}

void Scene::save(const QString &file)
{
    // pause the scene when save
    bool timer_active  = timer->isActive();
    pause();

    QFile of(file);
    if(!of.open(QFile::Text | QFile::WriteOnly | QFile::Truncate))
    {
        qDebug() << "Error: can not open file for saving scene" << file;
        return;
    }

    // show busy cursor
    QApplication::setOverrideCursor(Qt::BusyCursor);

    QDomDocument doc("Scene");
    QDomElement root = doc.createElement("Scene");
    doc.appendChild(root);

    root.setAttribute("width", _width);
    root.setAttribute("height", _height);
    root.setAttribute("grid_size", GRID_SIZE);

    // save the meta-scene info
    QDomElement meta_info = doc.createElement("Scene_Infomation");
    // spirits statistics
    QDomElement spirits_stis = doc.createElement("Spirit_Statistics");
    spirits_stis.setAttribute("total_num", spirits.size());

    QList<SpiritInfo> spirits_infos = statistics();
    for (QList<SpiritInfo>::iterator iit = spirits_infos.begin();
         iit != spirits_infos.end(); ++iit)
    {
        QDomElement ele = doc.createElement(iit->name);
        QDomText text = doc.createTextNode(QString::number(iit->num));
        ele.appendChild(text);

        spirits_stis.appendChild(ele);
    }
    meta_info.appendChild(spirits_stis);
    // other scene-scale info come here
    root.appendChild(meta_info);

    // traverse all the spirits
    for (QList<Spirit *>::iterator sit = spirits.begin();
         sit != spirits.end(); ++sit)
    {
        QDomElement spirit = doc.createElement("Spirit");
        spirit.setAttribute("type", Spirit::stypeToString((*sit)->spiritType()));
        // life
        QDomElement life = doc.createElement("life");
        QDomText life_val = doc.createTextNode(QString::number((*sit)->life()));
        life.appendChild(life_val);
        spirit.appendChild(life);
        // grid position
        // xpos
        // ypos
        QDomElement xpos = doc.createElement("xpos");
        QDomText xpos_val = doc.createTextNode(QString::number((*sit)->gridPos().x()));
        xpos.appendChild(xpos_val);
        QDomElement ypos = doc.createElement("ypos");
        QDomText ypos_val = doc.createTextNode(QString::number((*sit)->gridPos().y()));
        ypos.appendChild(ypos_val);
        spirit.appendChild(xpos);
        spirit.appendChild(ypos);
        // marked?
        QDomElement ismarked = doc.createElement("isMarked");
        QDomText ismarked_val;
        if ((*sit)->isMarked())
            ismarked_val = doc.createTextNode("true");
        else
            ismarked_val = doc.createTextNode("false");
        ismarked.appendChild(ismarked_val);
        spirit.appendChild(ismarked);
        /* for avatars */
        if ((*sit)->spiritCategory() == Spirit::AVATARSPIRIT)
        {
            AvatarSpirit *avaspt = dynamic_cast<AvatarSpirit *>(*sit);
            assert(avaspt != NULL);
            // id
            QDomElement id = doc.createElement("id");
            QDomText id_val = doc.createTextNode(QString::number(avaspt->getId()));
            id.appendChild(id_val);
            spirit.appendChild(id);
            // share range
            QDomElement share_range = doc.createElement("Share_Range");
            QDomText range_val = doc.createTextNode(QString::number(avaspt->getShareRange()));
            share_range.appendChild(range_val);
            spirit.appendChild(share_range);
            // awake?
            QDomElement isawake = doc.createElement("isAwake");
            QDomText isawake_val;
            if (avaspt->isAwake())
                isawake_val = doc.createTextNode("true");
            else
                isawake_val = doc.createTextNode("false");
            isawake.appendChild(isawake_val);
            spirit.appendChild(isawake);
            // avgfreq
            QDomElement freq = doc.createElement("Share_Freq");
            QDomText freq_val = doc.createTextNode(QString::number(avaspt->getShareFreq()));
            freq.appendChild(freq_val);
            spirit.appendChild(freq);
            // learning mode
            QDomElement learning_mode = doc.createElement("Learning_Mode");
            QDomText mode_val;
            if (avaspt->getLearningMode() == Agent::EXPLORE)
                mode_val = doc.createTextNode("Explore");
            else
                mode_val = doc.createTextNode("Online");
            learning_mode.appendChild(mode_val);
            spirit.appendChild(learning_mode);
        }

        root.appendChild(spirit);
    }

    QTextStream out(&of);
    doc.save(out, 4);
    of.close();

    QApplication::restoreOverrideCursor();
    // restore timer
    if (timer_active == true)
        resume();
}

void Scene::build(int width, int height)
{
    // clean spirits
    spirits.clear(); // clear spirits
    num_blocks = 0;
    num_cats = 0;
    num_cheeses = 0;
    num_mice = 0;
    num_nails = 0;
    num_bombs = 0;
    num_elephants = 0;

    elephant_id = 0;
    mouse_id = 0;
    cat_id = 0;

    // set scene size
    this->clear();  // clear items

    if (width == -1 || height == -1)	// load size from config file
    {
        bool ok;
        int val = g_config.getValue("Scene/Size/Width").toInt(&ok);
        if (ok && val > 0)
        {
            _width = val;
        }
        else
        {
            _width = 512;
            g_config.setValue("Scene/Size/Width", 512);
        }

        val = g_config.getValue("Scene/Size/Height").toInt(&ok);
        if (ok && val > 0)
        {
            _height = val;
        }
        else
        {
            _height = 256;
            g_config.setValue("Scene/Size/Height", 256);
        }
    }
    else	// temporarily set size
    {
        _width = width;
        _height = height;
    }

    this->setSceneRect(-40, -40, _width * GRID_SIZE + 80, _height * GRID_SIZE + 80);

    // draw boundary
    drawAxis();
    drawLimitLine();
}

void Scene::pause()
{
    timer->stop();
    emit paused();
}

void Scene::resume()
{
    timer->start(timer_interval);
    emit resumed();
}

void Scene::drawAxis()
{
    QPointF origin = QPointF(-20, -20);
    QPointF xend = QPointF( 5 * GRID_SIZE, -20);
    QPointF yend = QPointF(-20, 5 * GRID_SIZE);
    QLineF xaxis = QLineF(origin, xend);
    QLineF yaxis = QLineF(origin, yend);

    this->addLine(xaxis);
    this->addLine(yaxis);
    QGraphicsSimpleTextItem *olabel = this->addSimpleText("O");
    olabel->setPos(-35, -35);
    QGraphicsSimpleTextItem *xlabel = this->addSimpleText("X");
    xlabel->setPos(5 * GRID_SIZE, -20);
    QGraphicsSimpleTextItem *ylabel = this->addSimpleText("Y");
    ylabel->setPos(-20, 5 * GRID_SIZE);
}

void Scene::drawLimitLine()
{
    QPointF ur = QPointF(0, 0);
    QPointF ul = QPointF(GRID_SIZE * _width, 0);
    QPointF dr = QPointF(0, GRID_SIZE * _height);
    QPointF dl = QPointF(GRID_SIZE * _width, GRID_SIZE * _height);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setBrush(Qt::black);
    pen.setWidth(2);
    pen.setJoinStyle(Qt::RoundJoin);

    QLineF uline = QLineF(ul, ur);
    QLineF rline = QLineF(ur, dr);
    QLineF dline = QLineF(dr, dl);
    QLineF lline = QLineF(dl, ul);
    this->addLine(uline, pen);
    this->addLine(rline, pen);
    this->addLine(dline, pen);
    this->addLine(lline, pen);

    QGraphicsSimpleTextItem *text =  this->addSimpleText("Limit Line");
    text->setPos(_width * GRID_SIZE / 2.0, -20);
}

void Scene::setCurTool(Tool t)
{
    cur_tool = t;
}

void Scene::speedUp()
{
    int level = 0;

    switch (timer_interval)
    {
    case 1000:
        timer_interval = 500;
        break;
    case 500:
        timer_interval = 200;
        break;
    case 200:
        timer_interval = 100;
        break;
    case 100:
        timer_interval = 50;
        break;
    case 50:
        timer_interval = 20;
        break;
    case 20:
        timer_interval = 10;
        break;
    case 10:
        timer_interval = 5;
        break;
    case 5:
        timer_interval = 1;
        break;
    case 1:
        qDebug() << "It's the maximum speed!";
        level = 1;
        break;
    default:
        qWarning() << "!!! The timer interval is out of control!";
    }

    if (timer->isActive())
        timer->start(timer_interval);

    emit currentSpeedLevel(level);
}

void Scene::speedDown()
{
    int level = 0;

    switch (timer_interval)
    {
    case 1:
        timer_interval = 5;
        break;
    case 5:
        timer_interval = 10;
        break;
    case 10:
        timer_interval = 20;
        break;
    case 20:
        timer_interval = 50;
        break;
    case 50:
        timer_interval = 100;
        break;
    case 100:
        timer_interval = 200;
        break;
    case 200:
        timer_interval = 500;
        break;
    case 500:
        timer_interval = 1000;
        break;
    case 1000:
        qDebug() << "It's the minimum speed!";
        level = -1;
        break;
    default:
        qWarning() << "!!! The timer interval is out of control!";
    }

    if (timer->isActive())
        timer->start(timer_interval);

    emit currentSpeedLevel(level);
}

bool Scene::empty()
{
    return spirits.empty();
}

int Scene::width()
{
    return _width;
}

int Scene::height()
{
    return _height;
}

void Scene::genRandSpirit(int num, const QList<Spirit::SType> &types)
{
    bool timer_status = timer->isActive();  // save status
    pause();    // pause the scene

    int t_num = types.size();
    while (num > 0)
    {
        // find a random grid position inside SCENE
        int gx = qrand() % _width;
        int gy = qrand() % _height;
        Spirit::SType type = types.at(qrand() % t_num);

        addSpiritAt(type, QPoint(gx, gy));
        num--;
    }

    if (timer_status == true)   // restore status
        resume();
}

void Scene::setTeachSpirit(AvatarSpirit *spt)
{
    clearTeachSpirit();	// clear old one first

    Spirit *tmp = dynamic_cast<Spirit *>(spt);
    if (spirits.indexOf(tmp) == -1)	// not found
    {
        qDebug() << "The teach spirit can not be found!";
        return;
    }

    teach_spirit = spt;
    spirits.removeOne(tmp);	// remove it from spirits
    teach_spirit->setLearningMode(Agent::EXPLORE);	// set to EXPLORE mode for efficience
    teach_spirit->setTeach(true);
}

void Scene::clearTeachSpirit()
{
    if (teach_spirit == NULL)
        return;

    teach_spirit->setTeach(false);
    teach_spirit->setLearningMode(Agent::ONLINE);	// set back to the default Online mode
    spirits.append(dynamic_cast<Spirit *>(teach_spirit));	// restore it to spirits
    teach_spirit = NULL;
}

QList<SpiritInfo> Scene::statistics()
{
    QList<SpiritInfo> infos;

    SpiritInfo blocks_info;
    SpiritInfo cheeses_info;
    SpiritInfo nails_info;
    SpiritInfo bombs_info;
    SpiritInfo mice_info;
    SpiritInfo cats_info;
    SpiritInfo elephants_info;

    blocks_info.name = "Block";
    blocks_info.num = num_blocks;
    infos.append(blocks_info);

    cheeses_info.name = "Cheese";
    cheeses_info.num = num_cheeses;
    infos.append(cheeses_info);

    nails_info.name = "Nail";
    nails_info.num = num_nails;
    infos.append(nails_info);

    bombs_info.name = "Bomb";
    bombs_info.num = num_bombs;
    infos.append(bombs_info);

    mice_info.name = "Mouse";
    mice_info.num = num_mice;
    infos.append(mice_info);

    cats_info.name = "Cat";
    cats_info.num = num_cats;
    infos.append(cats_info);

    elephants_info.name = "Elephant";
    elephants_info.num = num_elephants;
    infos.append(elephants_info);

    return infos;
}

void Scene::useToolAt(const QPoint &grid_pos)
{
    if (cur_tool == T_ERASER)
    {
        removeSpiritAt(grid_pos);
    }
    else if (cur_tool == T_BLOCK)
    {
        addSpiritAt(Spirit::BLOCK, grid_pos);
    }
    else if (cur_tool == T_CHEESE)
    {
        addSpiritAt(Spirit::CHEESE, grid_pos);
    }
    else if (cur_tool == T_NAIL)
    {
        addSpiritAt(Spirit::NAIL, grid_pos);
    }
    else if (cur_tool == T_MOUSE)
    {
        addSpiritAt(Spirit::MOUSE, grid_pos);
    }
    else if (cur_tool == T_CAT)
    {
        addSpiritAt(Spirit::CAT, grid_pos);
    }
    else if (cur_tool == T_ELEPHANT)
    {
        addSpiritAt(Spirit::ELEPHANT, grid_pos);
    }
    else if (cur_tool == T_BOMB)
    {
        addSpiritAt(Spirit::BOMB, grid_pos);
    }
    else if (cur_tool == T_NONE)
    {
        //
    }
}

void Scene::removeSpiritAt(const QPoint &grid_pos)
{
    Spirit *spt = this->getSpiritAt(grid_pos.x(), grid_pos.y());

    if (spt == NULL) // not exist
    {
        return;
    }
    else    // found a tool
    {
        removeSpirit(spt);
    }
}

void Scene::addSpiritAt(Spirit::SType type, const QPoint &grid_pos)
{
    // check if there's some type already at pos
    Spirit *old_spirit = getSpiritAt(grid_pos.x(), grid_pos.y());
    if (old_spirit == NULL) // no spirit at pos, add directly
    {
        newSpiritAt(type, grid_pos);
    }
    else 	// some type is at pos
    {
        Spirit::SType old_spirit_type = old_spirit->spiritType();
        Spirit::SCategory old_spirit_cate = old_spirit->spiritCategory();
        if (old_spirit_type == Spirit::BLOCK)
        {
            qDebug() << "can not add types on a Block, erase it first!";
        }
        else if (old_spirit_cate == Spirit::STATICSPIRIT)	// static spirits
        {
            if (type == Spirit::BLOCK)	// erase old spirit first
            {
                removeSpirit(old_spirit);
                newSpiritAt(Spirit::BLOCK, grid_pos);
            }
            else if (type == old_spirit_type)   // supply
            {
                Spirit *spt = newSpiritAt(type, grid_pos);
                if (spt != NULL)
                    spt->healed(old_spirit->life());
                removeSpirit(old_spirit);
            }
            else if (type == Spirit::MOUSE || type == Spirit::CAT || type == Spirit::ELEPHANT)  // add avatar directly
            {
                newSpiritAt(type, grid_pos);
            }
            else    // other non-avatar spirits, erase old and add new
            {
                removeSpirit(old_spirit);
                newSpiritAt(type, grid_pos);
            }
        }
        else if (old_spirit_cate == Spirit::AVATARSPIRIT)	// avatar spirits
        {
            if (type == Spirit::BLOCK)	// move avatar
            {
                bool found;
                QPoint npos = findAvatarNewPos(grid_pos, &found);
                if (found)
                {
                    old_spirit->setPos(npos * GRID_SIZE);   // move to the new pos
                }
                else
                {
                    removeSpirit(old_spirit);
                }

                newSpiritAt(Spirit::BLOCK, grid_pos);
            }
            else	// add directly
            {
                newSpiritAt(type, grid_pos);
            }
        }
        // other types come here
    }
}

QPoint Scene::findAvatarNewPos(const QPoint &grid_pos, bool *found)
{
    Spirit *spt = NULL;
    *found = false;
    // check if there's space above
    spt = this->getSpiritAt(grid_pos.x(), grid_pos.y() - 1);
    if (grid_pos.y() != 0 &&
            (spt == NULL || spt->spiritType() != Spirit::BLOCK))	// not cross limit line and (empty or not block)
    {
        *found = true;
        return QPoint(grid_pos.x(), grid_pos.y() - 1);
    }

    // check right
    spt = this->getSpiritAt(grid_pos.x() + 1, grid_pos.y());
    if (grid_pos.x() != _width -1 &&
            (spt == NULL || spt->spiritType() != Spirit::BLOCK))
    {
        *found = true;
        return QPoint(grid_pos.x() + 1, grid_pos.y());
    }

    // check below
    spt = this->getSpiritAt(grid_pos.x(), grid_pos.y() + 1);
    if (grid_pos.y() != _height - 1 &&
            (spt == NULL || spt->spiritType() != Spirit::BLOCK))
    {
        *found = true;
        return QPoint(grid_pos.x(), grid_pos.y() + 1);
    }

    // check left
    spt = this->getSpiritAt(grid_pos.x() - 1, grid_pos.y());
    if (grid_pos.x() != 0 &&
            (spt == NULL || spt->spiritType() != Spirit::BLOCK))
    {
        *found = true;
        return QPoint(grid_pos.x() - 1, grid_pos.y());
    }

    // not found
    *found = false;
    return QPoint();
}

Spirit *Scene::newSpiritAt(Spirit::SType type, const QPoint &grid_pos)
{
    // can not add spirit out of limit line
    if (outOfLimitLine(grid_pos))
    {
        qDebug() << "--- Scene: can not create spirit out of limit line";
        return NULL;
    }

    Spirit *spt = NULL;
    int *num = NULL;

    if (type == Spirit::BLOCK)
    {
        spt = new Block();
        spt->setZValue(Spirit::BLOCK_Z);
        num = &num_blocks;
    }
    else if (type == Spirit::CHEESE)
    {
        spt = new Cheese();
        spt->setZValue(Spirit::CHEESE_Z);
        num = &num_cheeses;
    }
    else if (type == Spirit::NAIL)
    {
        spt = new Nail();
        spt->setZValue(Spirit::NAIL_Z);
        num = &num_nails;
    }
    else if (type == Spirit::BOMB)
    {
        spt = new Bomb();
        spt->setZValue(Spirit::BOMB_Z);
        num = &num_bombs;
    }
    else if (type == Spirit::MOUSE)
    {
        spt = new Mouse(mouse_id++);
        num = &num_mice;
        spt->setZValue(Spirit::MOUSE_Z);
    }
    else if (type == Spirit::CAT)
    {
        spt = new Cat(cat_id);
        num = &num_cats;
        spt->setZValue(Spirit::CAT_Z);
    }
    else if (type == Spirit::ELEPHANT)
    {
        spt = new Elephant(elephant_id);
        num = &num_elephants;
        spt->setZValue(Spirit::ELEPHANT_Z);
    }

    spt->setPos(grid_pos.x() * GRID_SIZE, grid_pos.y() * GRID_SIZE);
    this->addItem(spt);
    int i = spt->initialize();		// initialize spirit
    if (i != 0)	// init failed
    {
        qDebug() << "Scene: failed to new Spirit at" << grid_pos;
        this->removeItem(spt);
        return NULL;
    }

    // avatars use channel to share memory
    if (spt->spiritCategory() == Spirit::AVATARSPIRIT)
    {
        AvatarSpirit *ava = dynamic_cast<AvatarSpirit *>(spt);
        assert(ava != NULL);
        ava->setChannel(channel);    // set channel
    }

    spirits.append(spt);
    (*num)++;
    emit spiritsNumChanged(spirits.size());

    return spt;
}

void Scene::removeSpirit(Spirit *spt)
{
    Spirit::SType type = spt->spiritType();
    int *num = NULL;

    if (type == Spirit::BLOCK)
    {
        num = &num_blocks;
    }
    else if (type == Spirit::CHEESE)
    {
        num = &num_cheeses;
    }
    else if (type == Spirit::NAIL)
    {
        num = &num_nails;
    }
    else if (type == Spirit::BOMB)
    {
        num = &num_bombs;
    }
    else if (type == Spirit::MOUSE)
    {
        num = &num_mice;
    }
    else if (type == Spirit::CAT)
    {
        num = &num_cats;
    }
    else if (type == Spirit::ELEPHANT)
    {
        num = &num_elephants;
    }

    this->removeItem(spt);
    spirits.removeOne(spt);
    (*num)--;
    emit spiritsNumChanged(spirits.size());
}

/**
 * @brief Snap to grid.
 * @param pos
 * @return
 */
QPoint Scene::calGridPos(const QPointF &pos)
{
    int x = (int) pos.x();
    int y = (int) pos.y();


    x /= GRID_SIZE;
    y /= GRID_SIZE;

    return QPoint(x, y);
}

Spirit *Scene::getSpiritAt(int grid_x, int grid_y)
{
    qreal x = (grid_x + 0.5) * GRID_SIZE;
    qreal y = (grid_y + 0.5) * GRID_SIZE;

    QGraphicsItem *item = this->itemAt(x, y, QTransform());	 // the topmost item
    if (item == NULL)
    {
        return NULL;
    }
    else
    {
        Spirit *spt = qgraphicsitem_cast<Spirit *>(item);	// 0 if it's not a Spirit
        return spt;
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->useToolAt(calGridPos(event->scenePos()));
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() == Qt::CTRL)    // ctrl + move
    {
        this->useToolAt(calGridPos(event->scenePos()));
    }

    QGraphicsScene::mouseMoveEvent(event);
}

bool Scene::outOfLimitLine(const QPoint &grid_pos)
{
    if (grid_pos.x() < 0
            || grid_pos.x() > _width - 1
            || grid_pos.y() < 0
            || grid_pos.y() > _height - 1)
        return true;
    else
        return false;
}

void Scene::keyPressEvent(QKeyEvent *keyEvent)
{
    if (teach_spirit != NULL)
    {
        if (keyEvent->key() == Qt::Key_Up)
        {
            teach_spirit->teach(1);		// teach must be called before action performed
            teach_spirit->moveUp();
        }
        else if (keyEvent->key() == Qt::Key_Down)
        {
            teach_spirit->teach(2);		// the action numbers must be the same with that used in performAction()
            teach_spirit->moveDown();
        }
        else if (keyEvent->key() == Qt::Key_Left)
        {
            teach_spirit->teach(3);
            teach_spirit->moveLeft();
        }
        else if (keyEvent->key() == Qt::Key_Right)
        {
            teach_spirit->teach(4);
            teach_spirit->moveRight();
        }
        else if (keyEvent->key() == Qt::Key_Space)
        {
            teach_spirit->teach(5);
        }

        // now update position
        teach_spirit->postAct();
    }
    else
    {
        QGraphicsScene::keyPressEvent(keyEvent);
    }
}
