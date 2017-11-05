#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "state.h"
#include "closebutton.h"
#include "normalbutton.h"
#include "helpbutton.h"
#include "backalime.h"
#include "solve.h"
#include <QDialog>
#include <QProgressDialog>
#include <QPainter>
#include <QPushButton>
#include <QMouseEvent>
#include <QLCDNumber>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QMediaPlayer>
#include <QMessageBox>

extern record *Record;

class mainwindow : public QDialog
{
    Q_OBJECT
public:
    explicit mainwindow(int, int, QWidget *parent = 0);

    void setLevelSetting(int, int);
    void setSelectWindow(QDialog *);
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);

signals:

public slots:

private slots:
    void refreshNowTime();
    void quit();
    void previousLevel();
    void restart();
    void nextLevel();
    void toRepaint();
    void gethelp();
    void workhelp();

private:
    QPoint getCell(QPoint&);

    QDialog *selectWindow;
    CloseButton *exitB;
    QLCDNumber *nflL, *tflL, *perL, *nowtL, *besttL;
    QTimer *nowtimeT, *paintT, *animeT, *helpT;
    NormalButton *preB, *resB, *nexB;
    HelpButton *helpB;
    QMediaPlayer *player;
    QProgressDialog *progressDlg;

    BackAlime *backalime;

    int hardLevel, levelLevel;
    state *nowS, *backup;
    QPoint nowAtCell;
    int xl[8][8], xr[8][8], yl[8][8], yr[8][8], cellSize;

    QPoint nowMouse, globalMouse, pressMouse;
    bool clicked, busy;

    int nowtime;
    bool passWindowShow, helped;
    int preflow;

    QVector<QPoint> nowPath;
};

#endif // MAINWINDOW_H
