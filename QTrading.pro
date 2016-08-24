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
	lsclient.cpp \
    	lssubscription.cpp \
    	lstable.cpp \
    	modeltrades.cpp \
    	dialogconnection.cpp \
#    http.cpp
#   // sleeper.cpp

HEADERS  += qtrading.h \
    lsclient.h \
    lssubscription.h \
    lstable.h \
    assoc.h \
    sleeper.h \
    modeltrades.h \
    common.h \
    dialogconnection.h \
#    http.hpp

FORMS    += qtrading.ui \
    dialogconnection.ui

RESOURCES += \
    image.qrc

DISTFILES += \
    Qtrading.rc

win32:RC_ICONS += finance.ico


#######################################
#WINDEPLOYQT
#######################################
win32 {
    DEPLOY_COMMAND = windeployqt
}
macx {
    DEPLOY_COMMAND = macdeployqt
}
DEPLOY_TARGET = $$OUT_PWD

#Uncomment the following line to help debug the deploy command when running qmake
#warning($${DEPLOY_COMMAND} $$shell_quote($${DEPLOY_TARGET}/release))
QMAKE_POST_LINK += $${DEPLOY_COMMAND} $$shell_quote($${DEPLOY_TARGET}/release) &
