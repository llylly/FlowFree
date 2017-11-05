#include "closebutton.h"

CloseButton::CloseButton(QWidget *parent) : QPushButton(parent)
{
    this->setStyleSheet("QPushButton{border-image: url(:/closeB/1.png);}"
                        "QPushButton:hover{border-image: url(:/closeB/2.png);}"
                        "QPushButton:pressed{border-image: url(:/closeB/3.png);}");
}

