#include "solve.h"

Solve::Solve(QObject *parent) : QObject(parent)
{
    find = 0;
    memset(map, 0, sizeof(map));
    direc[0][0] = -1, direc[0][1] =  0,
    direc[1][0] =  1, direc[1][1] =  0,
    direc[2][0] =  0, direc[2][1] = -1,
    direc[3][0] =  0, direc[3][1] =  1;
}

void Solve::setState(state* S) {
    init = S;
    N = init->size;
    res = new state(init);
    for (int i=1; i<=N; i++)
        for (int j=1; j<=N; j++)
            res->netColor[i][j] = 0;
    for (int i=1; i<=res->totFlows; i++)
        res->flowTrack[i].clear();
}

void Solve::work() {
    DFS(0, 0, 0, -1);
    res->nowFlows = res->totFlows;
}

state* Solve::getResult() {
    return res;
}

void Solve::DFS(int No, int X, int Y, int nowF) {
    if ((No == 0) ||
        ((res->flowTrack[No].size()>1) &&
         (res->netDest[res->flowTrack[No].last().x()][res->flowTrack[No].last().y()]))) {
        if (nowF+1 == res->totFlows) {
            bool covered = 1;
            for (int i=1; i<=N; i++)
                for (int j=1; j<=N; j++)
                    if (res->netColor[i][j] == 0)
                        covered = 0;
            if (covered) find = 1;
            return;
        }
        int nexNo = BFS();
        if (nexNo == -1) return; else {
            res->netColor[res->flowDest[nexNo][0].x()][res->flowDest[nexNo][0].y()] = nexNo;
            res->flowTrack[nexNo].push_back(QPoint(res->flowDest[nexNo][0].x(), res->flowDest[nexNo][0].y()));
            DFS(nexNo, res->flowDest[nexNo][0].x(), res->flowDest[nexNo][0].y(), nowF+1);
            if (find) return;
            res->netColor[res->flowDest[nexNo][0].x()][res->flowDest[nexNo][0].y()] = 0;
            res->flowTrack[nexNo].pop_back();
        }
        return;
    }
    netBFS(No);
    int tot = 0, step[5], setX[5], setY[5];
    for (int i=0; i<4; i++)
        if ((X+direc[i][0]>=1) && (X+direc[i][0]<=N) &&
            (Y+direc[i][1]>=1) && (Y+direc[i][1]<=N) &&
            (res->netColor[X+direc[i][0]][Y+direc[i][1]] == 0) &&
            ((res->netDest[X+direc[i][0]][Y+direc[i][1]] == 0) ||
             (res->netDest[X+direc[i][0]][Y+direc[i][1]] == No))) {
            ++tot,
            step[tot] = map[X+direc[i][0]][Y+direc[i][1]],
            setX[tot] = X+direc[i][0],
            setY[tot] = Y+direc[i][1];
        }
    for (int i=1; i<=tot-1; i++)
        for (int j=tot; j>=i+1; j--)
            if (step[j]<step[j-1]) {
                int t;
                t=step[j], step[j]=step[j-1], step[j-1]=t;
                t=setX[j], setX[j]=setX[j-1], setX[j-1]=t;
                t=setY[j], setY[j]=setY[j-1], setY[j-1]=t;
            }
    for (int i=1; i<=tot; i++) {
        res->netColor[setX[i]][setY[i]] = No;
        res->flowTrack[No].push_back(QPoint(setX[i],setY[i]));
        DFS(No, setX[i], setY[i], nowF);
        if (find) return;
        res->flowTrack[No].pop_back();
        res->netColor[setX[i]][setY[i]] = 0;
    }
}

