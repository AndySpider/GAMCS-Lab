#ifndef AMOUNT_H
#define AMOUNT_H
#include <QGraphicsItem>
#include <QMutex>

class Amount
{
public:
    Amount(QGraphicsItem *h, float a);

    float amount();
    void increase(float n);
    void decrease(float n);

private:
    float _amount;
    QMutex mutex;
    QGraphicsItem *host;
};

#endif // AMOUNT_H
