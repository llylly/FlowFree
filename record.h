#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include <string>
#include <QObject>
#include <QDebug>

using namespace std;

class record : public QObject
{
    Q_OBJECT
public:
    explicit record(QObject *parent = 0);

public slots:
    void save();

public:
    int maxValue;
    int minStep[10][50], minTime[10][50], needStep[10][50];
};

#endif // RECORD_H
