#ifndef SELECTWINDOW_H
#define SELECTWINDOW_H

#include "mainwindow.h"
#include "aboutwindow.h"
#include "closebutton.h"
#include "normalbutton.h"
#include "record.h"
#include <QDialog>
#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QtGui>

extern record *Record;

class SelectWindow : public QDialog
{
    Q_OBJECT

public:
    SelectWindow(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    ~SelectWindow();
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    QPoint last;

    QGridLayout *mainG;
    QListWidget *leftL, *rightL;
    CloseButton *exitB;
    NormalButton *beginB, *aboutB;

private slots:
    void begin();
    void readLevel();
    void about();
};

#endif // SELECTWINDOW_H
