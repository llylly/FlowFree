#include "aboutwindow.h"

int ShowAbout = 0;

aboutwindow::aboutwindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle(QStringLiteral("Flow Free"));
    this->resize(300, 300);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    switchB = new NormalButton(this);
    switchB->setText("幫助");
    switchB->setGeometry(50, 250, 90, 30);
    connect(switchB, SIGNAL(clicked(bool)), this, SLOT(switchIt()));

    OKB = new NormalButton(this);
    OKB->setText("確定");
    OKB->setGeometry(160, 250, 90, 30);
    connect(OKB, SIGNAL(clicked(bool)), this, SLOT(quit()));

    inforL = new QLabel(this);
    inforL->setStyleSheet("background-color: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, "
                          "stop: 0 rgb(150, 150, 150, 150), stop: 0.5 rgb(0,0,0,255),"
                          "stop: 1 rgb(150, 150, 150, 150));"
                          "color: white;"
                          "border-radius: 20");
    inforL->setGeometry(50, 50, 200, 180);
    inforL->setAlignment(Qt::AlignCenter);
    inforL->setText("作者：李林翼  學號：2014011361 \n"
                    "課程學號：32 \n\n"
                    "    此遊戲為作者個人之程序設計\n訓練課程之第一星期之大作業，"
                    "僅\n用於測試及內部演示。          ");
    status = 0;
}

void aboutwindow::switchIt() {
    if (status == 0) {
        switchB->setText("作者");
        inforL->setText("使用鼠標拖動來連接有顏色的管道，\n形成水流。                       \n\n"
                        "連接每一對相同顏色的管道，並且覆\n蓋整塊棋盤。                     \n\n"
                        "請注意：如果相交或覆蓋，管道將會\n破裂！                          \n\n");
        status = 1;
    } else {
        switchB->setText("幫助");
        inforL->setText("作者：李林翼  學號：2014011361 \n"
                        "課程學號：32 \n\n"
                        "    此遊戲為作者個人之程序設計\n訓練課程之第一星期之大作業，"
                        "僅\n用於測試及內部演示。          ");
        status = 0;
    }
}

void aboutwindow::quit() {
    ShowAbout = 0;
    s->show();
    this->close();
}

void aboutwindow::setSelectWindow(QDialog *S) {
    s = S;
}

void aboutwindow::paintEvent(QPaintEvent *e) {
    QPainter paint(this);
    paint.setRenderHint(QPainter::Antialiasing, true);
    QLinearGradient background(150, 400, 150, 0);
    background.setColorAt(0.0, QColor(150,150,150,200));
    background.setColorAt(0.1, QColor(0,0,0,200));
    background.setColorAt(0.9, QColor(0,0,0,150));
    background.setColorAt(1.0, QColor(0,0,0,255));
    paint.setPen(QPen(Qt::NoPen));
    paint.setBrush(QBrush(background));
    paint.drawRoundRect(QRect(0, 0, this->size().width(), this->size().height()), 10, 10);

    paint.setPen(QPen(Qt::white,5));
    paint.setFont(QFont("Arial",12,QFont::Bold,true));
    paint.drawText(QPoint(10, 25), "Flow Free");
}
