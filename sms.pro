#-------------------------------------------------
#
# Project created by QtCreator 2014-10-01T19:47:13
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sms
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    conn.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
