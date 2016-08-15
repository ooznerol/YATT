#-------------------------------------------------
#
# Project created by QtCreator 2015-08-05T16:37:40
#
#-------------------------------------------------

QT       += core gui

QT += network
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTrading
TEMPLATE = app




SOURCES += main.cpp\
        qtrading.cpp \
    modeltrades.cpp
#   // sleeper.cpp

HEADERS  += qtrading.h \
    sleeper.h \
    modeltrades.h \
    common.h

FORMS    += qtrading.ui
