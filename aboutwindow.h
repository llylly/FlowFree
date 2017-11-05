#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "normalbutton.h"
#include <QMainWindow>
#include <QDialog>
#include <QPainter>
#include <QLabel>

class aboutwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit aboutwindow(QWidget *parent = 0);
    void setSelectWindow(QDialog*);
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void switchIt();
    void quit();

private:
    QDialog *s;
    NormalButton *switchB, *OKB;
    QLabel *inforL;
    bool status;
};

#endif // ABOUTWINDOW_H
