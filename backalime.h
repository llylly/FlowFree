#ifndef BACKALIME_H
#define BACKALIME_H

#include <QObject>
#include <QList>
#include <QDebug>

const int MWidth = 600, MHeight = 700, need = 5;

struct QuadGroup {
    int x,y,l,d,c;
    QuadGroup();
};

class BackAlime : public QObject
{
    Q_OBJECT
public:
    explicit BackAlime(QObject *parent = 0);

public slots:
    void refresh();

public:
    QList<QuadGroup> vet;
    int angle, times;

signals:

public slots:

};

#endif // BACKALIME_H
