#ifndef STATE_H
#define STATE_H

#include "record.h"

#include <QObject>
#include <QColor>
#include <QPoint>
#include <QVector>
#include <QDebug>

#include <iostream>
#include <fstream>

using namespace std;

extern record *Record;

class state: QObject
{
    Q_OBJECT

public:
    state(state*);
    state(int, int);
    void refresh(QVector<QPoint>);
    bool checkSuccess(int);

    int size, levelNo, totFlows, minStep, minTime;
    int nowFlows, nowStep;
    int nowColor;
    QVector<QPoint> flowTrack[9];
    QPoint flowDest[9][2];
    int netColor[8][8];
    int netDest[8][8];

    int maxValue;

    static const QColor colorSet[9];
};


#endif // STATE_H
