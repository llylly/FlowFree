#include "backalime.h"

QuadGroup::QuadGroup() {
    l = rand() % 200 + 200;
    d = rand() % 4 + 1;
    c = rand() % 8 + 1;
    switch (d) {
        case 1:{
            x = -l-20;
            y = rand() % MHeight;
            break;
        }
        case 2:{
            x = MWidth+20;
            y = rand() % MHeight;
            break;
        }
        case 3:{
            x = rand() % MWidth;
            y = -l-20;
            break;
        }
        case 4:{
            x = rand() % MWidth;
            y = MHeight+20;
            break;
        }
    }
}

BackAlime::BackAlime(QObject *parent) : QObject(parent)
{
    vet.clear();
    for (int i=1; i<=need; i++)
        vet.append(QuadGroup());
    angle = 0;
    times = 0;
}

void BackAlime::refresh() {
    int speed = 1;
    //++times;
    //qDebug()<<"refreshed " << times;
    angle += 2;
    //if (angle >= 360) angle -= 360;

    QList<QuadGroup>::iterator p = vet.begin();
    while (p != vet.end()) {
        switch (p->d) {
            case 1: p->x+=speed; break;
            case 2: p->x-=speed; break;
            case 3: p->y+=speed; break;
            case 4: p->y-=speed; break;
        }
        bool legal = 1;
        switch (p->d) {
            case 1: if (p->x >= MWidth + 20) legal = 0; break;
            case 2: if (p->x <= -p->l - 20) legal = 0; break;
            case 3: if (p->y >= MHeight + 20) legal = 0; break;
            case 4: if (p->y <= -p->l - 20) legal = 0; break;
        }
        if (!legal)
            p = vet.erase(p);
        else
            p++;
    }
    while (vet.size() < need)
        vet.append(QuadGroup());
}