int Solve::netBFS(int No) {
    int l = 1, r = 1;

    for (int ii=1; ii<=N; ii++)
        for (int jj=1; jj<=N; jj++)
            vis[ii][jj] = 0, map[ii][jj] = 10000000;
    vis[res->flowDest[No][1].x()][res->flowDest[No][1].y()] = 1,
    quex[1] = res->flowDest[No][1].x(), quey[1] = res->flowDest[No][1].y();
    map[quex[1]][quey[1]] = 0;
    while (l<=r) {
        for (int ii=0; ii<4; ii++)
            if ((quex[l]+direc[ii][0]>=1) && (quex[l]+direc[ii][0]<=N) &&
                (quey[l]+direc[ii][1]>=1) && (quey[l]+direc[ii][1]<=N))
                if (!vis[quex[l]+direc[ii][0]][quey[l]+direc[ii][1]])
                    if ((res->netColor[quex[l]+direc[ii][0]][quey[l]+direc[ii][1]] == 0) &&
                        ((res->netDest[quex[l]+direc[ii][0]][quey[l]+direc[ii][1]] == 0)
                         || (res->netDest[quex[l]+direc[ii][0]][quey[l]+direc[ii][1]] == No))) {
                            ++r,
                            quex[r] = quex[l]+direc[ii][0],
                            quey[r] = quey[l]+direc[ii][1],
                            map[quex[r]][quey[r]] = map[quex[l]][quey[l]] + 1,
                            vis[quex[l]+direc[ii][0]][quey[l]+direc[ii][1]] = 1;
                        }
        l++;
    }
}

int Solve::BFS() {
    int l = 1, r = 0;
    for (int i=1; i<=res->totFlows; i++)
        for (int j=1; j<=N; j++)
            for (int k=1; k<=N; k++)
                bigMap[i][j][k] = 10000000;

    for (int i=1; i<=res->totFlows; i++)
        if (res->flowTrack[i].isEmpty()) {
            int (*nowMap)[9] = bigMap[i], nowNo = i;
            for (int ii=1; ii<=N; ii++)
                for (int jj=1; jj<=N; jj++)
                    vis[ii][jj] = 0;
            l = 1, r = 1,
            vis[res->flowDest[nowNo][0].x()][res->flowDest[nowNo][0].y()] = 1,
            quex[1] = res->flowDest[nowNo][0].x(), quey[1] = res->flowDest[nowNo][0].y();
            nowMap[quex[1]][quey[1]] = 0;
            while (l<=r) {
                for (int ii=0; ii<4; ii++)
                    if ((quex[l]+direc[ii][0]>=1) && (quex[l]+direc[ii][0]<=N) &&
                        (quey[l]+direc[ii][1]>=1) && (quey[l]+direc[ii][1]<=N))
                        if (!vis[quex[l]+direc[ii][0]][quey[l]+direc[ii][1]])
                            if ((res->netColor[quex[l]+direc[ii][0]][quey[l]+direc[ii][1]] == 0) &&
                                ((res->netDest[quex[l]+direc[ii][0]][quey[l]+direc[ii][1]] == 0)
                                 || (res->netDest[quex[l]+direc[ii][0]][quey[l]+direc[ii][1]] == nowNo))) {
                                ++r,
                                quex[r] = quex[l]+direc[ii][0],
                                quey[r] = quey[l]+direc[ii][1],
                                nowMap[quex[r]][quey[r]] = nowMap[quex[l]][quey[l]] + 1,
                                vis[quex[l]+direc[ii][0]][quey[l]+direc[ii][1]] = 1;
                            }
                l++;
            }
            if (!vis[res->flowDest[nowNo][1].x()][res->flowDest[nowNo][1].y()])
                bigMap[nowNo][res->flowDest[nowNo][1].x()][res->flowDest[nowNo][1].y()] = 100000;
        }

    int ans = -1, minS = 10000;
    bool able = 1;
    for (int i=1; i<=res->totFlows; i++)
        if (res->flowTrack[i].isEmpty()) {
            if (bigMap[i][res->flowDest[i][1].x()][res->flowDest[i][1].y()]<minS) {
                minS = bigMap[i][res->flowDest[i][1].x()][res->flowDest[i][1].y()];
                ans = i;
            }
            if (bigMap[i][res->flowDest[i][1].x()][res->flowDest[i][1].y()]>=10000)
                able = 0;
        }
    if (!able) ans = -1;
    return ans;
}
