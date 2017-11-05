#include "record.h"

record::record(QObject *parent) : QObject(parent)
{
    maxValue = 999999;
    for (int i=5; i<=7; i++)
        for (int j=1; j<=30; j++)
            minStep[i][j] = minTime[i][j] = maxValue;

    ifstream recFin("record/record.txt");
    int hard, level, step, time;
    while (recFin>>hard>>level>>step>>time) {
        minStep[hard][level] = step;
        minTime[hard][level] = time;
    }
    recFin.close();

    ifstream needFin;
    for (int i=5; i<=7; i++)
        for (int j=1; j<=30; j++) {
            string path = "map/";
            path.push_back(i+'0');
            path.push_back('/');
            if (j>=10) path.push_back(j/10 + '0');
            path.push_back(j % 10 + '0');
            path.append(".txt");
            needFin.open(path.c_str());
            int tmp;
            if (needFin>>tmp)
                needStep[i][j] = tmp;
            needFin.close();
        }
}

void record::save() {
    ofstream recFout("record/record.txt");
    for (int i=5; i<=7; i++)
        for (int j=1; j<=30; j++)
            if (minStep[i][j] < maxValue)
                recFout<<i<<" "<<j<<" "<<minStep[i][j]<<" "<<minTime[i][j]<<endl;
    recFout.close();
}
