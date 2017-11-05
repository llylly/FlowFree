#-------------------------------------------------
#
# Project created by QtCreator 2015-08-26T09:04:25
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flowfree
TEMPLATE = app

RC_FILE += icon/icon.rc


SOURCES += main.cpp\
        selectwindow.cpp \
    mainwindow.cpp \
    state.cpp \
    record.cpp \
    closebutton.cpp \
    normalbutton.cpp \
    aboutwindow.cpp \
    helpbutton.cpp \
    backalime.cpp \
    solve.cpp

HEADERS  += selectwindow.h \
    mainwindow.h \
    state.h \
    record.h \
    closebutton.h \
    normalbutton.h \
    aboutwindow.h \
    helpbutton.h \
    backalime.h \
    solve.h

DISTFILES +=

RESOURCES += \
    closeB/closeb.qrc \
    levels/levels.qrc \
    stateI/statei.qrc \
    helpB/helpb.qrc \
    kmt/kmt.qrc \
    icon/icon.qrc


