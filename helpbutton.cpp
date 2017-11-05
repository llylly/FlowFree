#include "helpbutton.h"

HelpButton::HelpButton(QWidget *parent) : QPushButton(parent)
{
    this->setStyleSheet("QPushButton{border-image: url(:/helpB/1.png);}"
                        "QPushButton:hover{border-image: url(:/helpB/2.png);}"
                        "QPushButton:pressed{border-image: url(:/helpB/3.png);}");

}

