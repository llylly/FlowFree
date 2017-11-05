#include "state.h"

const QColor state::colorSet[9] = {QColor(0, 0, 0), QColor(255, 0, 0), QColor(0, 0, 255), QColor(0, 170, 0),
                                   QColor(255, 255, 0), QColor(255, 140, 0), QColor(0, 255, 255), QColor(255, 0, 255), QColor(213, 95 ,0)};

state::state(int a, int b)
{
    size = a;
    levelNo = b;

    string path = "map\\";
    path.push_back('0' + a);
    path += "\\";
    if (b>=10) path.push_back('0' + (b / 10));
    path.push_back('0' + (b % 10));
    path += ".txt";

    ifstream fin(path.c_str());
    fin >> totFlows;
    for (int i=1; i<=totFlows; i++) {
        int x1, y1, x2, y2;
        fin>>x1>>y1>>x2>>y2;
        flowDest[i][0].setX(x1);
        flowDest[i][0].setY(y1);
        flowDest[i][1].setX(x2);
        flowDest[i][1].setY(y2);
    }

    maxValue = Record->maxValue;
    minStep = Record->minStep[a][b], minTime = Record->minTime[a][b];

    nowFlows = nowStep = nowColor = 0;
    for (int i=1; i<totFlows; i++)
        flowTrack[i].clear();

    for (int i=1; i<=size; i++)
        for (int j=1; j<=size; j++)
            netColor[i][j] = 0, netDest[i][j] = 0;
    for (int i=1; i<=totFlows; i++) {
        netDest[flowDest[i][0].x()][flowDest[i][0].y()] = i;
        netDest[flowDest[i][1].x()][flowDest[i][1].y()] = i;
    }

    fin.close();
}

state::state(state *old) {
    size = old->size, totFlows = old->totFlows, minStep = old->minStep,
    minTime = old->minTime, nowFlows = old->nowFlows,
    nowStep = old->nowStep, nowColor = old->nowColor,
    levelNo = old->levelNo, maxValue = old->maxValue;
    for (int i=0; i<9; i++)
        flowTrack[i] = old->flowTrack[i];
    for (int i=0; i<9; i++)
        for (int j=0; j<2; j++)
            flowDest[i][j] = old->flowDest[i][j];
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            netColor[i][j] = old->netColor[i][j],
            netDest[i][j] = old->netDest[i][j];
}

void state::refresh(QVector<QPoint> path) {
    bool res = 0;

    int color = netColor[path[0].x()][path[0].y()] | netDest[path[0].x()][path[0].y()];
    int posPath = -1, posFlow = -1;
    for (int i=0; i<path.size(); i++)
        for (int j=0; j<flowTrack[color].size(); j++)
            if ((path[i] == flowTrack[color][j]) &&
                ((posFlow == -1) || (j<posFlow))) {
                posPath = i, posFlow = j;
            }
    if (netDest[path[0].x()][path[0].y()])
        res = 1, posPath = -1;
    //找到插入的位置

    if ((posFlow != -1) || (res))
        while ((flowTrack[color].size()-1 > posFlow) ||
               ((res) && (!flowTrack[color].isEmpty()))) {
            QPoint todel = flowTrack[color].last();
            netColor[todel.x()][todel.y()] = 0;
            flowTrack[color].pop_back();
        }
    //刪掉插入位置之後(包括)的元素

    for (int i=posPath+1; i<path.size(); i++) {
        QPoint toins = path[i];
        if ((netColor[toins.x()][toins.y()]) && (netColor[toins.x()][toins.y()] != color)) {
            int todelColor = netColor[toins.x()][toins.y()];
            QPoint lastdel = QPoint(0, 0);
            do {
                lastdel = flowTrack[todelColor].last();
                netColor[lastdel.x()][lastdel.y()] = 0;
                flowTrack[todelColor].pop_back();
            } while (lastdel != toins);
            if (flowTrack[todelColor].size() == 1) {
                lastdel = flowTrack[todelColor].last();
                netColor[lastdel.x()][lastdel.y()] = 0;
                flowTrack[todelColor].pop_back();
            }
        }
        flowTrack[color].push_back(toins);
        netColor[toins.x()][toins.y()] = color;
    }
    //延長路徑,同時處理截斷其他顏色路徑的情況

    int headfin = -1;
    for (int i=1; i<flowTrack[color].size(); i++)
        if (netDest[flowTrack[color][i].x()][flowTrack[color][i].y()])
            headfin = i;
    while ((headfin != -1) && (flowTrack[color].size() - 1 > headfin)) {
        QPoint todel = flowTrack[color].last();
        netColor[todel.x()][todel.y()] = 0;
        flowTrack[color].pop_back();
    }
    //刪除兩頭後的多餘路徑

    if (flowTrack[color].size() == 1) {
        netColor[flowTrack[color][0].x()][flowTrack[color][0].y()] = 0;
        flowTrack[color].pop_back();
    }
    //刪除長度為1的路徑

    nowFlows = 0;
    for (int i=1; i<=totFlows; i++)
        if (!flowTrack[i].isEmpty()) {
            int t = flowTrack[i].size()-1;
            if ((flowTrack[i][0] == flowDest[i][0]) || (flowTrack[i][0] == flowDest[i][1]))
                if ((flowTrack[i][t] == flowDest[i][0]) || (flowTrack[i][t] == flowDest[i][1]))
                    nowFlows++;
        }
    //紀錄當前流數
    /*
    for (int i=1; i<=totFlows; i++) {
        netColor[flowDest[i][0].x()][flowDest[i][0].y()] = i;
        netColor[flowDest[i][1].x()][flowDest[i][1].y()] = i;
    }
    //恢復網格的顏色
    */
}

bool state::checkSuccess(int nowTime) {
    if (nowFlows < totFlows) return 0;
    for (int i=1; i<=size; i++)
        for (int j=1; j<=size; j++)
            if (netColor[i][j] == 0)
                return 0;

    bool best = 0;
    if (nowStep < minStep) {
        best = 1;
        minStep = nowStep;
    }
    if ((nowStep == minStep) && (nowTime < minTime)) {
        best = 1;
        minTime = nowTime;
    }
    if (best) {
        Record->minStep[size][levelNo] = nowStep,
        Record->minTime[size][levelNo] = nowTime;
        minStep = nowStep,
        minTime = nowTime;
        Record->save();
    }

    return 1;
}
