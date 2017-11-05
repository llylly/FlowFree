#include "mainwindow.h"

extern QMediaPlayer *backPlayer;
bool KMT = 0;

mainwindow::mainwindow(int L1, int L2, QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle(QStringLiteral("Flow Free"));
    this->resize(600, 700);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    exitB = new CloseButton(this);
    exitB->setGeometry(size().width()-30,0,30,30);

    preB = new NormalButton(this);
    preB->setGeometry(size().width()/2-210, size().height()/2+300, 70, 30);
    preB->setText(QStringLiteral("上一關"));

    resB = new NormalButton(this);
    resB->setGeometry(size().width()/2-35, size().height()/2+300, 70, 30);
    resB->setText(QStringLiteral("重新開始"));

    nexB = new NormalButton(this);
    nexB->setGeometry(size().width()/2+140, size().height()/2+300, 70, 30);
    nexB->setText(QStringLiteral("下一關"));

    helpB = new HelpButton(this);
    helpB->setGeometry(size().width()/2-20, size().height()/2+250, 40, 40);
    connect(helpB, SIGNAL(clicked(bool)), this, SLOT(gethelp()));

    perL = new QLCDNumber(this);
    perL->setGeometry(size().width()/2+140, size().height()/2-240,100,30);
    perL->setFrameShape(QFrame::NoFrame);
    perL->setDigitCount(3);
    perL->setProperty("value", QVariant(0));

    nflL = new QLCDNumber(this);
    nflL->setGeometry(size().width()/2-220, size().height()/2-240,30,30);
    nflL->setFrameShape(QFrame::NoFrame);
    nflL->setDigitCount(1);
    nflL->setProperty("value", QVariant(0));

    tflL = new QLCDNumber(this);
    tflL->setGeometry(size().width()/2-190, size().height()/2-240,30,30);
    tflL->setFrameShape(QFrame::NoFrame);
    tflL->setDigitCount(1);
    tflL->setProperty("value", QVariant(0));

    nowtL = new QLCDNumber(this);
    nowtL->setGeometry(size().width()/2-130, size().height()/2+215,60,30);
    nowtL->setFrameShape(QFrame::NoFrame);
    nowtL->setDigitCount(4);
    nowtL->setProperty("value", QVariant(0));

    besttL = new QLCDNumber(this);
    besttL->setGeometry(size().width()/2+150, size().height()/2+215,60,30);
    besttL->setFrameShape(QFrame::NoFrame);
    besttL->setDigitCount(4);
    besttL->setProperty("value", QVariant(0));

    nowtimeT = new QTimer(this);
    connect(nowtimeT, SIGNAL(timeout()), this, SLOT(refreshNowTime()));
    nowtimeT->start(1000);

    paintT = new QTimer(this);
    connect(paintT, SIGNAL(timeout()), this, SLOT(toRepaint()));
    paintT->start(1000 / 24);

    backalime = new BackAlime;
    animeT = new QTimer(this);
    connect(animeT, SIGNAL(timeout()), backalime, SLOT(refresh()));
    animeT->start(1000 / 24);

    player = new QMediaPlayer(this);
    player->setMedia(QUrl::fromLocalFile("sound/bignews.wav"));
    player->play();

    connect(exitB, SIGNAL(clicked(bool)), this, SLOT(quit()));
    connect(preB, SIGNAL(clicked(bool)), this, SLOT(previousLevel()));
    connect(resB, SIGNAL(clicked(bool)), this, SLOT(restart()));
    connect(nexB, SIGNAL(clicked(bool)), this, SLOT(nextLevel()));
    preB->installEventFilter(this);
    resB->installEventFilter(this);
    nexB->installEventFilter(this);
    helpB->installEventFilter(this);

    setLevelSetting(L1, L2);

    nowAtCell = QPoint(0, 0);
    clicked = busy = 0;
    passWindowShow = 0;
    nowtime = 0;
    helped = 0;
    preflow = 0;
}

