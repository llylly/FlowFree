#include "selectwindow.h"

extern int ShowAbout;

SelectWindow::SelectWindow(QWidget *parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle(QStringLiteral("Flow Free"));
    this->resize(480,480);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setMouseTracking(true);

    mainG = new QGridLayout(this);
    mainG->setSpacing(6);
    mainG->setContentsMargins(50,100,50,50);

    leftL = new QListWidget(this);
    leftL->setMaximumWidth(50);
    leftL->setStyleSheet("background-color:rgb(0,0,0,100); color:white; "
                         "border-width:0px; border-radius:5px");
    mainG->addWidget(leftL, 0, 0, 1, 1);

    rightL = new QListWidget(this);
    rightL->setViewMode(QListView::IconMode);
    rightL->setDragEnabled(false);
    rightL->setStyleSheet("QListWidget{ background-color:rgb(0,0,0,100); color:white; "
                          "border-width:0px; border-radius:5px} "
                          );
    mainG->addWidget(rightL, 0, 1, 1, 1);

    beginB = new NormalButton(this);
    beginB->setMinimumHeight(50);
    beginB->setText(QStringLiteral("開始遊戲!"));
    mainG->addWidget(beginB, 1, 0, 1, 2);

    aboutB = new NormalButton(this);
    aboutB->setText(QStringLiteral("?"));
    aboutB->setGeometry(450,450,20,20);

    exitB = new CloseButton(this);
    exitB->setGeometry(450, 0, 30, 30);

    leftL->addItem(QStringLiteral("5 x 5"));
    leftL->addItem(QStringLiteral("6 x 6"));
    leftL->addItem(QStringLiteral("7 x 7"));

    connect(exitB, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(beginB, SIGNAL(clicked(bool)), this, SLOT(begin()));
    connect(leftL, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(readLevel()));
    connect(aboutB, SIGNAL(clicked(bool)), this, SLOT(about()));
}

void SelectWindow::readLevel() {
    rightL->clear();
    int L1 = (*(leftL->selectedItems().first()->text().begin())).toLatin1()-'0';

    for (int i=1; i<=30; i++) {
        if (Record->minStep[L1][i] == Record->maxValue)
            rightL->addItem(new QListWidgetItem(QIcon(QObject::tr(":stateI/1.png")),QString::number(i)));
        else if (Record->minStep[L1][i] == Record->needStep[L1][i])
            rightL->addItem(new QListWidgetItem(QIcon(QObject::tr(":stateI/3.png")),QString::number(i)));
        else
            rightL->addItem(new QListWidgetItem(QIcon(QObject::tr(":stateI/2.png")),QString::number(i)));
    }
}

void SelectWindow::begin() {
    if ((leftL->selectedItems().size() == 0) || (rightL->selectedItems().size() == 0)) {
        QMessageBox q(QMessageBox::Information, QStringLiteral("錯誤"),
                      QStringLiteral("請選擇一個關卡。"), QMessageBox::Yes);
        q.setWindowModality(Qt::WindowModal);
        q.exec();
        return;
    }

    int L1 = (*(leftL->selectedItems().first()->text().begin())).toLatin1()-'0';
    int L2 = rightL->selectedItems().first()->text().toInt();
    mainwindow *mainW = new mainwindow(L1, L2);
    mainW->setSelectWindow(this);
    this->hide();
    mainW->setWindowModality(Qt::WindowModal);
    mainW->exec();
}

void SelectWindow::about() {
    if (!ShowAbout) {
        ShowAbout = 1;
        aboutwindow *aboutW = new aboutwindow();
        aboutW->setSelectWindow(this);
        aboutW->setWindowModality(Qt::WindowModal);
        aboutW->show();
    }
}

void SelectWindow::mousePressEvent(QMouseEvent *e) {
    last = e->globalPos();
}

void SelectWindow::mouseMoveEvent(QMouseEvent *e) {
    QPoint delta = e->globalPos() - last;
    if (e->buttons() & Qt::LeftButton)
        this->move(this->pos() + delta);
    last = e->globalPos();
}

void SelectWindow::mouseReleaseEvent(QMouseEvent *e) {
    QPoint delta = e->globalPos() - last;
    this->move(this->pos() + delta);
}

void SelectWindow::paintEvent(QPaintEvent *e) {
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

    paint.drawPixmap(this->width()/2-84, this->height()/2-200, QPixmap(":levels/levels.png"));
}

SelectWindow::~SelectWindow()
{

}
