#include <QDebug>
#include <QGraphicsScene>
#include "amount.h"

Amount::Amount(QGraphicsItem *h, float a) : host(h), _amount(a)
{
}

float Amount::amount()
{
    return _amount;
}

void Amount::increase(float n)
{
    mutex.lock();
    _amount += n;
    mutex.unlock();
}

void Amount::decrease(float n)
{
    mutex.lock();
    _amount -= n;
    if (_amount <= 0)	// used up, remove the host
    {
        qDebug() << "it's used up!";
        QGraphicsScene *scene = host->scene();
        scene->removeItem(host);
    }
    mutex.unlock();
}