bool mainwindow::eventFilter(QObject *o, QEvent *e) {
    if (e->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mE = static_cast<QMouseEvent*> (e);
        pressMouse = mE->globalPos() - this->pos();
    }
    return 0;
}

void mainwindow::setLevelSetting(int L1, int L2) {
    hardLevel = L1, levelLevel = L2;
    nowS = new state(hardLevel, levelLevel);
}


void mainwindow::paintEvent(QPaintEvent *) {
    QPainter P(this);

    P.setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient background(400, 600, 400, 0);
    background.setColorAt(0.0, QColor(150,150,150,200));
    background.setColorAt(0.1, QColor(0,0,0,200));
    background.setColorAt(0.9, QColor(0,0,0,150));
    background.setColorAt(1.0, QColor(0,0,0,255));
    //設置背景色

    P.setBrush(QBrush(background));
    P.setPen(QPen(Qt::NoPen));
    P.drawRoundRect(QRect(0, 0, this->size().width(), this->size().height()), 5, 5);
    //繪製背景

    for (QList<QuadGroup>::iterator i=(backalime->vet.end() - 1); i!=backalime->vet.begin(); i--) {
        QColor nowC = nowS->colorSet[i->c];
        nowC = nowC.darker();
        nowC.setAlpha(100);
        P.setPen(QPen(nowC,30,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        QPoint p1 = QPoint(i->x, i->y), p2;
        switch (i->d) {
            case 1: p2 = p1 + QPoint(i->l, 0); break;
            case 2: p2 = p1 + QPoint(i->l, 0); break;
            case 3: p2 = p1 + QPoint(0, i->l); break;
            case 4: p2 = p1 + QPoint(0, i->l); break;
        }
        P.drawLine(p1, p2);
    }
    //繪製背景動畫

    P.setPen(QPen(QColor(200,200,200),3,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin));
    //設置筆觸

    QVector<QPoint> borderPoints;
    borderPoints.clear();
    borderPoints.push_back(QPoint(size().width()/2-210,size().height()/2-210)),
    borderPoints.push_back(QPoint(size().width()/2-210,size().height()/2+210)),
    borderPoints.push_back(QPoint(size().width()/2+210,size().height()/2+210)),
    borderPoints.push_back(QPoint(size().width()/2+210,size().height()/2-210)),
    P.drawPolygon(QPolygon(borderPoints));
    //繪製邊框

    int tmp = size().width()/2-210;
    for (int i=1; i<=hardLevel-1; i++) {
        tmp += (420 / hardLevel);
        P.drawLine(QPoint(tmp, size().height()/2-210), QPoint(tmp, size().height()/2+210));
    }
    tmp = size().height()/2-210;
    for (int i=1; i<=hardLevel-1; i++) {
        tmp += (420 / hardLevel);
        P.drawLine(QPoint(size().width()/2-210, tmp), QPoint(size().width()/2+210, tmp));
    }
    //繪製格線

    for (int i=1; i<=hardLevel; i++) {
        for (int j=1; j<=hardLevel; j++) {
            xl[i][j] = size().width()/2-210 + (420 / hardLevel) * (j-1);
            xr[i][j] = size().width()/2-210 + (420 / hardLevel) * (j);
            yl[i][j] = size().height()/2-210 + (420 / hardLevel) * (i-1);
            yr[i][j] = size().height()/2-210 + (420 / hardLevel) * (i);
        }
    }
    cellSize = 420 / hardLevel;
    //紀錄每個格子的座標

    for (int i=1; i<=hardLevel; i++)
        for (int j=1; j<=hardLevel; j++)
            if (nowS->netColor[i][j]) {
                P.setPen(Qt::NoPen);
                QColor nowc = nowS->colorSet[nowS->netColor[i][j]];
                if (nowc.red()<=205) nowc.setRed(nowc.red()+50);
                if (nowc.green()<=205) nowc.setGreen(nowc.green()+50);
                if (nowc.blue()<=205) nowc.setBlue(nowc.blue()+50);
                nowc.setAlpha(100);
                P.setBrush(QBrush(nowc));
                P.drawRect(QRect(xl[i][j], yl[i][j], 420 / hardLevel, 420 / hardLevel));
            }
    //繪製格子的背景

    for (int i=1; i<=nowS->totFlows; i++)
        if (nowS->flowTrack[i].size()>=2) {
            for (int j=0; j<=nowS->flowTrack[i].size()-2; j++) {
                QColor tmpC = nowS->colorSet[i];
                tmpC.setAlpha(200);
                P.setPen(QPen(tmpC,7,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
                QPoint p1 = QPoint(xl[nowS->flowTrack[i][j].x()][nowS->flowTrack[i][j].y()], yl[nowS->flowTrack[i][j].x()][nowS->flowTrack[i][j].y()]) +
                        QPoint(420 / hardLevel / 2, 420 / hardLevel / 2),
                       p2 = QPoint(xl[nowS->flowTrack[i][j+1].x()][nowS->flowTrack[i][j+1].y()], yl[nowS->flowTrack[i][j+1].x()][nowS->flowTrack[i][j+1].y()]) +
                        QPoint(420 / hardLevel / 2, 420 / hardLevel / 2);
                P.drawLine(p1, p2);
            }
        }
    //畫連線

    for (int i=1; i<=nowS->totFlows; i++) {
        for (int j=0; j<2; j++) {
            QPoint cellNo = nowS->flowDest[i][j];
            QPoint cellPos;
            cellPos.setX((xl[cellNo.x()][cellNo.y()]+xr[cellNo.x()][cellNo.y()])/2);
            cellPos.setY((yl[cellNo.x()][cellNo.y()]+yr[cellNo.x()][cellNo.y()])/2);
            P.setPen(Qt::NoPen);

            if (nowS->flowTrack[i].size()) {
                int s = nowS->flowTrack[i].size()-1;
                if ((nowS->netDest[nowS->flowTrack[i][0].x()][nowS->flowTrack[i][0].y()])
                        && (nowS->netDest[nowS->flowTrack[i][s].x()][nowS->flowTrack[i][s].y()])) {
                    QRadialGradient lightcolor(cellPos, cellSize/2);
                    lightcolor.setColorAt(0, QColor(255, 255, 255, 255));
                    lightcolor.setColorAt(0.8, QColor(255, 255, 255, 255));
                    lightcolor.setColorAt(1.0, QColor(255, 255, 255, 0));
                    P.setBrush(lightcolor);
                    P.drawEllipse(cellPos, cellSize/2, cellSize/2);
                }
            }

            QRadialGradient circleColor(cellPos, cellSize);
            circleColor.setColorAt(0, nowS->colorSet[i]);
            QColor tmpColor = nowS->colorSet[i];
            tmpColor.setAlpha(0);
            circleColor.setColorAt(1, tmpColor);
            P.setBrush(circleColor);

            P.drawEllipse(cellPos, cellSize*2/5, cellSize*2/5);
        }
    }
    //畫管頭

    if (nowAtCell != QPoint(0, 0)) {
        P.setPen(QPen(Qt::NoPen));
        P.setBrush(QBrush(QColor(255, 255, 255, 50)));
        P.drawRect(QRect(xl[nowAtCell.x()][nowAtCell.y()], yl[nowAtCell.x()][nowAtCell.y()], cellSize, cellSize));
    }
    //對選中的格子進行高亮

    if ((nowMouse.x() >= size().width()/2-210) && (nowMouse.x() <= size().width()/2+210) &&
        (nowMouse.y() >= size().height()/2-210) && (nowMouse.y() <= size().height()/2+210) &&
        clicked &&
        busy && (nowS->nowColor)) {
        P.setPen(QPen(Qt::NoPen));
        QColor nowColor = nowS->colorSet[nowS->nowColor];
        nowColor.setAlpha(50);
        P.setBrush(QBrush(nowColor));
        P.drawEllipse(nowMouse, 20, 20);
    }
    //在光標前畫圈

    P.setPen(QPen(Qt::white,5));
    P.setFont(QFont("Arial", 12, QFont::Bold ,true));
    P.drawText(QPoint(15, 25), "Flow Free");
    //標題欄

    P.setPen(Qt::yellow);
    P.setFont(QFont("Courier New", 20, QFont::Bold));
    P.drawText(QPoint(15,50), "Level "+QString::number(levelLevel));

    P.setPen(QColor(230, 230, 230));
    P.setFont(QFont("Arial", 12));
    P.drawText(QPoint(150,50), QString::number(hardLevel) + " × " + QString::number(hardLevel));
    //關卡欄

    int totCover = 0;
    for (int i=1; i<=hardLevel; i++)
        for (int j=1; j<=hardLevel; j++)
            if (nowS->netColor[i][j])
                totCover ++;
    int perC = totCover * 100 / (hardLevel * hardLevel);
    //計算完成率

    P.setPen(QColor(255, 255, 255, 230));
    P.setFont(QFont("黑体", 12));
    P.drawText(QPoint(size().width()/2-194, size().height()/2-220), "/");
    nflL->setProperty("value", QVariant(nowS->nowFlows));
    tflL->setProperty("value", QVariant(nowS->totFlows));

    if ((nowS->nowStep >= nowS->minStep) && (!helped))
        P.setPen(QColor(255, 60, 221, 230));

    QString text = QStringLiteral("步數: ");
    if (helped) text += "---"; else text += QString::number(nowS->nowStep);
    text += "  最佳紀錄: ";
    if (nowS->minStep == nowS->maxValue) text += "---"; else text += QString::number(nowS->minStep);

    P.drawText(QPoint(size().width()/2-100, size().height()/2-220), text);

    P.setPen(QColor(255, 255, 255, 230));
    P.drawText(QPoint(size().width()/2+110, size().height()/2-220),
               QStringLiteral("覆蓋比: "));
    perL->setProperty("value", QVariant(perC));

    nowtL->setProperty("value", QVariant(nowtime));
    besttL->setProperty("value", QVariant(nowS->minTime));
    P.drawText(QPoint(size().width()/2-210, size().height()/2+240),
               QStringLiteral("當前用時:"));

    text = QStringLiteral("最短用時:  ");
    if (besttL->isHidden()) besttL->show();
    if (nowS->minTime == nowS->maxValue) {
        text += "---";
        besttL->hide();
    }

    P.drawText(QPoint(size().width()/2+70, size().height()/2+240), text);
    //數據顯示

    P.setPen(QPen(Qt::white, 3));

    if ((nowMouse.x() >= size().width()/2-210) && (nowMouse.x() <= size().width()/2+210) &&
        (nowMouse.y() >= size().height()/2-210) && (nowMouse.y() <= size().height()/2+210)) {
        P.drawArc(nowMouse.x() - 25, nowMouse.y() - 25, 50, 50, 15.7*(45+backalime->angle), 15.7*90);
        P.drawArc(nowMouse.x() - 25, nowMouse.y() - 25, 50, 50, 15.7*(225+backalime->angle), 15.7*90);
        QColor nowColor = Qt::white;
        nowColor = nowColor.darker();
        P.setPen(QPen(nowColor, 1));
        P.drawEllipse(nowMouse.x() - 20, nowMouse.y() - 20, 40, 40);
        P.setPen(QPen(Qt::white, 2));
        P.drawLine(nowMouse.x() - 10, nowMouse.y(), nowMouse.x() + 10, nowMouse.y());
        P.drawLine(nowMouse.x(), nowMouse.y() - 10, nowMouse.x(), nowMouse.y() + 10);
        this->setCursor(Qt::BlankCursor);
        if (KMT)
            P.drawPixmap(nowMouse.x() - 20, nowMouse.y() - 20, 40, 40, QPixmap(":kmt/kmt.png"));
    } else
        this->setCursor(Qt::ArrowCursor);
    //顯示光標

    P.end();
}

void mainwindow::mouseMoveEvent(QMouseEvent *e) {
    if ((pressMouse.y()<=50) && (clicked)) {
        QPoint D = e->globalPos() - globalMouse;
        this->move(this->pos()+D);
    }

    nowMouse = e->pos(), globalMouse = e->globalPos();

    nowAtCell = getCell(nowMouse);
    if (nowAtCell != QPoint(0, 0)) {
        if (busy) {
            for (int i=1; i<=nowS->totFlows; i++)
                if (i!=nowS->nowColor)
                    if ((nowAtCell == nowS->flowDest[i][0]) || (nowAtCell == nowS->flowDest[i][1]))
                        busy = 0;
        }
        if (busy) {
            int no=100;
            for (int i=0; i<nowPath.size(); i++) {
                if (nowPath[i] == nowAtCell)
                    no = i;
            }
            while ((nowPath.size() > no) && (nowPath.size())) nowPath.pop_back();
            if (!nowPath.isEmpty()) {
                QPoint top = nowPath.last();
                if (abs(top.x()-nowAtCell.x())+abs(top.y()-nowAtCell.y())>1)
                    busy = 0;
            }
        }
        if (busy) {
            nowPath.push_back(nowAtCell);
            nowS = new state(backup);
            nowS->refresh(nowPath);
        }
    } else
        busy = 0;

    if ((e->buttons() & Qt::LeftButton) || (e->buttons() & Qt::RightButton))
        clicked = 1;
    else
        clicked = 0;

    if (nowS->nowFlows < preflow) {
        player->setMedia(QUrl::fromLocalFile("sound/naive.wav"));
        player->play();
    } else
    if ((nowS->nowFlows > preflow) && (nowS->nowFlows < nowS->totFlows) && (!helped)) {
        player->setMedia(QUrl::fromLocalFile("sound/good.wav"));
        player->play();
    };
    preflow = nowS->nowFlows;
}

void mainwindow::mousePressEvent(QMouseEvent *e) {
    nowMouse = e->pos(), globalMouse = e->globalPos();

    QPoint nowC = getCell(nowMouse);
    if ((nowC != QPoint(0, 0)) &&
        (nowS->netColor[nowC.x()][nowC.y()] | nowS->netDest[nowC.x()][nowC.y()])) {
        player->setMedia(QUrl::fromLocalFile("sound/open.wav"));
        player->play();

        int tColor = 0;
        if (nowS->netColor[nowC.x()][nowC.y()]) tColor = nowS->netColor[nowC.x()][nowC.y()];
        if (nowS->netDest[nowC.x()][nowC.y()]) tColor = nowS->netDest[nowC.x()][nowC.y()];

        if (tColor != nowS->nowColor) {
            ++nowS->nowStep;
            nowS->nowColor = tColor;
        }
        nowPath.clear();
        nowPath.push_back(nowC);
        nowS->refresh(nowPath);
        backup = new state(nowS);
        busy = 1;
    }

    if ((e->buttons() & Qt::LeftButton) || (e->buttons() & Qt::RightButton))
        clicked = 1;
    else
        clicked = 0;

    pressMouse = nowMouse;
}

void mainwindow::mouseReleaseEvent(QMouseEvent *e) {
    if ((pressMouse.y()<=50) && (clicked)) {
        QPoint D = e->globalPos() - globalMouse;
        this->move(this->pos()+D);
    }

    nowMouse = e->pos();

    busy = 0;

    if ((e->buttons() & Qt::LeftButton) || (e->buttons() & Qt::RightButton))
        clicked = 1;
    else
        clicked = 0;

    if (!helped)
    if ((nowS->checkSuccess(nowtime)) && (!passWindowShow)) {
        if ((hardLevel == 7) && (levelLevel == 30)) KMT = 1;

        player->setMedia(QUrl::fromLocalFile("sound/victory.wav"));
        player->play();
        preflow = 0;

        passWindowShow = 1;
        QString text;
        if ((nowS->nowStep == nowS->minStep) && (nowtime == nowS->minTime))
            text = QStringLiteral("恭喜您成功通關, 並創造了新紀錄! 是否前往下一關卡?");
        else
            text = QStringLiteral("恭喜您成功通關! 是否前往下一關卡?");
        QMessageBox success(QMessageBox::Information, QStringLiteral("Free Flow"), text, QMessageBox::Yes | QMessageBox::No);
        busy = clicked = 0;
        if (success.exec() == QMessageBox::Yes) {
            nextLevel();
        };
    } else if (!nowS->checkSuccess(nowtime))
        passWindowShow = 0;
}

void mainwindow::setSelectWindow(QDialog *s) {
    selectWindow = s;
}

void mainwindow::quit() {
    player->setMedia(QUrl::fromLocalFile("sound/imangry.wav"));
    player->play();

    this->selectWindow->repaint();
    this->selectWindow->show();
    this->close();
}

void mainwindow::previousLevel() {
    busy = clicked = 0;
    int La = hardLevel, Lb = levelLevel;
    if ((La == 5) && (Lb == 1)) {

    } else {
        Lb--;
        if (Lb <= 0)
            Lb = 30, La--;
    }
    hardLevel = La, levelLevel = Lb;
    nowS = new state(hardLevel, levelLevel);
    backup = NULL;
    nowtime = 0;
    nowtimeT->stop();
    nowtimeT->start(1000);
    helped = 0;
}

void mainwindow::restart() {
    clicked = busy = 0;
    passWindowShow = 0;
    nowS = new state(hardLevel, levelLevel);
    backup = NULL;
    nowtime = 0;
    nowtimeT->stop();
    nowtimeT->start(1000);
    helped = 0;
}

void mainwindow::nextLevel() {
    busy = clicked = 0;
    int La = hardLevel, Lb = levelLevel;
    if ((La == 7) && (Lb == 30)) {

    } else {
        Lb++;
        if (Lb > 30)
            Lb = 1, La++;
    }
    hardLevel = La, levelLevel = Lb;
    nowS = new state(hardLevel, levelLevel);
    backup = NULL;
    nowtime = 0;
    nowtimeT->stop();
    nowtimeT->start(1000);
    helped = 0;
}

QPoint mainwindow::getCell(QPoint &nowPoint) {
    bool atCell = 0;
    QPoint Y;
    for (int i=1; i<=hardLevel; i++)
        for (int j=1; j<=hardLevel; j++)
            if ((nowPoint.x()>=xl[i][j]) && (nowPoint.x()<xr[i][j]) &&
                (nowPoint.y()>=yl[i][j]) && (nowPoint.y()<yr[i][j])) {
                atCell = 1;
                Y = QPoint(i, j);
            }
    if (!atCell) Y = QPoint(0, 0);
    return Y;
}

void mainwindow::refreshNowTime() {
    nowtime++;
}

void mainwindow::toRepaint() {
    repaint();
}

void mainwindow::gethelp() {
    QMessageBox QM(QMessageBox::Information, QStringLiteral("Free FLow"), QStringLiteral("確定需要幫助嗎?"), QMessageBox::Yes | QMessageBox::No, this);
    if (QM.exec() == QMessageBox::Yes) {
        player->setMedia(QUrl::fromLocalFile("sound/experience.wav"));
        player->play();

        progressDlg=new QProgressDialog(this);
        progressDlg->setWindowModality(Qt::WindowModal);
        progressDlg->setMinimumDuration(5);
        progressDlg->setWindowTitle(tr("please wait"));
        progressDlg->setLabelText(tr("正在求解......"));
        progressDlg->setCancelButtonText(tr("取消"));
        progressDlg->setRange(0,100);
        progressDlg->show();

        helpT = new QTimer(this);
        connect(helpT, SIGNAL(timeout()), this, SLOT(workhelp()));
        helpT->start(10);
    }
}

void mainwindow::workhelp() {
    helpT->stop();
    helped = 1;
    Solve solve;
    solve.setState(nowS);
    solve.work();
    nowS = solve.getResult();
    progressDlg->close();
}
