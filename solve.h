#ifndef SOLVE_H
#define SOLVE_H

#include "state.h"
#include <QObject>
#include <cstring>

class Solve : public QObject
{
    Q_OBJECT
public:
    explicit Solve(QObject *parent = 0);

signals:

public:
    void setState(state*);
    void work();
    state* getResult();

private:
    state *init, *res;
    bool find;
    int N;
    int map[9][9];
    int bigMap[9][9][9];
    int direc[4][2];

    int quex[50], quey[50], vis[9][9];

    void DFS(int, int, int, int);
    int BFS();
    int netBFS(int);
};

#endif // SOLVE_H
